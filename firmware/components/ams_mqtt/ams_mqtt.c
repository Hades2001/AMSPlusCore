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

static char *payload = "{\"pushing\": {\"sequence_id\": \"0\",\"command\": \"pushall\",\"version\": 1,\"push_target\": 1}}";
QueueHandle_t xqueue_ams_msg;
esp_mqtt_client_handle_t client;


static void resolution_report(char *json_str,filament_msg_t *filament_msg, int ams_id)
{

    filament_msg->amd_id = -1;

    // 解析 JSON 字符串
    cJSON *root = cJSON_Parse(json_str);
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

    // 获取 "ams" 对象
    cJSON *ams = cJSON_GetObjectItem(print, "ams");
    if (!cJSON_IsObject(ams)) {
        ESP_LOGW(TAG,"Failed to get 'ams' object\n");
        cJSON_Delete(root);
        return;
    }

    // 获取 "ams" 数组
    cJSON *ams_array = cJSON_GetObjectItem(ams, "ams");
    if (!cJSON_IsArray(ams_array)) {
        ESP_LOGW(TAG,"Failed to get 'ams' array\n");
        cJSON_Delete(root);
        return;
    }

    // 遍历 "ams" 数组
    cJSON *ams_item = NULL;
    cJSON_ArrayForEach(ams_item, ams_array) {
        
        cJSON *j_ams_id = cJSON_GetObjectItem(ams_item, "id");
        const char *ams_id_str = cJSON_IsString(j_ams_id) ? j_ams_id->valuestring : "N/A";
        filament_msg->ams_id = atoi(ams_id_str);
        if(filament_msg->ams_id != ams_id){
            ESP_LOGI(TAG,"Is ams %d,not this one,find next!",filament_msg->ams_id);
            continue;
        }
        // 获取 "tray" 数组
        cJSON *tray_array = cJSON_GetObjectItem(ams_item, "tray");
        if (!cJSON_IsArray(tray_array)) {
            ESP_LOGW(TAG,"Failed to get 'tray' array\n");
            //printf("Failed to get 'tray' array\n");
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
            filament_msg->color = (uint32_t)strtoul(tray_color_str, NULL, 16);;
            strncpy(filament_msg->filament_type,tray_type_str,32);
            xQueueSend(xqueue_ams_msg,filament_msg,(TickType_t)10);
            ESP_LOGI(TAG,"Tray ID: %d, Tray Color: %ld, Tray Type: %s\n", filament_msg->amd_id, filament_msg->color, tray_type_str);
        }
    }

    // 获取 "sequence_id"
    cJSON *sequence_id = cJSON_GetObjectItem(print, "sequence_id");
    if (cJSON_IsString(sequence_id)) {
        ESP_LOGI(TAG,"Sequence ID: %s\n", sequence_id->valuestring);
    }

    // 清理 JSON 对象
    cJSON_Delete(root);
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
    int msg_id = -1;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, report_topic, 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        filament_msg.event_id = event_id;
        filament_msg.msg_id = msg_id;
        xQueueSend(xqueue_ams_msg,&filament_msg,(TickType_t)10);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        filament_msg.event_id = event_id;
        filament_msg.msg_id = msg_id;
        xQueueSend(xqueue_ams_msg,&filament_msg,(TickType_t)10);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, request_topic, payload, 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        filament_msg.event_id = event_id;
        filament_msg.msg_id = msg_id;
        //printf("TOPIC=%.*s topic_len = %d\r\n", event->topic_len, event->topic, event->total_data_len);
        ESP_LOGD(TAG,"topic_len = %d,DATA=%.*s\r\n",event->total_data_len,event->data_len, event->data);
        //printf("DATA=%.*s\r\n", event->data_len, event->data);
        resolution_report(event->data,&filament_msg,_g_ams_id);
        
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

int mqtt_send_filament_setting(char* payload){
    return esp_mqtt_client_publish(client, request_topic, payload, 0, 0, 0);
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
    ESP_LOGI(TAG,"certificate \n%s",(const char *)mqtt_eclipseprojects_io_pem_start);
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            //.address.uri = "mqtts://10.0.0.76:8883",
            .address.hostname = device_ip,
            .address.port = 8883,
            .verification.skip_cert_common_name_check = true,
            .address.transport = MQTT_TRANSPORT_OVER_SSL,
            .verification.certificate = (const char *)mqtt_eclipseprojects_io_pem_start
        },
        .credentials = {
            .username = "bblp",
            .client_id = "OpenSpool_Client",
            .authentication.password = device_password
        },
        .buffer = {
            .size = 16 * 1024,
        }
    };

    sprintf(report_topic,"device/%s/report",printer_device_id);
    sprintf(request_topic,"device/%s/request",printer_device_id);

    ESP_LOGI(TAG,"TOPIC:%s",report_topic);
    ESP_LOGI(TAG,"TOPIC:%s",request_topic);

    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}
