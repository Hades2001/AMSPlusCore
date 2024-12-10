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

#define TAG "QMSD-MAIN"

lv_ui ams_ui;

void gui_user_init() {
    //lv_obj_t* main_page = lv_obj_create(NULL);
    //lv_obj_set_style_bg_color(main_page, lv_color_make(0xff, 0x00, 0x00), 0);
    //lv_obj_t* label = lv_label_create(main_page);
    //lv_label_set_text_static(label, "Hello Smartpanle");
    //lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    //lv_obj_center(label);
    
    setup_scr_screen(&ams_ui);

    //lv_obj_t * img1 = lv_img_create(main_page);
    //lv_img_set_src(img1, &WebUI2_map);
    //lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_size(img1, 240, 135);
    lv_label_set_text(ams_ui.screen_lab_filament_1, "PLA-Basic");
    lv_label_set_text(ams_ui.screen_lab_filament_2, "PETG-CF");
    lv_obj_set_style_bg_color(ams_ui.screen_box_filament_2, lv_color_hex(0xff0000), LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_scr_load(ams_ui.screen);
}

#define RC522_SPI_BUS_GPIO_MISO (7)
#define RC522_SPI_BUS_GPIO_MOSI (6)
#define RC522_SPI_BUS_GPIO_SCLK (5)

#define RC522_SPI_SCANNER_1_GPIO_SDA (16)
#define RC522_SPI_SCANNER_2_GPIO_SDA (4)

#define EXAMPLE_ESP_WIFI_SSID      "zxwifi2g4"
#define EXAMPLE_ESP_WIFI_PASS      "12345678"
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
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
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
    //nvs_open();
    wifi_init_sta();

    init_aht10(BOARD_I2C_SCL_PIN,BOARD_I2C_SDA_PIN);
    float temp,hum;
    char tempstr[10];
    while(1){
        if(get_aht10_data(&temp,&hum) == 0 ){
            memset(tempstr,'\0',sizeof(tempstr));
            sprintf(tempstr,"%.1f°C",temp);
            lv_label_set_text(ams_ui.screen_lab_temp,tempstr);
        
            memset(tempstr,'\0',sizeof(tempstr));
            sprintf(tempstr,"%d%%",((uint16_t)(hum*100))%100);
            lv_label_set_text(ams_ui.screen_lab_hum, tempstr);

            lv_bar_set_value(ams_ui.screen_bar_hum, ((uint16_t)(hum*100))%100, LV_ANIM_OFF);

            ESP_LOGI(TAG,"T:%.2f,H:%.2f",temp,hum);
        }
        vTaskDelay(100);
    }
}
