#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "esp_log.h"
#include "cJSON.h"

#include "ams_ntag.h"

static const char *TAG = "AMS_NTAG";

// 函数1：解析 command 字段
char* parse_command(const char *json_string, size_t length) {
    cJSON *json = cJSON_ParseWithLength(json_string, length);
    if (!json) {
        ESP_LOGI(TAG, "Error parsing JSON: %s", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *command = cJSON_GetObjectItemCaseSensitive(json, "command");
    if (!cJSON_IsString(command) || (command->valuestring == NULL)) {
        ESP_LOGI(TAG, "Error: command field is not a valid string.");
        cJSON_Delete(json);
        return NULL;
    }

    char *command_value = strdup(command->valuestring);
    cJSON_Delete(json);
    return command_value;
}

// 函数2：解析 payload 并填充 filament_info 结构体
int parse_payload(const char *json_string, size_t length, filament_info *info) {
    cJSON *json = cJSON_ParseWithLength(json_string, length);
    if (!json) {
        ESP_LOGI(TAG, "Error parsing JSON: %s", cJSON_GetErrorPtr());
        return -1;
    }

    cJSON *payload = cJSON_GetObjectItemCaseSensitive(json, "payload");
    if (!cJSON_IsObject(payload)) {
        ESP_LOGI(TAG, "Error: payload field is not a valid object.");
        cJSON_Delete(json);
        return -1;
    }

    // 解析 protocol
    cJSON *protocol = cJSON_GetObjectItemCaseSensitive(payload, "protocol");
    if (cJSON_IsString(protocol) && (protocol->valuestring != NULL)) {
        strncpy(info->protocol, protocol->valuestring, sizeof(info->protocol) - 1);
    }

    // 解析 version
    cJSON *version = cJSON_GetObjectItemCaseSensitive(payload, "version");
    if (cJSON_IsString(version) && (version->valuestring != NULL)) {
        strncpy(info->version, version->valuestring, sizeof(info->version) - 1);
    }

    // 解析 type
    cJSON *type = cJSON_GetObjectItemCaseSensitive(payload, "type");
    if (cJSON_IsString(type) && (type->valuestring != NULL)) {
        strncpy(info->type, type->valuestring, sizeof(info->type) - 1);
    }

    // 解析 color_hex
    cJSON *color_hex = cJSON_GetObjectItemCaseSensitive(payload, "color_hex");
    if (cJSON_IsString(color_hex) && (color_hex->valuestring != NULL)) {
        info->color_hex = (uint32_t)strtoul(color_hex->valuestring, NULL, 16);
    }

    // 解析 brand
    cJSON *brand = cJSON_GetObjectItemCaseSensitive(payload, "brand");
    if (cJSON_IsString(brand) && (brand->valuestring != NULL)) {
        strncpy(info->brand, brand->valuestring, sizeof(info->brand) - 1);
    }

    // 解析 min_temp
    cJSON *min_temp = cJSON_GetObjectItemCaseSensitive(payload, "min_temp");
    if (cJSON_IsString(min_temp) && (min_temp->valuestring != NULL)) {
        info->min_temp = atoi(min_temp->valuestring);
    }

    // 解析 max_temp
    cJSON *max_temp = cJSON_GetObjectItemCaseSensitive(payload, "max_temp");
    if (cJSON_IsString(max_temp) && (max_temp->valuestring != NULL)) {
        info->max_temp = atoi(max_temp->valuestring);
    }

    cJSON_Delete(json);
    return 0;
}