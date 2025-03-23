#include "nvs_flash.h"
#include "esp_system.h"

#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "easy_wifi.h"
#include "esp_sntp.h"

#include "cJSON.h"

#define TAG "E-WIFI"

static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "system date sync！");
}

void initialize_sntp(void)
{
    ESP_LOGI("NTP", "Initializing SNTP...");
    
    // 配置 NTP 服务器
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    esp_sntp_setservername(0, "time.windows.com");
    esp_sntp_setservername(1, "time.google.com");
    esp_sntp_setservername(2, "time.apple.com");
    esp_sntp_setservername(3, "pool.ntp.org");
    esp_sntp_init();
    
    while (esp_sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
        ESP_LOGI(TAG, "waiting SNTP sync...");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void obtain_time(void)
{
    time_t now = 0;
    struct tm timeinfo = { 0 };
    time(&now);
    localtime_r(&now, &timeinfo);

    // 这里可以输出当前时间，检查是否获取成功
    ESP_LOGI(TAG, "Date now: %s", asctime(&timeinfo));
}

static int s_retry_num = 0;
EventGroupHandle_t s_wifi_event_group;
char* device_ipstr[20] = {};

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
        snprintf(device_ipstr,20,IPSTR,IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(char* wifi_ssid,char* wifi_pwd)
{
    s_wifi_event_group = xEventGroupCreate();

    //ESP_ERROR_CHECK(esp_netif_init());

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
            .ssid = "",
            .password = "",
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
    strncpy((char *)wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char *)wifi_config.sta.password, wifi_pwd, sizeof(wifi_config.sta.password) - 1);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

char* get_device_ip(void)
{
    return device_ipstr;
}

/**
 * @brief 从 NVS 加载 http_config_t 的内容到给定结构体
 * 
 * @param config 存放结果的配置结构体指针
 * @return esp_err_t 
 *         - ESP_OK: 成功加载
 *         - ESP_ERR_NVS_NOT_FOUND: NVS 中未找到某些键
 *         - 其他:    加载失败（参考 esp_err_to_name(err)）
 */
esp_err_t load_config_from_nvs(http_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t err;

    // 打开 NVS 命名空间 "storage"
    err = nvs_open("d_config", NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!", esp_err_to_name(err));
        memset(config,0,sizeof(http_config_t));
        return err;
    }

    // 由于事先不知道字符串在 NVS 中存储的长度，
    // 可以先用 nvs_get_str(handle, key, NULL, &required_size) 获取所需空间
    // 或者直接给出足够的缓冲区（比如我们知道结构体的限制）。
    // 下面直接使用 config->ssid 的大小做为缓冲区最大长度。

    size_t length = 0;

    // 1) 读取 ssid
    length = sizeof(config->ssid);
    err = nvs_get_str(handle, "ssid", config->ssid, &length);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to get ssid, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 2) 读取 password
    length = sizeof(config->password);
    err = nvs_get_str(handle, "password", config->password, &length);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to get password, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 3) 读取 printer_ip
    length = sizeof(config->printer_ip);
    err = nvs_get_str(handle, "p_d_ip", config->printer_ip, &length);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to get printer_ip, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 4) 读取 printer_password
    length = sizeof(config->printer_password);
    err = nvs_get_str(handle, "p_d_pwd", config->printer_password, &length);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to get printer_password, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 5) 读取 printer_device_id
    length = sizeof(config->printer_device_id);
    err = nvs_get_str(handle, "p_d_id", config->printer_device_id, &length);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to get printer_device_id, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 6) 读取 ams_id
    length = sizeof(config->ams_id);
    err = nvs_get_str(handle, "p_ams_id", config->ams_id, &length);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to get ams_id, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 关闭
    nvs_close(handle);

    ESP_LOGI("NVS", "Configuration loaded successfully");
    return ESP_OK;
}


/**
 * @brief 将 http_config_t 的内容保存到 NVS
 * 
 * @param config 要保存的配置结构体指针
 * @return esp_err_t 
 *         - ESP_OK: 保存成功
 *         - 其他:   保存失败（参考 esp_err_t）
 */
esp_err_t save_config_to_nvs(const http_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t err;

    // 打开 NVS 命名空间 "storage"（可自定义）
    // NVS_READWRITE 表示需要读写权限
    err = nvs_open("d_config", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return err;
    }

    // 写入字符串到 NVS
    // 注意：nvs_set_str 第三个参数必须是以 '\0' 结尾的字符串
    err = nvs_set_str(handle, "ssid", config->ssid);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set ssid, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    err = nvs_set_str(handle, "password", config->password);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set password, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    err = nvs_set_str(handle, "p_d_ip", config->printer_ip);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set printer_ip, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    err = nvs_set_str(handle, "p_d_pwd", config->printer_password);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set printer_password, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    err = nvs_set_str(handle, "p_d_id", config->printer_device_id);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set printer_device_id, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    err = nvs_set_str(handle, "p_ams_id", config->ams_id);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set ams_id, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 提交写入操作
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to commit, err = %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    // 关闭
    nvs_close(handle);

    ESP_LOGI("NVS", "Configuration saved successfully");
    return ESP_OK;
}

#define WIFI_AP_SSID_PREFIX "amsplus_"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_AP_MAX_STA_CONN       (4)
QueueHandle_t xqueue_http_msg; 

//static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);

void wifi_init_softap(void *event_handler_arg)
{
    // 1. 初始化 NVS，用于存储 Wi-Fi 配置信息
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. 初始化 tcpip 和事件循环
    //ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 3. 创建默认的 Wi-Fi AP netif
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
    assert(ap_netif);

    // 4. 配置 Wi-Fi 模式
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 5. 注册事件处理（这里简化处理）
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        event_handler_arg, // 需要在下面实现
                                                        NULL,
                                                        NULL));

    // 6. 获取自定义 SSID
    char ap_ssid[32] = {0};
    generate_softap_ssid(ap_ssid, sizeof(ap_ssid));

    // 7. 配置 SoftAP
    wifi_config_t wifi_config = {
        .ap = {
            // 自定义 SSID
            .ssid = "",
            .ssid_len = 0,  // 在 strcpy 后再设置
            .channel = 1,
            .password = "12345678",
            .max_connection = WIFI_AP_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            //.authmode = WIFI_AUTH_OPEN, // 若需要开放式 AP，可取消注释
        },
    };
    strcpy((char *)wifi_config.ap.ssid, ap_ssid);
    wifi_config.ap.ssid_len = strlen(ap_ssid);
    strcpy((char *)wifi_config.ap.password, "12345678"); // 若使用 WPA2，加密码

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi SoftAP started with SSID:%s password:%s", ap_ssid, wifi_config.ap.password);
}

void generate_softap_ssid(char* buff, size_t size)
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    snprintf(buff, size, WIFI_AP_SSID_PREFIX "%02X%02X%02X", mac[3], mac[4], mac[5]);
}

/**
 * @brief 处理根路径 “/” 的 GET 请求，返回一个简单的 HTML 表单
 */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    const char *response =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "    <meta charset=\"UTF-8\">"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "    <title>WiFi & Bambu 3D Printer Config</title>"
    "    <style>"
    "        body {"
    "            margin: 0;"
    "            padding: 0;"
    "            font-family: Arial, sans-serif;"
    "            background-color: #121212;" /* 暗色背景 */
    "            color: #ffffff;"
    "        }"
    "        .container {"
    "            display: flex;"
    "            flex-direction: column;"
    "            align-items: center;"
    "            justify-content: center;"
    "            min-height: 100vh;"
    "            padding: 0 20px;"
    "        }"
    "        .form-wrapper {"
    "            background-color: #1e1e1e;"
    "            border-radius: 8px;"
    "            padding: 20px;"
    "            max-width: 400px;"
    "            width: 100%;"
    "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.5);"
    "        }"
    "        .form-title {"
    "            margin-top: 0;"
    "            text-align: center;"
    "            margin-bottom: 20px;"
    "        }"
    "        .form-group {"
    "            display: flex;"
    "            flex-direction: column;"
    "            margin-bottom: 15px;"
    "        }"
    "        .label {"
    "            margin-bottom: 5px;"
    "            font-weight: bold;"
    "        }"
    "        .input {"
    "            padding: 10px;"
    "            border-radius: 4px;"
    "            border: 1px solid #333333;"
    "            background-color: #2a2a2a;"
    "            color: #ffffff;"
    "        }"
    "        .button {"
    "            margin-top: 15px;"
    "            background-color: #007bff;"
    "            border: none;"
    "            padding: 15px;"
    "            border-radius: 4px;"
    "            cursor: pointer;"
    "            width: 100%;"
    "            color: #ffffff;"
    "            font-size: 16px;"
    "        }"
    "        .button:hover {"
    "            background-color: #005dbf;"
    "        }"
    "        .select {"
    "            padding: 10px;"
    "            font-size: 16px;"
    "            border-radius: 4px;"
    "            border: 1px solid #333333;"
    "            background-color: #2a2a2a;"
    "            color: #ffffff;"
    "            height: 40px;"
    "            -webkit-appearance: none;"
    "            -moz-appearance: none;"
    "            appearance: none;"
    "        }"
    "        .select:focus {"
    "            border-color: #007bff;"
    "        }"
    "    </style>"
    "</head>"
    "<body>"
    "    <div class=\"container\">"
    "        <div class=\"form-wrapper\">"
    "            <h2 class=\"form-title\">WiFi & Bambu 3D Printer Config</h2>"
    "            <form id=\"configForm\" action=\"/config\" method=\"post\" onsubmit=\"return validateForm();\">"
    "                <div class=\"form-group\">"
    "                    <label class=\"label\" for=\"ssid\">Wi-Fi SSID</label>"
    "                    <input class=\"input\" type=\"text\" id=\"ssid\" name=\"ssid\">"
    "                </div>"
    "                <div class=\"form-group\">"
    "                    <label class=\"label\" for=\"password\">Wi-Fi Password</label>"
    "                    <input class=\"input\" type=\"password\" id=\"password\" name=\"password\">"
    "                </div>"
    "                <div class=\"form-group\">"
    "                    <label class=\"label\" for=\"printer_ip\">Bambu 3D printing IP</label>"
    "                    <input class=\"input\" type=\"text\" id=\"printer_ip\" name=\"printer_ip\" placeholder=\"e.g. 192.168.1.100\">"
    "                </div>"
    "                <div class=\"form-group\">"
    "                    <label class=\"label\" for=\"printer_password\">Bambu 3D printing password</label>"
    "                    <input class=\"input\" type=\"password\" id=\"printer_password\" name=\"printer_password\">"
    "                </div>"
    "                <div class=\"form-group\">"
    "                    <label class=\"label\" for=\"printer_device_id\">Bambu 3D printing DeviceID</label>"
    "                    <input class=\"input\" type=\"text\" id=\"printer_device_id\" name=\"printer_device_id\">"
    "                </div>"
    "                <div class=\"form-group\">"
    "                <label class=\"label\" for=\"ams_id\">Bambu AMS ID</label>"
    "                <select class=\"input select\" id=\"ams_id\" name=\"ams_id\">"
    "                    <option value=\"0\">0</option>"
    "                    <option value=\"1\">1</option>"
    "                    <option value=\"2\">2</option>"
    "                    <option value=\"3\">3</option>"
    "                </select>"
    "                </div>"
    "                <button class=\"button\" type=\"submit\">Save</button>"
    "            </form>"
    "        </div>"
    "    </div>"

    "    <script>"
    "    function validateForm() {"
    "        var ssid = document.getElementById('ssid').value.trim();"
    "        var password = document.getElementById('password').value.trim();"
    "        var printerIp = document.getElementById('printer_ip').value.trim();"
    "        var printerPwd = document.getElementById('printer_password').value.trim();"
    "        var printerId = document.getElementById('printer_device_id').value.trim();"
    "        var amsId = document.getElementById('ams_id').value;"

    "        if (!ssid || !password || !printerIp || !printerPwd || !printerId || amsId === "") {"
    "            alert('All fields are required. Please fill them before submitting.');"
    "            return false;"
    "        }"
    "        return true;"
    "    }"
    "    </script>"

    "</body>"
    "</html>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

void url_decode(const char *src, char *dst, size_t dst_size)
{
    size_t i = 0, j = 0;
    while (src[i] && j < (dst_size - 1)) {
        if (src[i] == '%' &&
            isxdigit((int)src[i+1]) &&
            isxdigit((int)src[i+2])) 
        {
            // 解析 %xx
            char hex[3] = { src[i+1], src[i+2], '\0' };
            dst[j++] = (char) strtol(hex, NULL, 16);
            i += 3;
        } else if (src[i] == '+') {
            // application/x-www-form-urlencoded 中 + 表示空格
            dst[j++] = ' ';
            i++;
        } else {
            // 普通字符
            dst[j++] = src[i++];
        }
    }
    dst[j] = '\0';
}
/**
 * @brief 处理 “/config” 的 POST 请求，接收表单数据并保存
 */
static esp_err_t config_post_handler(httpd_req_t *req)
{
    // 一般需要比之前稍大一些的缓冲区
    char buf[256] = {0};
    char s_buf[256] = {0};
    int total_len = req->content_len;
    int received = 0, cur_len = 0;

    if (total_len >= sizeof(buf)) {
        // 如果请求体太大，超出缓冲区，返回 500
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // 循环读取 POST 数据
    while (cur_len < total_len) {
        received = httpd_req_recv(req, s_buf + cur_len, total_len - cur_len);
        if (received <= 0) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        cur_len += received;
    }
    s_buf[cur_len] = '\0'; // 结尾加上字符串终止符
    url_decode(s_buf,buf,sizeof(buf));

    ESP_LOGI("WEB_SERVER", "Received config: %s", buf);

    // 定义局部变量来存放各字段
    char ssid[32]              = {0};
    char password[64]          = {0};
    char printer_ip[32]        = {0};
    char printer_password[32]  = {0};
    char printer_device_id[32] = {0};
    char ams_id[32]            = {0};
    // 依次解析表单字段 (ssid, password, printer_ip, printer_password, printer_device_id)
    // 简易解析方式，可以用 strtok 或者更好的 HTTP 解析库，示例仅供参考

    // 1) 解析 ssid
    char *p = strstr(buf, "ssid=");
    if (p) {
        p += strlen("ssid=");
        char *end = strchr(p, '&');
        if (end) {
            memcpy(ssid, p, end - p);
            ssid[end - p] = '\0';
        } else {
            strcpy(ssid, p);
        }
    }

    // 2) 解析 password
    p = strstr(buf, "password=");
    if (p) {
        p += strlen("password=");
        char *end = strchr(p, '&');
        if (end) {
            memcpy(password, p, end - p);
            password[end - p] = '\0';
        } else {
            strcpy(password, p);
        }
    }

    // 3) 解析 printer_ip
    p = strstr(buf, "printer_ip=");
    if (p) {
        p += strlen("printer_ip=");
        char *end = strchr(p, '&');
        if (end) {
            memcpy(printer_ip, p, end - p);
            printer_ip[end - p] = '\0';
        } else {
            strcpy(printer_ip, p);
        }
    }

    // 4) 解析 printer_password
    p = strstr(buf, "printer_password=");
    if (p) {
        p += strlen("printer_password=");
        char *end = strchr(p, '&');
        if (end) {
            memcpy(printer_password, p, end - p);
            printer_password[end - p] = '\0';
        } else {
            strcpy(printer_password, p);
        }
    }

    // 5) 解析 printer_device_id
    p = strstr(buf, "printer_device_id=");
    if (p) {
        p += strlen("printer_device_id=");
        char *end = strchr(p, '&');
        if (end) {
            memcpy(printer_device_id, p, end - p);
            printer_device_id[end - p] = '\0';
        } else {
            strcpy(printer_device_id, p);
        }
    }

    // 5) 解析 ams_id
    p = strstr(buf, "ams_id=");
    if (p) {
        p += strlen("ams_id=");
        char *end = strchr(p, '&');
        if (end) {
            memcpy(ams_id, p, end - p);
            ams_id[end - p] = '\0';
        } else {
            strcpy(ams_id, p);
        }
    }

    // =============== 后端非空校验 ===============
    // 防止有人绕过前端 JS 校验，直接发 POST
    if (strlen(ssid) == 0 || strlen(password) == 0 ||
        strlen(printer_ip) == 0 || strlen(printer_password) == 0 ||
        strlen(printer_device_id) == 0 || strlen(ams_id) == 0) {
        ESP_LOGE("WEB_SERVER", "Some fields are empty, reject config.");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Error: All fields required!");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Parsed data:");
    ESP_LOGI(TAG, "SSID: %s", ssid);
    ESP_LOGI(TAG, "PWD: %s", password);
    ESP_LOGI(TAG, "Printer IP: %s", printer_ip);
    ESP_LOGI(TAG, "Printer PWD: %s", printer_password);
    ESP_LOGI(TAG, "Printer DeviceID: %s", printer_device_id);
    ESP_LOGI(TAG, "AMS ID: %s", ams_id);
    // =============== 一切合法后，存入结构体并发送队列 ===============
    http_config_t config_data = {0};
    config_data.type = kHTTPMSG_DEVICE_SET;
    strlcpy(config_data.ssid, ssid, sizeof(ssid) - 1);
    strlcpy(config_data.password, password, sizeof(password) - 1);
    strlcpy(config_data.printer_ip, printer_ip, sizeof(printer_ip) - 1);
    strlcpy(config_data.printer_password, printer_password, sizeof(printer_password) - 1);
    strlcpy(config_data.printer_device_id, printer_device_id, sizeof(printer_device_id) - 1);
    strlcpy(config_data.ams_id, ams_id, sizeof(ams_id) - 1);

    // 将结构体发送到队列
    if (xQueueSend(xqueue_http_msg, &config_data, 0) != pdTRUE) {
        ESP_LOGE("WEB_SERVER", "Failed to send config to queue!");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // 返回响应给前端
    const char* resp_str = "Config received. The device will reboot after 5 seconds.";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    //return config_success_handler(req, ssid, password, printer_ip, printer_password, printer_device_id);
    // 这里你可以选择在此处或者在队列接收任务中再去执行 Wi-Fi 连接、Bambu 配置等操作
    // ...
    return ESP_OK;
}

httpd_handle_t start_config_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    // 设置服务器绑定端口，默认为 80
    config.server_port = 80;
    // 对于 SoftAP 情况，默认 IP 即 192.168.4.1

    xqueue_http_msg = xQueueCreate(1,sizeof(http_config_t));

    // 启动 httpd
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        // 注册 URI 处理
        httpd_uri_t root_uri = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &root_uri);

        httpd_uri_t config_uri = {
            .uri       = "/config",
            .method    = HTTP_POST,
            .handler   = config_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &config_uri);

        return server;
    }
    return NULL;
}

#include "esp_ota_ops.h"
#include "esp_flash_partitions.h"

#define OTA_WEB_PORT    80

static int s_ota_progress = -1;

QueueHandle_t xqueue_ota_msg; 

// ================== OTA ================== //

static esp_err_t write_ota_data(const char *data, size_t len, esp_ota_handle_t ota_handle)
{
    return esp_ota_write(ota_handle, data, len);
}

static esp_err_t finalize_ota(esp_ota_handle_t ota_handle, const esp_partition_t *partition)
{
    if (esp_ota_end(ota_handle) != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_end failed");
        return ESP_FAIL;
    }
    if (esp_ota_set_boot_partition(partition) != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "OTA done! Rebooting...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();
    return ESP_OK; // 不会真正执行到这里
}

#include "esp_app_desc.h"

char* device_info_to_json(http_config_t *info)
{
    // 创建 cJSON 根对象
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return NULL; 
    }
    const esp_app_desc_t* app_desc = esp_app_get_description();
    int _rssi;
    esp_wifi_sta_get_rssi(&_rssi);
    snprintf(info->rssi,sizeof(info->rssi),"%d",_rssi);
    // 将结构体字段逐一加入 JSON
    cJSON_AddStringToObject(root, "version",     app_desc->version);
    cJSON_AddStringToObject(root, "ssid",        info->ssid);
    cJSON_AddStringToObject(root, "passwd",      info->password);
    cJSON_AddStringToObject(root, "rssi",        info->rssi);
    cJSON_AddStringToObject(root, "p_ip",        info->printer_ip);
    cJSON_AddStringToObject(root, "p_passwd",    info->printer_password);
    cJSON_AddStringToObject(root, "p_device_id", info->printer_device_id);
    cJSON_AddStringToObject(root, "ams_id",      info->ams_id);

    // 生成 JSON 字符串（带缩进）
    char *json_str = cJSON_PrintUnformatted(root); 
    // 或使用 cJSON_Print() 若想要有缩进和换行

    // 清理根对象
    cJSON_Delete(root);

    // 返回 JSON 字符串，注意：需在外部释放内存（free）
    return json_str;
}

bool json_to_device_info(const char *json_str, http_config_t *info)
{
    if (json_str == NULL || info == NULL) {
        return false;
    }

    // 解析 JSON
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL) {
        ESP_LOGE("JSON", "Failed to parse JSON.");
        return false;
    }

    // 依次获取各字段并赋值
    cJSON *item = NULL;

    // ssid
    item = cJSON_GetObjectItem(root, "ssid");
    if (cJSON_IsString(item)) {
        strncpy(info->ssid, item->valuestring, sizeof(info->ssid) - 1);
    }

    // passwd
    item = cJSON_GetObjectItem(root, "passwd");
    if (cJSON_IsString(item)) {
        strncpy(info->password, item->valuestring, sizeof(info->password) - 1);
    }

    // p_ip
    item = cJSON_GetObjectItem(root, "p_ip");
    if (cJSON_IsString(item)) {
        strncpy(info->printer_ip, item->valuestring, sizeof(info->printer_ip) - 1);
    }

    // p_passwd
    item = cJSON_GetObjectItem(root, "p_passwd");
    if (cJSON_IsString(item)) {
        strncpy(info->printer_password, item->valuestring, sizeof(info->printer_password) - 1);
    }

    // p_device_id
    item = cJSON_GetObjectItem(root, "p_device_id");
    if (cJSON_IsString(item)) {
        strncpy(info->printer_device_id, item->valuestring, sizeof(info->printer_device_id) - 1);
    }

    // ams_id
    item = cJSON_GetObjectItem(root, "ams_id");
    if (cJSON_IsString(item)) {
        strncpy(info->ams_id, item->valuestring, sizeof(info->ams_id) - 1);
    }

    // 释放 JSON 根对象
    cJSON_Delete(root);
    return true;
}



extern const uint8_t temp_index_html_start[]   asm("_binary_index_html_start");
extern const uint8_t temp_index_html_end[]   asm("_binary_index_html_end");

// 主页：展示固件版本信息以及一个上传固件的表单（使用深色主题 + 弹性布局）
static esp_err_t index_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char*)temp_index_html_start, strlen((const char*)temp_index_html_start));
    return ESP_OK;
}

static esp_err_t device_info_get_handler(httpd_req_t *req)
{
    http_config_t device_config;
    esp_err_t ret = load_config_from_nvs(&device_config);
    if(ret!= ESP_OK){
        ESP_LOGE(TAG,"Load device info fault");
        httpd_resp_send(req,"", 0);
        return false;
    }
    httpd_resp_set_type(req, "application/json");
    char* jsonplayload = device_info_to_json(&device_config);
    if(jsonplayload == NULL){
        ESP_LOGE(TAG,"Load device info fault");
        httpd_resp_send(req,"", 0);
        return false;
    }
    httpd_resp_send(req, jsonplayload, strlen(jsonplayload));
    free(jsonplayload);
    return ESP_OK;
}

static esp_err_t device_info_write_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    char* buf = heap_caps_malloc(total_len+2, MALLOC_CAP_SPIRAM);
    if(buf==NULL){
        const char json_msg[] = "{\"status\": \"error\", \"message\": \"no memory to malloc recv buff\"}";
        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, json_msg, HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }

    memset(buf,0,total_len+2);

    ESP_LOGI(TAG, "Received Len: %d", total_len);

    httpd_req_recv(req, buf, total_len);

    ESP_LOGI(TAG, "Received JSON: %s", buf);
    httpd_resp_set_type(req, "application/json");
    http_config_t device_config;
    json_to_device_info(buf,&device_config);
    esp_err_t res = save_config_to_nvs((const http_config_t*)&device_config);
    if(res != ESP_OK){
        const char json_msg[] = "{\"status\": \"error\", \"message\": \"Fault to write NVS\"}";
        httpd_resp_set_type(req, "application/json");
        httpd_resp_send(req, json_msg, HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }
    const char json_msg[] = "{\"status\": \"success\", \"message\": \"Device info saved.\"}";
    httpd_resp_send(req, json_msg, HTTPD_RESP_USE_STRLEN);
    vTaskDelay(2000);
    esp_restart();
    return ESP_OK;
}

typedef enum {
    STATE_FIND_BOUNDARY,
    STATE_PARSE_HEADERS,
    STATE_IN_DATA,
    STATE_FINISHED
} upload_state_t;

static esp_err_t upload_post_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Start receiving firmware (multipart/form-data)");

    // 1. 从头部获取 boundary
    char content_type[128] = {0};
    if (httpd_req_get_hdr_value_str(req, "Content-Type", content_type, sizeof(content_type)) != ESP_OK) {
        ESP_LOGE(TAG, "Content-Type not found!");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Content-Type not found");
        return ESP_FAIL;
    }
    const char *b_ptr = strstr(content_type, "boundary=");
    if (!b_ptr) {
        ESP_LOGE(TAG, "No boundary in Content-Type");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "No boundary in Content-Type");
        return ESP_FAIL;
    }

    httpmsg_type_t type = kHTTPMSG_INTO_OTA;

    if (xQueueSend(xqueue_get_calilist_msg, &type, 10) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to send msg to queue!");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    b_ptr += strlen("boundary=");
    char boundary[64];
    snprintf(boundary, sizeof(boundary), "--%s", b_ptr); // 需加 "--"
    ESP_LOGI(TAG, "Boundary: %s", boundary);

    s_ota_progress = 0;
    xQueueSend(xqueue_ota_msg, &s_ota_progress, 0);

    // 2. OTA 准备
    esp_ota_handle_t ota_handle = 0;
    const esp_partition_t *partition = esp_ota_get_next_update_partition(NULL);
    if (!partition) {
        ESP_LOGE(TAG, "No OTA partition found!");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "No OTA partition found");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset %ld",
             partition->subtype, partition->address);

    if (esp_ota_begin(partition, OTA_SIZE_UNKNOWN, &ota_handle) != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_begin failed!");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "esp_ota_begin failed");
        return ESP_FAIL;
    }
    
    s_ota_progress = 10;
    xQueueSend(xqueue_ota_msg, &s_ota_progress, 0);

    // 3. 状态机 & 行级缓冲
    upload_state_t state = STATE_FIND_BOUNDARY;
    static char line_buf[1024];
    int line_pos = 0; // 当前 line_buf 中有效数据长度

    while (1) {
        // 每次收一小块
        char temp_buf[128];
        int recv_len = httpd_req_recv(req, temp_buf, sizeof(temp_buf));
        if (recv_len < 0) {
            // 出错或断开
            ESP_LOGE(TAG, "httpd_req_recv failed: %d", recv_len);
            esp_ota_end(ota_handle);
            //httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
            httpd_resp_send(req, "{\"success\":false}", HTTPD_RESP_USE_STRLEN);
            return ESP_FAIL;
        } else if (recv_len == 0) {
            // 连接关闭 or 数据结束
            break;
        }

        int offset = 0;
        while (offset < recv_len) {
            // 如果正处于STATE_IN_DATA，我们需要检测 boundary；否则拼行解析
            if (state == STATE_IN_DATA) {
                // 检查是否出现 boundary
                // simplest approach: look for boundary as a substring in the chunk
                // 但 boundary 可能跨 chunk，需要稍微拼到 line_buf 里做搜索

                // 先把本次数据放到 line_buf (暂存区)
                int can_copy = sizeof(line_buf) - line_pos - 1;
                if (can_copy > (recv_len - offset)) {
                    can_copy = (recv_len - offset);
                }
                memcpy(&line_buf[line_pos], &temp_buf[offset], can_copy);
                line_pos += can_copy;
                line_buf[line_pos] = '\0';
                offset += can_copy;

                // 搜索 boundary
                char *found = strstr(line_buf, boundary);
                if (found) {
                    // 找到 boundary，就表示文件数据到此结束
                    int boundary_idx = (int)(found - line_buf);

                    // boundary 之前的内容才是固件
                    if (boundary_idx > 0) {
                        // 写入 OTA
                        esp_err_t r = write_ota_data(line_buf, boundary_idx, ota_handle);
                        if (r != ESP_OK) {
                            ESP_LOGE(TAG, "OTA write error in data section");
                            esp_ota_end(ota_handle);
                            //httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA write error");
                            httpd_resp_send(req, "{\"success\":false}", HTTPD_RESP_USE_STRLEN);
                            return ESP_FAIL;
                        }
                    }

                    // boundary 后面有可能是 `\r\n` 或 `--` 表示结束
                    // 把 boundary 之后的内容移到 line_buf 开头
                    int remain = line_pos - (boundary_idx + strlen(boundary));
                    if (remain > 0) {
                        memmove(line_buf, &line_buf[boundary_idx + strlen(boundary)], remain);
                    }
                    line_pos = remain;
                    line_buf[line_pos] = '\0';

                    // 进入下一步
                    ESP_LOGI(TAG, "Found boundary => finishing OTA data");
                    state = STATE_PARSE_HEADERS; // 可能还有下一个表单字段(本例只处理一个文件)
                } else {
                    // 未找到 boundary，则全部内容都是固件
                    // 为了不重复写入，需要把 line_buf 里能确认无 boundary 的部分写入
                    // 简单做法：假设 boundary 很短(<=64)，可以留出足够余量在 line_buf 里
                    // 先写入 line_buf 长度的一半，留一部分用来匹配可能的 boundary 跨界
                    int safe_len = line_pos - 64; 
                    if (safe_len < 0) safe_len = 0;

                    if (safe_len > 0) {
                        esp_err_t r = write_ota_data(line_buf, safe_len, ota_handle);
                        if (r != ESP_OK) {
                            ESP_LOGE(TAG, "OTA write error in partial data");
                            esp_ota_end(ota_handle);
                            //httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA write error");
                            httpd_resp_send(req, "{\"success\":false}", HTTPD_RESP_USE_STRLEN);
                            return ESP_FAIL;
                        }
                        // 把剩余数据往前挪
                        int remain = line_pos - safe_len;
                        memmove(line_buf, &line_buf[safe_len], remain);
                        line_pos = remain;
                    }
                    // 增加进度（演示用）
                    if (s_ota_progress < 100) s_ota_progress++;
                    xQueueSend(xqueue_ota_msg, &s_ota_progress, 0);
                }
            } 
            else {
                // 不在 DATA 状态，说明要按行解析 (STATE_FIND_BOUNDARY or STATE_PARSE_HEADERS)
                char c = temp_buf[offset++];
                if (c == '\n') {
                    // 行结束(注意行尾可能是 "\r\n" 或 "\n")
                    line_buf[line_pos] = '\0';

                    // 去除行尾的 '\r'
                    if (line_pos > 0 && line_buf[line_pos-1] == '\r') {
                        line_buf[line_pos-1] = '\0';
                    }

                    // 解析此行
                    if (state == STATE_FIND_BOUNDARY) {
                        // 等待出现第一行 boundary: `--xxxxxx`
                        if (strcmp(line_buf, boundary) == 0) {
                            ESP_LOGI(TAG, "First boundary found => parse headers next");
                            state = STATE_PARSE_HEADERS;
                        }
                    } 
                    else if (state == STATE_PARSE_HEADERS) {
                        // 如果这一行为空，表示 headers 结束，进入数据区
                        if (strlen(line_buf) == 0) {
                            ESP_LOGI(TAG, "Headers end => in data");
                            state = STATE_IN_DATA;
                            line_pos = 0;
                            line_buf[0] = '\0';
                        } else {
                            // 可能是: Content-Disposition / Content-Type 等
                            ESP_LOGD(TAG, "Header line: %s", line_buf);
                        }
                    }
                    line_pos = 0; // 重置行缓冲
                    line_buf[0] = '\0';
                } else {
                    // 累计到行缓冲
                    if (line_pos < (int)(sizeof(line_buf) - 1)) {
                        line_buf[line_pos++] = c;
                    } else {
                        // 行过长，可能是异常
                        line_buf[line_pos] = '\0';
                        ESP_LOGW(TAG, "Header line too long, truncating: %s", line_buf);
                        line_pos = 0;
                    }
                }
            }
        }

        // 如果已经读完并且状态是 FINISHED 就退出
        if (state == STATE_FINISHED) {
            break;
        }
    }

    // 如果最后是在 STATE_IN_DATA，说明没有 boundary 截断
    if (state == STATE_IN_DATA) {
        // 说明文件数据直到 EOF 结束(不太符合multipart规范，但也有可能)
        // 写剩余在line_buf中的数据
        if (line_pos > 0) {
            esp_err_t r = write_ota_data(line_buf, line_pos, ota_handle);
            if (r != ESP_OK) {
                ESP_LOGE(TAG, "OTA write error at finalize");
                esp_ota_end(ota_handle);
                //httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA write error");
                httpd_resp_send(req, "{\"success\":false}", HTTPD_RESP_USE_STRLEN);
                return ESP_FAIL;
            }
        }
        line_pos = 0;
        ESP_LOGI(TAG, "No boundary found before EOF, finalize anyway...");
    }

    // 返回响应
    //httpd_resp_sendstr(req, "OTA update successful! Rebooting...");
    httpd_resp_send(req, "{\"success\":true}", HTTPD_RESP_USE_STRLEN);
    // OTA finalize
    if (finalize_ota(ota_handle, partition) != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA finalize failed");
        return ESP_FAIL;
    }

    return ESP_OK;
}

QueueHandle_t xqueue_get_calilist_msg;
QueueHandle_t xqueue_calilist_json_msg; 
// 处理 /calilist/data 请求，返回 JSON
static esp_err_t calilist_data_get_handler(httpd_req_t *req)
{
    httpmsg_type_t type = kHTTPMSG_CALI_GET;

    if (xQueueSend(xqueue_get_calilist_msg, &type, 10) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to send msg to queue!");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    httpd_resp_set_type(req, "application/json");

    char *calilist_json_ptr = NULL;
    ssize_t calilist_json_len = 0;
    if(xQueueReceive(xqueue_calilist_json_msg,&calilist_json_ptr,20000) != pdTRUE){
        ESP_LOGW(TAG,"Failed to get cali list, xQueueReceive fault");
        httpd_resp_send(req, "", calilist_json_len);
        return ESP_FAIL;
    }
    if(calilist_json_ptr == NULL ){
        ESP_LOGW(TAG,"Failed to get cali list, calilist_json_ptr is nullptr");
        httpd_resp_send(req, "", calilist_json_len);
        return ESP_FAIL;
    }
    else{
        calilist_json_len = strlen(calilist_json_ptr);
        ESP_LOGI(TAG,"Success to get cali list %d %s",calilist_json_len,calilist_json_ptr);
        httpd_resp_send(req, calilist_json_ptr, calilist_json_len);
        free(calilist_json_ptr);
    }
    
    return ESP_OK;
}

// 注册 URI 路由
httpd_handle_t start_gen_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = OTA_WEB_PORT;

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        // 根路径 GET
        httpd_uri_t root_uri = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = index_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &root_uri);

        // 注册 /info/r
        httpd_uri_t info_read_uri = {
            .uri       = "/info/r",
            .method    = HTTP_GET,
            .handler   = device_info_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &info_read_uri);

        // 注册 /info/w
        httpd_uri_t info_write_uri = {
            .uri       = "/info/w",
            .method    = HTTP_POST,
            .handler   = device_info_write_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &info_write_uri);

        // 固件上传 POST
        httpd_uri_t upload_uri = {
            .uri       = "/upload",
            .method    = HTTP_POST,
            .handler   = upload_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &upload_uri);

        // 注册 /calilist/data
        httpd_uri_t calilist_data_uri = {
            .uri       = "/calilist/data",
            .method    = HTTP_GET,
            .handler   = calilist_data_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &calilist_data_uri);

        ESP_LOGI(TAG, "HTTP server started on port %d", config.server_port);
    } else {
        ESP_LOGE(TAG, "Error starting server!");
    }

    xqueue_ota_msg = xQueueCreate(5,sizeof(int));
    xqueue_calilist_json_msg = xQueueCreate(2,sizeof(char *));
    xqueue_get_calilist_msg = xQueueCreate(2,sizeof(int));

    return server;
}

