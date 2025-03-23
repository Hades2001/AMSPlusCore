#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_app_desc.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "gui_guider.h"

#include "aht10.h"
#include "ams_ntag.h"
#include "ams_mqtt.h"
#include "easy_wifi.h"
#include "picc/rc522_ntag.h"

#define TAG "QMSD-MAIN"

lv_ui ams_ui;

enum{
    kSYS_IDLE= 0,
    kSYS_CONNECT_WFIF,
    kSYS_CONNECT_MQTT,
    kSYS_GATFIRST,
    kSYS_RUNNING,
    kSYS_CONFIG,
    kSYS_REBOOTING,
    kSYS_ERROR,
    kSYS_OTA,
    kSYS_DEFAULE,
};

enum{
    kFLAG_CONNECT_WIFI      = 0x0001,
    kFLAG_CONNECTED_MQTT    = 0x0002,
    kFLAG_FLUSHAMS          = 0x0004,
};

int sysstate = kSYS_IDLE;
int sysflag = 0;
int _g_ams_id = 0;

void reconfigAMSPlus(){
    http_config_t device_config;
    memset(&device_config,0,sizeof(http_config_t));
    save_config_to_nvs(&device_config);
    ESP_LOGI(TAG, "Reconfig command received. Restarting AMSPlus...");
    vTaskDelay(1000);
    esp_restart();
    while (1)
    {
        vTaskDelay(1000);
    }
}

/**
 * @brief Wi-Fi 事件回调
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_01,true);
    }
}

void connect_mqtt(http_config_t *device_config){
    init_mqtt(device_config->printer_ip,device_config->printer_password,device_config->printer_device_id);
}

void connect_to_wifi(){

    char ssid[32] = {0};
    char formatbuff[64] = {0};

    http_config_t device_config;
    esp_err_t ret = load_config_from_nvs(&device_config);

    if ((ret != ESP_OK) || strlen(device_config.ssid) == 0 || strlen(device_config.password) == 0) {
        
        wifi_init_softap(&wifi_event_handler);
        generate_softap_ssid(ssid,sizeof(ssid));
        sprintf(formatbuff,"Wi-Fi SSID:%s",ssid);
        lv_label_set_text(ams_ui.screen_00_lab_desctext, formatbuff);
        lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_00,true);
        httpd_handle_t server = start_config_webserver();
        if (server) {
            ESP_LOGI(TAG, "HTTP server started successfully");
        } else {
            ESP_LOGE(TAG, "Failed to start HTTP server");
        }
        sysstate = kSYS_CONFIG;
        while(1){
            if(xQueueReceive(xqueue_http_msg,&device_config,500) == pdTRUE){
                if(device_config.type != kHTTPMSG_DEVICE_SET )continue;
                save_config_to_nvs(&device_config);
                ESP_LOGI(TAG,"");
                vTaskDelay(2000);
                esp_restart();
            }
            vTaskDelay(500);
        }
    }
    else {
        _g_ams_id = atoi(device_config.ams_id);
        sprintf(formatbuff,"ssid:%s,ams %d",device_config.ssid,_g_ams_id);
        lv_label_set_text(ams_ui.screen_21_lab_SSID, formatbuff);
        wifi_init_sta(device_config.ssid,device_config.password);
        sysstate = kSYS_CONNECT_WFIF;
        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
    
        /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
        * happened. */
        if (bits & WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                    device_config.ssid, device_config.printer_password);
        } else if (bits & WIFI_FAIL_BIT) {
            ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                    device_config.ssid, device_config.printer_password);
            lv_label_set_text(ams_ui.screen_21_lab_SSID, "Failed to connect, Reconfigure device");
            reconfigAMSPlus();
        } else {
            ESP_LOGE(TAG, "UNEXPECTED EVENT");
            lv_label_set_text(ams_ui.screen_21_lab_SSID, "Failed to connect, Reconfigure device");
            reconfigAMSPlus();
        }

        //sprintf(formatbuff,"",temp);
        lv_label_set_text(ams_ui.screen_31_lab_info,get_device_ip());
        //lv_label_set_text(ams_ui.screen_21_lab_SSID, "Sync sntp time");
        start_gen_webserver();
        //initialize_sntp();
        //obtain_time();
        connect_mqtt(&device_config);
        lv_label_set_text(ams_ui.screen_21_lab_SSID, "Connecting to print mqtt");
        sysstate = kSYS_CONNECT_MQTT;
        
    }
}

void parse_ntag(ndef_record **records,int scanner_id,int amsid){
    ndef_record *current = *records;
    while (current) {
        char *command = parse_command((const char*)current->payload,(size_t)current->payload_length);
        if(command !=NULL){
            ESP_LOGI(TAG, "NTAG CMD:%s",command);
            if (strcmp(command, "reconfig") == 0){
                reconfigAMSPlus();
            }
            else if(strcmp(command, "filament") == 0){
                filament_info info;
                parse_payload((const char*)current->payload,(size_t)current->payload_length,&info);
                char jsonbuff[256] = {0};
                if(sysstate == kSYS_RUNNING){
                    int id = filament_setting_payload(jsonbuff,sizeof(jsonbuff),&info,scanner_id,amsid);
                    ESP_LOGI(TAG,"Payload JSON:%s",jsonbuff);
                    enqueue_print_message(jsonbuff,strlen(jsonbuff),id);
                    if(info.cali_idx >= 0 ){
                        id = cali_idx_setting_payload(jsonbuff,sizeof(jsonbuff),&info,scanner_id,amsid);
                        ESP_LOGI(TAG,"Payload JSON:%s",jsonbuff);
                        enqueue_print_message(jsonbuff,strlen(jsonbuff),id);
                    }
                    filament_msg_t filament_msg;
                    filament_msg.event_id = MQTT_USER_EVENT;
                    filament_msg.msg_id = 0;
                    xQueueSend(xqueue_ams_msg,&filament_msg,(TickType_t)10);
                }
            }
            free(command);
        }
        current = current->next;
    }
}

uint8_t read_ntag_buff[32];

static void on_picc_state_changed(void *arg, esp_event_base_t base, int32_t event_id, void *data)
{
    rc522_handle_t scanner = (rc522_handle_t)arg;
    rc522_picc_state_changed_event_t *event = (rc522_picc_state_changed_event_t *)data;
    rc522_picc_t *picc = event->picc;

    uint8_t scanner_no = (scanner == scanner_2) ? 2 : 1;

    if (picc->state == RC522_PICC_STATE_ACTIVE) {
        ESP_LOGI(TAG, "Card detected on the scanner #%d", scanner_no);
        rc522_picc_print(picc);

        uint8_t *dataptr = NULL;
        ndef_record *records = NULL;
        ntag_read_ndef(scanner, picc, &dataptr, &records);
        if(records != NULL){
            parse_ntag(&records,scanner_no,_g_ams_id);
            free_ndef_records(records);
        }
        if(dataptr != NULL ){
            free(dataptr);
        }
    }
    else if (picc->state == RC522_PICC_STATE_IDLE && event->old_state >= RC522_PICC_STATE_ACTIVE) {
        ESP_LOGI(TAG, "Card has been removed from the scanner #%d", scanner_no);
    }
}

int init_ntag()
{
    // Register events for each scanner
    rc522_register_events(scanner_1, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_1);
    rc522_register_events(scanner_2, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_2);

    // Start scanners
    if(rc522_start(scanner_1) != ESP_OK ){
        return ESP_FAIL;
    }
    if(rc522_start(scanner_2) != ESP_OK ){
        return ESP_FAIL;
    }
    return ESP_OK;
}

void gui_user_init() {

    setup_ui(&ams_ui);
    lv_scr_load(ams_ui.screen_5);
}

void app_main(void) {
    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
    config.gui.buffer_size = 240 * 135 * 2;
    config.gui.en = true;
    config.gui.flags.double_fb = 0;
    config.gui.flags.fb_in_psram = 0;
    config.gui.refresh_task.en = 0;
    config.board_dir = BOARD_ROTATION_90;
    config.touch.en = false;
    qmsd_board_init(&config);

    const esp_app_desc_t* app_desc = esp_app_get_description();
    ESP_LOGI(TAG,"Firmware Version %s",app_desc->version);

    lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_11,true);
    vTaskDelay(200);
    lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_21,true);
    esp_err_t ntag_res,ath10_res;
    bool isError = false;
    do
    {
        ntag_res = init_ntag();
        if((isError == false) && (ntag_res != ESP_OK)){
            ESP_LOGE(TAG,"Failed to initialize RC522");
            lv_obj_add_flag(ams_ui.screen_21_img_1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_text_color(ams_ui.screen_21_lab_SSID,lv_color_make(0xff, 0x00, 0x00),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_label_set_text(ams_ui.screen_21_lab_SSID, "Failed to initialize RC522");
            isError = true;
        }
        else if((isError == true) && (ntag_res != ESP_OK)){
            ESP_LOGE(TAG,"Re - initialize RC522");
            vTaskDelay(1000);
        }
    } while (isError);
    isError = false;
    do
    {
        ath10_res = init_aht10(BOARD_I2C_SCL_PIN,BOARD_I2C_SDA_PIN);
        if((isError == false) && (ath10_res != ESP_OK)){
            ESP_LOGE(TAG,"Failed to initialize ATH10");
            lv_obj_add_flag(ams_ui.screen_21_img_1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_text_color(ams_ui.screen_21_lab_SSID,lv_color_make(0xff, 0x00, 0x00),LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_label_set_text(ams_ui.screen_21_lab_SSID, "Failed to initialize ATH10");
            isError = true;
        }
        else if((isError == true) && (ath10_res != ESP_OK)){
            ESP_LOGE(TAG,"Re - initialize ATH10");
            vTaskDelay(1000);
        }
    } while (isError);

    lv_obj_set_style_text_color(ams_ui.screen_21_lab_SSID,lv_color_make(0xff, 0xff, 0xff),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_clear_flag(ams_ui.screen_21_img_1, LV_OBJ_FLAG_HIDDEN);
    
    connect_to_wifi();

    float temp,hum;
    char tempstr[10];

    lv_obj_t *filament_boxlist[4] = {
        ams_ui.screen_31_box_filament_1,
        ams_ui.screen_31_box_filament_2,
        ams_ui.screen_31_box_filament_3,
        ams_ui.screen_31_box_filament_4,
    };
    lv_obj_t *filament_textlist[4] = {
        ams_ui.screen_31_lab_filament_1,
        ams_ui.screen_31_lab_filament_2,
        ams_ui.screen_31_lab_filament_3,
        ams_ui.screen_31_lab_filament_4,
    };

    int cnt = 200;
    int flush_ams_cnt = 19;
    int ota_s_ota_progress = -1;
    bool is_flushing_filament = true;
    char *extrusion_cali_json = NULL;
    httpmsg_type_t httpmsg;
    filament_msg_t filament_msg;

    while(1){
        if(xQueueReceive( xqueue_ams_msg,&filament_msg,10) == pdTRUE){
            switch (filament_msg.event_id)
            {
            case MQTT_EVENT_CONNECTED:
                lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_31,true);
                //mqtt_send_filament_setting();
                sysstate = kSYS_RUNNING;
                break;
            case MQTT_EVENT_DATA:
                if(filament_msg.amd_id >= 0 ){
                    filament_msg.amd_id = (filament_msg.amd_id > 3 ) ? 3 : filament_msg.amd_id;
                    lv_label_set_text(filament_textlist[filament_msg.amd_id], filament_msg.filament_type);
                    lv_obj_set_style_bg_color(filament_boxlist[filament_msg.amd_id], lv_color_hex(filament_msg.color>>8), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(filament_boxlist[filament_msg.amd_id], lv_color_hex(filament_msg.color>>8), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    //flush_ams_cnt = (is_flushing_filament) ? 19 : 20;
                }
                break;
            case MQTT_EVENT_DISCONNECTED:
                if(sysstate == kSYS_RUNNING ){
                    lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_21,true);
                }
                sysstate = kSYS_CONNECT_MQTT;
                break;
            case MQTT_USER_EVENT:
                flush_ams_cnt = (is_flushing_filament) ? 19 : 20;
                break;
            default:
                break;
            }
        }
        if(sysstate == kSYS_RUNNING ){
            if(get_aht10_data(&temp,&hum) == 0 ){
                memset(tempstr,0,sizeof(tempstr));
                sprintf(tempstr,"%.1f°C",temp);
                lv_label_set_text(ams_ui.screen_31_lab_temp,tempstr);
            
                memset(tempstr,0,sizeof(tempstr));
                sprintf(tempstr,"%d%%",((uint16_t)(hum*100))%100);
                lv_label_set_text(ams_ui.screen_31_lab_hum, tempstr);
                //lv_bar_set_value(ams_ui.screen_31_bar_hum, ((uint16_t)(hum*100))%100, LV_ANIM_OFF);
            }

            if( flush_ams_cnt == 0 ){
                lv_anim_set_values(&ams_ui.screen_31_anim, 0, -71);
                lv_anim_start(&ams_ui.screen_31_anim);
                is_flushing_filament = false;
            }else if(flush_ams_cnt == 20 ){
                lv_anim_set_values(&ams_ui.screen_31_anim, -71, 0);
                lv_anim_start(&ams_ui.screen_31_anim);
                is_flushing_filament = true;
            }
            if( flush_ams_cnt > -1){
                flush_ams_cnt --;
            }
        }

        if(xQueueReceive(xqueue_ota_msg,&ota_s_ota_progress,5) == pdTRUE){
            if(ota_s_ota_progress >= 0){
                lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_41,true);
                lv_bar_set_value(ams_ui.screen_41_bar_ota, ota_s_ota_progress, LV_ANIM_OFF);
            }
        }
        if(xQueueReceive(xqueue_get_calilist_msg,&httpmsg,5) == pdTRUE){
            if(sysstate != kSYS_RUNNING ){
                xQueueSend(xqueue_calilist_json_msg,&extrusion_cali_json,10);
            }
            else{
                if(httpmsg == kHTTPMSG_CALI_GET ){
                    mqtt_send_get_cali_list();
                }else if(httpmsg == kHTTPMSG_INTO_OTA ){
                    bambu_mqtt_disconnect();
                    sysstate = kSYS_OTA;
                }
            }
        }
        if(xQueueReceive(xqueue_cali_get_msg,&extrusion_cali_json,5) == pdTRUE){
            xQueueSend(xqueue_calilist_json_msg,&extrusion_cali_json,10);
        }

        cnt ++;
        if(cnt >= 200 ){
            qmsd_debug_heap_print(MALLOC_CAP_INTERNAL,0);
            qmsd_debug_heap_print(MALLOC_CAP_SPIRAM,0);
            cnt = 0;
        }
        vTaskDelay(300);
    }
}
