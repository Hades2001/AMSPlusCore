#include "nvs_flash.h"
#include "esp_system.h"

#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "easy_wifi.h"

#define TAG "E-WIFI"


static int s_retry_num = 0;
EventGroupHandle_t s_wifi_event_group;

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
#define WIFI_AP_PASSWORD "88888888"
#define WIFI_AP_MAX_STA_CONN       (4)
QueueHandle_t xqueue_http_msg; 

static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);

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

"        if (!ssid || !password || !printerIp || !printerPwd || !printerId) {"
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

/**
 * @brief 处理 “/config” 的 POST 请求，接收表单数据并保存
 */
static esp_err_t config_post_handler(httpd_req_t *req)
{
    // 一般需要比之前稍大一些的缓冲区
    char buf[256] = {0};
    int total_len = req->content_len;
    int received = 0, cur_len = 0;

    if (total_len >= sizeof(buf)) {
        // 如果请求体太大，超出缓冲区，返回 500
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // 循环读取 POST 数据
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len - cur_len);
        if (received <= 0) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[cur_len] = '\0'; // 结尾加上字符串终止符

    ESP_LOGI("WEB_SERVER", "Received config: %s", buf);

    // 定义局部变量来存放各字段
    char ssid[32]              = {0};
    char password[64]          = {0};
    char printer_ip[32]        = {0};
    char printer_password[32]  = {0};
    char printer_device_id[32] = {0};

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

    // =============== 后端非空校验 ===============
    // 防止有人绕过前端 JS 校验，直接发 POST
    if (strlen(ssid) == 0 || strlen(password) == 0 ||
        strlen(printer_ip) == 0 || strlen(printer_password) == 0 ||
        strlen(printer_device_id) == 0) {
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

    // =============== 一切合法后，存入结构体并发送队列 ===============
    http_config_t config_data = {0};
    strlcpy(config_data.ssid, ssid, sizeof(ssid) - 1);
    strlcpy(config_data.password, password, sizeof(password) - 1);
    strlcpy(config_data.printer_ip, printer_ip, sizeof(printer_ip) - 1);
    strlcpy(config_data.printer_password, printer_password, sizeof(printer_password) - 1);
    strlcpy(config_data.printer_device_id, printer_device_id, sizeof(printer_device_id) - 1);

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

httpd_handle_t start_webserver(void)
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
