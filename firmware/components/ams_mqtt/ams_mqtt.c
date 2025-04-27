#include "ams_mqtt.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_partition.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_random.h"

#include "esp_log.h"
#include "esp_tls.h"
#include <sys/param.h>

#include "cJSON.h"

static const char *TAG = "AMS_MQTTS";

extern const uint8_t mqtt_eclipseprojects_io_pem_start[]   asm("_binary_ca_certificate_pem_start");
extern const uint8_t mqtt_eclipseprojects_io_pem_end[]   asm("_binary_ca_certificate_pem_end");
extern int _g_ams_id;

char report_topic[64] = {0};
char request_topic[64] = {0};

char *extrusion_cali_list_ptr = NULL;

typedef struct {
    char *payload;
    size_t topic_len;
    size_t data_len;
    size_t data_offset;
} mqtt_message_t;

mqtt_message_t rec_msg_struct;

const char push_all_payload[] = "{\"pushing\": {\"sequence_id\": \"0\",\"command\": \"pushall\",\"version\": 1,\"push_target\": 1}}";
QueueHandle_t xqueue_ams_msg = NULL;

QueueHandle_t xqueue_print_msg = NULL;
QueueHandle_t xqueue_cali_get_msg = NULL;
QueueHandle_t xqueue_send_msg = NULL;

static message_t s_current_msg_in_flight = {0};
static bool s_waiting_response = false;
static mqtt_state_t s_mqtt_state = kSTATE_MQTT_DISCONNECTED;

esp_mqtt_client_handle_t client;

#define MQTT_RX_BUFF_SIZE   64*1024
#define MQTT_RX_TOPIC_SIZE  4*1024
#define CALILIST_BUFF_SIZE  16*1024

static int resolution_report_push_state(char *json_str,size_t len,cJSON* print,filament_msg_t *filament_msg)
{
    filament_msg->amd_id = -1;

    //mc_percent(num) -> 0 - 100%
    //mc_print_stage(string) -> "2"(unfinished) "1"(finished) "0"(Preparing)

    // 获取 "mc_percent" 对象
    cJSON *mc_percent = cJSON_GetObjectItem(print, "mc_percent");
    // 获取 "mc_print_stage" 对象
    cJSON *mc_print_stage = cJSON_GetObjectItem(print, "mc_print_stage");

    if (cJSON_IsString(mc_print_stage)) {
        //int _percent = mc_percent->valueint;
        const char *_print_stage_str = mc_print_stage->valuestring;
        int _print_stage = atoi(_print_stage_str);
        ESP_LOGI(TAG,"get mc_print_stage: %s->%d",_print_stage_str,_print_stage);
        //ESP_LOGI(TAG,"get 'mc_percent:%d' & 'mc_print_stage:%d'",_percent,_print_stage);
        if(_print_stage == 1){
            ESP_LOGI(TAG,"print finished");
            filament_msg->sub_event_id = kEVENT_ID_PRINT_FINISHED;
            //TODO: send print finished event
        }
        else if(_print_stage == 2){
            ESP_LOGI(TAG,"print Printing");
            filament_msg->sub_event_id = kEVENT_ID_PRINT_PRINTING;
            //TODO: send print paused event
        }  
        else if(_print_stage == 0){
            ESP_LOGI(TAG,"print preparing");
            //TODO: send print preparing event
        }
        xQueueSend(xqueue_ams_msg,filament_msg,(TickType_t)10);
    }

    // 获取 "ams" 对象
    cJSON *ams = cJSON_GetObjectItem(print, "ams");
    if (!cJSON_IsObject(ams)) {
        //ESP_LOGW(TAG,"Failed to get 'ams' object\n");
        return -1;
    }

    // 获取 "ams" 数组
    cJSON *ams_array = cJSON_GetObjectItem(ams, "ams");
    if (!cJSON_IsArray(ams_array)) {
        ESP_LOGW(TAG,"Failed to get 'ams' array\n");
        return -1;
    }

    // 遍历 "ams" 数组
    cJSON *ams_item = NULL;
    cJSON_ArrayForEach(ams_item, ams_array) {
        
        cJSON *j_ams_id = cJSON_GetObjectItem(ams_item, "id");
        const char *ams_id_str = cJSON_IsString(j_ams_id) ? j_ams_id->valuestring : "N/A";
        int ams_id_now = atoi(ams_id_str);
        if(filament_msg->ams_id != ams_id_now){
            ESP_LOGI(TAG,"Is ams %d,not this one,find next!",filament_msg->ams_id);
            continue;
        }
        // 获取 "tray" 数组
        cJSON *tray_array = cJSON_GetObjectItem(ams_item, "tray");
        if (!cJSON_IsArray(tray_array)) {
            ESP_LOGW(TAG,"Failed to get 'tray' array\n");
            continue;
        }
        // 遍历 "tray" 数组
        cJSON *tray_item = NULL;
        cJSON_ArrayForEach(tray_item, tray_array) {
            // 提取 "id"
            cJSON *id = cJSON_GetObjectItem(tray_item, "id");
            const char *id_str = cJSON_IsString(id) ? id->valuestring : "N/A";

            // 提取 "tray_color"
            cJSON *tray_color = cJSON_GetObjectItem(tray_item, "tray_color");
            const char *tray_color_str = cJSON_IsString(tray_color) ? tray_color->valuestring : "N/A";
    
            // 提取 "tray_type"
            cJSON *tray_type = cJSON_GetObjectItem(tray_item, "tray_type");
            const char *tray_type_str = cJSON_IsString(tray_type) ? tray_type->valuestring : "N/A";

            // 打印结果
            filament_msg->amd_id = atoi(id_str);
            filament_msg->sub_event_id = kEVENT_ID_FILAMENT;
            filament_msg->color = (uint32_t)strtoul(tray_color_str, NULL, 16);;
            strncpy(filament_msg->filament_type,tray_type_str,32);
            xQueueSend(xqueue_ams_msg,filament_msg,(TickType_t)10);
            ESP_LOGI(TAG,"Tray ID: %d, Tray Color: %ld, Tray Type: %s\n", filament_msg->amd_id, filament_msg->color, tray_type_str);
        }
    }
    return 0;
}

static int resolution_report_cali_get(char *json_str,size_t len,cJSON* print,filament_msg_t *filament_msg){
    //extrusion_cali_list_ptr = heap_caps_malloc(len+2, MALLOC_CAP_SPIRAM);
    memset(extrusion_cali_list_ptr,'\0',CALILIST_BUFF_SIZE);
    if(len >= CALILIST_BUFF_SIZE ){
        ESP_LOGE(TAG,"cali list size biger than buffer size! len:%d",len);
        return -1;
    }
    strlcpy(extrusion_cali_list_ptr,json_str,len+1);
    xQueueSend(xqueue_cali_get_msg,&extrusion_cali_list_ptr,(TickType_t)10);
    return 0;
}

static void resolution_report(char *json_str,size_t len,filament_msg_t *filament_msg)
{
    // 解析 JSON 字符串
    cJSON *root = cJSON_ParseWithLength(json_str,len);
    if (root == NULL) {
        ESP_LOGW(TAG,"Failed to parse JSON\n");
        return;
    }

    // 获取 "print" 对象
    cJSON *print = cJSON_GetObjectItem(root, "print");
    if (!cJSON_IsObject(print)) {
        ESP_LOGW(TAG,"Failed to get 'print' object\n");
        cJSON_Delete(root);
        return;
    }

    // 获取 "sequence_id"
    cJSON *sequence_id = cJSON_GetObjectItem(print, "sequence_id");
    if (cJSON_IsString(sequence_id)) {
        ESP_LOGD(TAG,"Sequence ID: %s\n", sequence_id->valuestring);
    }

    // 获取 "command" 对象
    cJSON *command = cJSON_GetObjectItem(print, "command");
    if (!cJSON_IsString(command)) {
        ESP_LOGW(TAG,"Failed to get 'command'\n");
        cJSON_Delete(root);
        return;
    }

    uint16_t resp_seq_id = (uint16_t)atoi(sequence_id->valuestring);

    // 2. 判断是否与当前飞行中消息的 sequence_id 匹配
    if (s_waiting_response && (resp_seq_id == s_current_msg_in_flight.sequence_id)) {
        // 表示这是我们等待的那条消息的应答
        ESP_LOGI(TAG,"Matched response for seq_id = %d\n", resp_seq_id);

        // 3. 释放当前消息资源
        free(s_current_msg_in_flight.payload);
        s_current_msg_in_flight.payload = NULL;
        s_current_msg_in_flight.length = 0;
        s_current_msg_in_flight.sequence_id = 0;
        // 清除等待标记
        s_waiting_response = false;
    }
    //s_waiting_response = false;
    if(strncmp(command->valuestring,"push_status",sizeof("push_status")) == 0 ){
        ESP_LOGD(TAG,"command: %s\n", "push_status");
        resolution_report_push_state(json_str,len,print,filament_msg);
    }
    else if(strncmp(command->valuestring,"extrusion_cali_get",sizeof("extrusion_cali_get")) == 0 ){
        ESP_LOGD(TAG,"command: %s\n", "extrusion_cali_get");
        resolution_report_cali_get(json_str,len,print,filament_msg);
    }
    else if(strncmp(command->valuestring,"extrusion_cali_sel",sizeof("extrusion_cali_sel")) == 0 ){
        ESP_LOGD(TAG,"command: %s %.*s", "extrusion_cali_sel",len,json_str);
    }
    // 清理 JSON 对象
    cJSON_Delete(root);
}

static void message_sender_task(void *pvParameters)
{
    while (1) {
        //获取发送消息
        if (!s_waiting_response) {
            if (xQueueReceive(xqueue_send_msg, &s_current_msg_in_flight, 0) == pdTRUE) {
                //获取MQTT连接状态
                if(s_mqtt_state != kSTATE_MQTT_CONNECTED){
                    ESP_LOGE(TAG,"MQTT not connected,skip send message");
                    vTaskDelay(pdMS_TO_TICKS(100));
                    continue;
                }
                esp_mqtt_client_publish(client, request_topic, s_current_msg_in_flight.payload, (int)s_current_msg_in_flight.length, 0, 0);

                ESP_LOGI(TAG,"Send message (seq_id=%d): %.*s\n",
                       s_current_msg_in_flight.sequence_id,
                       (int)s_current_msg_in_flight.length,
                       s_current_msg_in_flight.payload);
                s_waiting_response = true;
            }
        }
        else{
            if(s_current_msg_in_flight.timeout_cnt > 0 ){
                s_current_msg_in_flight.timeout_cnt--;
            }
            else{
                ESP_LOGE(TAG,"Send message (seq_id=%d) timeout",s_current_msg_in_flight.sequence_id);
                free(s_current_msg_in_flight.payload);
                s_current_msg_in_flight.payload = NULL;
                s_current_msg_in_flight.length = 0;
                s_current_msg_in_flight.sequence_id = 0;
                s_waiting_response = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int enqueue_print_message(char* _payload, size_t _len,uint16_t _sequence_id)
{
    if (!xqueue_send_msg) {
       ESP_LOGE(TAG,"Message queue not initialized!\n");
        return false;
    }
    message_t msg;
    msg.length = _len + 1;
    msg.sequence_id = _sequence_id;
    msg.payload = heap_caps_malloc(_len+2, MALLOC_CAP_SPIRAM);
    msg.timeout_cnt = 100;
    if(msg.payload  == NULL ){
        ESP_LOGW(TAG,"Failed to malloc msg.payload len:%d",_len+2);
    }
    memset(msg.payload,0,_len+2);
    strlcpy(msg.payload,_payload,_len+1);

    if (xQueueSend(xqueue_send_msg, &msg, 0) == pdTRUE) {
        ESP_LOGI(TAG,"Message (seq_id=%d) enqueued.\n", msg.sequence_id);
        return ESP_OK;
    } else {
        ESP_LOGE(TAG,"Queue is full, cannot enqueue message!\n");
        free(msg.payload);
        return ESP_ERR_NO_MEM;
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    filament_msg_t filament_msg;
    filament_msg.ams_id = _g_ams_id;
    int msg_id = -1;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, report_topic, 1);
        ESP_LOGI(TAG, "Sent subscribe successful, msg_id=%d", msg_id);
        filament_msg.event_id = event_id;
        filament_msg.msg_id = msg_id;
        xQueueSend(xqueue_ams_msg,&filament_msg,(TickType_t)10);
        s_mqtt_state = kSTATE_MQTT_CONNECTED;
        mqtt_send_filament_setting();
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        filament_msg.event_id = event_id;
        filament_msg.msg_id = msg_id;
        xQueueSend(xqueue_ams_msg,&filament_msg,(TickType_t)10);
        s_mqtt_state = kSTATE_MQTT_DISCONNECTED;
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        //msg_id = esp_mqtt_client_publish(client, request_topic, push_all_payload, 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGD(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGD(TAG, "MQTT_EVENT_DATA");
        filament_msg.event_id = event_id;
        filament_msg.msg_id = msg_id;

        ESP_LOGD(TAG,"topic_len = %d, data_len = %d, data_offset = %d,data=%.*s",event->total_data_len,event->data_len, event->current_data_offset, event->data_len, event->data);

        //由于MQTT接收缓冲区较小，将数据分段接收到rec_msg_struct中
        if( event->total_data_len <= MQTT_RX_TOPIC_SIZE ){
            resolution_report(event->data,event->data_len,&filament_msg);
        }
        else if(event->total_data_len > MQTT_RX_BUFF_SIZE){
            ESP_LOGE(TAG,"total_data_len > MQTT_RX_BUFF_SIZE");
        }
        else if((event->total_data_len < MQTT_RX_BUFF_SIZE)&&(event->total_data_len > MQTT_RX_TOPIC_SIZE)){
            if( event->current_data_offset == 0 ){
                memset(rec_msg_struct.payload,'\0',MQTT_RX_BUFF_SIZE);
                rec_msg_struct.topic_len = event->topic_len;
                rec_msg_struct.data_len = 0;
            }

            if( rec_msg_struct.data_len < event->total_data_len) {
                memcpy(&rec_msg_struct.payload[event->current_data_offset],event->data,event->data_len);
                rec_msg_struct.data_len += event->data_len;
            }

            if(rec_msg_struct.data_len >= event->total_data_len){
                ESP_LOGD(TAG,"receive complete message DATA=%.*s",rec_msg_struct.data_len,rec_msg_struct.payload);
                resolution_report(rec_msg_struct.payload,rec_msg_struct.data_len,&filament_msg);
            }
        }  
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            ESP_LOGI(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
            ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
        } else {
            ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }

}

int mqtt_send_filament_setting(){
    uint16_t seq_id = (uint16_t)(esp_random() & 0xFFFF);
    char payload[150] = {0};
    int len = sprintf(payload,"{\"pushing\": {\"sequence_id\": \"%d\",\"command\": \"pushall\",\"version\": 1,\"push_target\": 1}}",seq_id);
    return enqueue_print_message(payload,len,seq_id);
    //return esp_mqtt_client_publish(client, request_topic, payload, 0, 0, 0);
}

int mqtt_send_get_cali_list(){
    uint16_t seq_id = (uint16_t)(esp_random() & 0xFFFF);
    char payload[150] = {0};
    int len = sprintf(payload,
            "{\"print\": {"
            "    \"sequence_id\":\"%d\","
            "    \"command\": \"extrusion_cali_get\","
            "    \"filament_id\": \"\","
            "    \"nozzle_diameter\": \"0.4\""
            "}}",seq_id);

    return enqueue_print_message(payload,len,seq_id);
    //ESP_LOGI(TAG,"Get cali list topic %s,payload=%s",request_topic,cali_get_payload);
    //return esp_mqtt_client_publish(client, request_topic, cali_get_payload, 0, 0, 0);
}

char device_ip[32];
char device_password[32];

void init_mqtt(char* printer_ip,char* printer_password,char* printer_device_id)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    strlcpy(device_ip,printer_ip,sizeof(device_ip));
    strlcpy(device_password,printer_password,sizeof(device_password));

    xqueue_ams_msg = xQueueCreate(16,sizeof(filament_msg_t));
    xqueue_cali_get_msg = xQueueCreate(2,sizeof(char *));
    xqueue_send_msg = xQueueCreate(16, sizeof(message_t));
    if (xqueue_send_msg == NULL) {
        ESP_LOGE(TAG,"Failed to create message queue!");
        return;
    }

    ESP_LOGI(TAG,"certificate \n%s",(const char *)mqtt_eclipseprojects_io_pem_start);
    ESP_LOGI(TAG,"Device IP: %s",printer_ip);
    ESP_LOGI(TAG,"Device ID: %s",printer_device_id);
    ESP_LOGI(TAG,"Device PWD: %s",printer_password);

    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            //.address.uri = "mqtts://10.0.0.76:8883",
            .address.hostname = device_ip,
            .address.port = 8883,
            //.verification.use_global_ca_store = false,
            .verification.skip_cert_common_name_check = true,
            .address.transport = MQTT_TRANSPORT_OVER_SSL,
            .verification.certificate = (const char *)mqtt_eclipseprojects_io_pem_start,
            //.verification.certificate_len = strlen((const char *)mqtt_eclipseprojects_io_pem_start)
        },
        .credentials = {
            .username = "bblp",
            .client_id = "amsplus_client_",
            .authentication.password = device_password,
        },
        .buffer = {
            .size = MQTT_RX_TOPIC_SIZE,
        }
    };

    //xTaskCreate(message_sender_task, "message_sender_task", 4 * 1024, NULL, 5, NULL);

    sprintf(report_topic,"device/%s/report",printer_device_id);
    sprintf(request_topic,"device/%s/request",printer_device_id);

    ESP_LOGI(TAG,"TOPIC:%s",report_topic);
    ESP_LOGI(TAG,"TOPIC:%s",request_topic);

    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    extrusion_cali_list_ptr = heap_caps_malloc(sizeof(char)*CALILIST_BUFF_SIZE, MALLOC_CAP_SPIRAM);
    if(extrusion_cali_list_ptr == NULL ){
        ESP_LOGW(TAG,"Failed to malloc cali_get_list_ptr");
    }

    rec_msg_struct.payload = heap_caps_malloc(sizeof(char)*MQTT_RX_BUFF_SIZE, MALLOC_CAP_SPIRAM);
    if(rec_msg_struct.payload == NULL ){
        ESP_LOGE(TAG,"Failed to malloc rec_payload_ptr");
    }
    rec_msg_struct.topic_len = 0; 
    rec_msg_struct.data_len = 0;
    rec_msg_struct.data_offset = 0;

    xTaskCreate(message_sender_task, "message_sender_task", 4 * 1024, NULL, 5, NULL);
    ESP_LOGI(TAG, "Sent msg task creat successful");
}

void bambu_mqtt_disconnect(){
    esp_mqtt_client_disconnect(client);
}
