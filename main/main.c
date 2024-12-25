#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "aht10.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "ams_mqtt.h"

#define TAG "QMSD-MAIN"

lv_ui ams_ui;
extern QueueHandle_t xqueue_ams_msg; 

void gui_user_init() {

    setup_ui(&ams_ui);
    lv_scr_load(ams_ui.screen_5);
    //lv_scr_load(ams_ui.screen);
}

#define EXAMPLE_ESP_WIFI_SSID      "LinkWIFI"
#define EXAMPLE_ESP_WIFI_PASS      "stc89c52"
#define EXAMPLE_ESP_MAXIMUM_RETRY  10

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#if CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
#define EXAMPLE_H2E_IDENTIFIER ""
#elif CONFIG_ESP_WPA3_SAE_PWE_HASH_TO_ELEMENT
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#elif CONFIG_ESP_WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#endif
#if CONFIG_ESP_WIFI_AUTH_OPEN
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif

static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_HUNT_AND_PECK,
            .sae_h2e_identifier = "",
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
        init_mqtt();
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

#include "rc522.h"
#include "driver/rc522_spi.h"
#include "rc522_picc.h"
#include "picc/rc522_ntag.h"


static rc522_driver_handle_t driver_1;
static rc522_driver_handle_t driver_2;

// Scanners

static rc522_handle_t scanner_1;
static rc522_handle_t scanner_2;

static rc522_spi_config_t scanner_1_config = {
    .host_id = SPI3_HOST,
    .bus_config = &(spi_bus_config_t){
        .miso_io_num = RC522_SPI_MISO,
        .mosi_io_num = RC522_SPI_MOSI,
        .sclk_io_num = RC522_SPI_SCLK,
    },
    .dev_config = {
        .spics_io_num = RC522_SPI_CS1,
    },
    .rst_io_num = -1, // soft-reset
};

// Second scanner does not need bus configuration,
// since first scanner will initialize the bus.

static rc522_spi_config_t scanner_2_config = {
    .host_id = SPI3_HOST,
    .dev_config = {
        .spics_io_num = RC522_SPI_CS2,
    },
    .rst_io_num = -1, // soft-reset
};

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
        rc522_ntag_readn(scanner,picc,16,read_ntag_buff,32);
    }
    else if (picc->state == RC522_PICC_STATE_IDLE && event->old_state >= RC522_PICC_STATE_ACTIVE) {
        ESP_LOGI(TAG, "Card has been removed from the scanner #%d", scanner_no);
    }
}

void init_ntag()
{
    rc522_spi_create(&scanner_1_config, &driver_1);
    rc522_spi_create(&scanner_2_config, &driver_2);

    rc522_driver_install(driver_1);
    rc522_driver_install(driver_2);

    // Create scanners

    rc522_create(
        &(rc522_config_t) {
            .driver = driver_1,
        },
        &scanner_1);

    rc522_create(
        &(rc522_config_t) {
            .driver = driver_2,
        },
        &scanner_2);

    // Register events for each scanner

    rc522_register_events(scanner_1, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_1);
    rc522_register_events(scanner_2, RC522_EVENT_PICC_STATE_CHANGED, on_picc_state_changed, scanner_2);

    // Start scanners

    rc522_start(scanner_1);
    rc522_start(scanner_2);
}

void anim_y_callback(void * obj, int32_t value) {
    
    lv_obj_set_y((lv_obj_t *)obj, value);  // 设置垂直位置
    lv_obj_set_pos((lv_obj_t *)obj, 0, value);
}

void app_main(void) {
    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
    config.gui.en = true;
    config.board_dir = BOARD_ROTATION_90;
    config.touch.en = false;
    qmsd_board_init(&config);
    printf("Fine qmsd!\r\n");


    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    //ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_loop_create_default());
    //nvs_open();

    //wifi_init_sta();

    init_ntag();

    init_aht10(BOARD_I2C_SCL_PIN,BOARD_I2C_SDA_PIN);
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
    int cnt = 0;

    filament_msg_t filament_msg;

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, ams_ui.screen_5_cont_1);
    lv_anim_set_values(&anim, 0, -71);  // 从当前位置滚动到 x = 150
    lv_anim_set_time(&anim, 200);                          // 动画持续时间 1000 ms
    lv_anim_set_exec_cb(&anim, anim_y_callback);            // 水平方向动画
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);

    while(1){
        if(get_aht10_data(&temp,&hum) == 0 ){
            memset(tempstr,'\0',sizeof(tempstr));
            sprintf(tempstr,"%.1f°C",temp);
            lv_label_set_text(ams_ui.screen_31_lab_temp,tempstr);
        
            memset(tempstr,'\0',sizeof(tempstr));
            sprintf(tempstr,"%d%%",((uint16_t)(hum*100))%100);
            lv_label_set_text(ams_ui.screen_31_lab_hum, tempstr);
            lv_bar_set_value(ams_ui.screen_31_bar_hum, ((uint16_t)(hum*100))%100, LV_ANIM_OFF);
        }
        //if(xQueueReceive( xqueue_ams_msg,&filament_msg,portMAX_DELAY) == pdTRUE){
        //    filament_msg.amd_id = (filament_msg.amd_id > 3 ) ? 3 : filament_msg.amd_id;
        //    lv_label_set_text(filament_textlist[filament_msg.amd_id], filament_msg.filament_type);
        //    lv_obj_set_style_bg_color(filament_boxlist[filament_msg.amd_id], lv_color_hex(filament_msg.color>>8), LV_PART_MAIN|LV_STATE_DEFAULT);
        //}
        if(cnt < 5 ){
            lv_obj_set_tile(ams_ui.screen_5_tileview_1,pagelist[cnt],true);
            lv_obj_set_pos(ams_ui.screen_5_cont_1, 0, 0);
        }
        else if((cnt <= 10 )&&(cnt >= 5)){
            //lv_obj_set_pos(ams_ui.screen_5_cont_1, 0, -71);
            lv_anim_set_values(&anim, (cnt%2!=1) ? -71 : 0, (cnt%2==1) ? -71 : 0);  // 从当前位置滚动到 x = 150
            lv_anim_start(&anim);
        }
        else {
            cnt = 0;
        }
        cnt ++;
        vTaskDelay(1000);
    }
}
