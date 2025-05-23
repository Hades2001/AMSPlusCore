#pragma once

#include "esp_http_server.h"
#include "esp_wifi.h"

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

typedef enum httpmsg_type{
    kHTTPMSG_DEVICE_SET = 0,
    kHTTPMSG_CALI_GET,
    kHTTPMSG_INTO_OTA,
    kHTTPMSG_MAX,
}httpmsg_type_t;

typedef struct http_config{
    int  type;
    char version[32];
    char ssid[32];
    char password[64];
    char rssi[8];
    char printer_ip[20];
    char printer_password[16];
    char printer_device_id[20];
    char ams_id[4];
}http_config_t;

void wifi_init_sta(char* wifi_ssid,char* wifi_pwd);
esp_err_t save_config_to_nvs(const http_config_t *config);
esp_err_t load_config_from_nvs(http_config_t *config);
void wifi_init_softap(void *event_handler_arg);
void generate_softap_ssid(char* buff, size_t size);
httpd_handle_t start_config_webserver(void);
httpd_handle_t start_gen_webserver(void);
char* get_device_ip(void);

void initialize_sntp(void);
void obtain_time(void);

extern EventGroupHandle_t s_wifi_event_group;
extern QueueHandle_t xqueue_http_msg;
extern QueueHandle_t xqueue_ota_msg;
extern QueueHandle_t xqueue_get_calilist_msg;
extern QueueHandle_t xqueue_calilist_json_msg; 