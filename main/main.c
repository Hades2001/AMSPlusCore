#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
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
extern QueueHandle_t xqueue_ams_msg; 

enum{
    kSYS_IDLE = 0,
    kSYS_CONFIG,
    kSYS_CONNECT_WFIF,
    kSYS_CONNECT_MQTT,
    kSYS_RUNNING,
    kSYS_REBOOTING,
    kSYS_ERROR,
    kSYS_DEFAULE,
};

int sysstate = kSYS_IDLE;

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
        vTaskDelay(1000);
        generate_softap_ssid(ssid,sizeof(ssid));
        sprintf(formatbuff,"Wi-Fi SSID:%s",ssid);
        lv_label_set_text(ams_ui.screen_00_lab_desctext, formatbuff);
        lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_00,true);
        httpd_handle_t server = start_webserver();
        if (server) {
            ESP_LOGI(TAG, "HTTP server started successfully");
        } else {
            ESP_LOGE(TAG, "Failed to start HTTP server");
        }
        sysstate = kSYS_CONFIG;
        while(1){
            if(xQueueReceive(xqueue_http_msg,&device_config,500) == pdTRUE){
                save_config_to_nvs(&device_config);
                ESP_LOGI(TAG,"");
                vTaskDelay(2000);
                esp_restart();
            }
            vTaskDelay(500);
        }
    }
    else {
        sprintf(formatbuff,"ssid:%s",device_config.ssid);
        lv_label_set_text(ams_ui.screen_21_lab_SSID, formatbuff);
        wifi_init_sta(device_config.ssid,device_config.password);

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
        connect_mqtt(&device_config);
        lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_31,true);
    }
}

void parse_ntag(ndef_record **records){
    ndef_record *current = *records;
    while (current) {
        char *command = parse_command((const char*)current->payload,(size_t)current->payload_length);
        if(command !=NULL){
            ESP_LOGI(TAG, "NTAG CMD:%s",command);
            if (strcmp(command, "reconfig") == 0){
                reconfigAMSPlus();
            }
            else if(strcmp(command, "filament") == 0){

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

    uint8_t scanner_no = 1;

    if (scanner == scanner_2) {
        scanner_no = 2;
    }

    if (picc->state == RC522_PICC_STATE_ACTIVE) {
        ESP_LOGI(TAG, "Card detected on the scanner #%d", scanner_no);
        rc522_picc_print(picc);

        uint8_t *dataptr;
        ndef_record *records;
        ntag_read_ndef(scanner, picc, &dataptr, &records);
        if(records != NULL){
            parse_ntag(&records);
        }
        free_ndef_records(records);
        free(dataptr);
    }
    else if (picc->state == RC522_PICC_STATE_IDLE && event->old_state >= RC522_PICC_STATE_ACTIVE) {
        ESP_LOGI(TAG, "Card has been removed from the scanner #%d", scanner_no);
    }
}

void init_ntag()
{
    // Register events for each scanner
    rc522_register_events(scanner_1, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_1);
    rc522_register_events(scanner_2, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_2);

    // Start scanners
        rc522_start(scanner_1);
    rc522_start(scanner_2);
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
    printf("Fine qmsd!\r\n");

    lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_11,true);
    init_ntag();
    init_aht10(BOARD_I2C_SCL_PIN,BOARD_I2C_SDA_PIN);
    vTaskDelay(500);

    lv_obj_set_tile(ams_ui.screen_5_tileview_1,ams_ui.screen_21,true);
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

    lv_obj_t *pagelist[5] = {
        ams_ui.screen_00,
        ams_ui.screen_01,
        ams_ui.screen_11,
        ams_ui.screen_21,
        ams_ui.screen_31,
    };
    int cnt = 200;
    filament_msg_t filament_msg;

    lv_anim_set_values(&ams_ui.screen_31_anim, 0, -71);
    lv_anim_start(&ams_ui.screen_31_anim);

    while(1){
        if(get_aht10_data(&temp,&hum) == 0 ){
            memset(tempstr,0,sizeof(tempstr));
            sprintf(tempstr,"%.1f°C",temp);
            lv_label_set_text(ams_ui.screen_31_lab_temp,tempstr);
        
            memset(tempstr,0,sizeof(tempstr));
            sprintf(tempstr,"%d%%",((uint16_t)(hum*100))%100);
            lv_label_set_text(ams_ui.screen_31_lab_hum, tempstr);
            lv_bar_set_value(ams_ui.screen_31_bar_hum, ((uint16_t)(hum*100))%100, LV_ANIM_OFF);
        }
        if(xQueueReceive( xqueue_ams_msg,&filament_msg,10) == pdTRUE){
            filament_msg.amd_id = (filament_msg.amd_id > 3 ) ? 3 : filament_msg.amd_id;
            lv_label_set_text(filament_textlist[filament_msg.amd_id], filament_msg.filament_type);
            lv_obj_set_style_bg_color(filament_boxlist[filament_msg.amd_id], lv_color_hex(filament_msg.color>>8), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        cnt ++;
        if(cnt >= 200 ){
            qmsd_debug_heap_print(MALLOC_CAP_INTERNAL,0);
            qmsd_debug_heap_print(MALLOC_CAP_SPIRAM,0);
            qmsd_debug_task_print(0);
            cnt = 0;
        }
        vTaskDelay(300);
    }
}
