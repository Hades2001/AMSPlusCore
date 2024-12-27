#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "esp_log.h"
#include "cJSON.h"

#include "ams_ntag.h"

static const char *TAG = "AMS_NTAG";

// 定义键值对结构
typedef struct {
    const char *key;    // 键
    const char *value;  // 值
} KeyValuePair;

// 定义映射
const KeyValuePair filament_mappings[] = {
    {"TPU", "GFU99"},
    {"PLA", "GFL99"},
    {"PLA High Speed", "GFL95"},
    {"PLA Silk", "GFL96"},
    {"PETG", "GFG99"},
    {"PET-CF", "GFG99"},
    {"ASA", "GFB98"},
    {"ABS", "GFB99"},
    {"PC", "GFC99"},
    {"PA", "GFN99"},
    {"PA-CF", "GFN98"},
    {"PLA-CF", "GFL98"},
    {"PVA", "GFS99"},
    {"BVOH", "GFS97"},
    {"EVA", "GFR99"},
    {"HIPS", "GFS98"},
    {"PCTG", "GFG97"},
    {"PE", "GFP99"},
    {"PE-CF", "GFP98"},
    {"PHA", "GFR98"},
    {"PP", "GFP97"},
    {"PP-CF", "GFP96"},
    {"PP-GF", "GFP95"},
    {"PPA-CF", "GFN97"},
    {"PPA-GF", "GFN96"},
    {"Support", "GFS00"}
};

// 映射的大小
#define MAPPINGS_SIZE (sizeof(filament_mappings) / sizeof(filament_mappings[0]))

// 查找函数
const char* find_mapping(const char *key) {
    for (size_t i = 0; i < MAPPINGS_SIZE; i++) {
        if (strcmp(filament_mappings[i].key, key) == 0) {
            return filament_mappings[i].value;
        }
    }
    return NULL;  // 未找到时返回
}

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

    // 解析 surface
    cJSON *surface = cJSON_GetObjectItemCaseSensitive(payload, "surface");
    if (cJSON_IsString(surface) && (surface->valuestring != NULL)) {
        info->surface[0] = surface->valuestring[0];
    }

    cJSON_Delete(json);
    return 0;
}


int filament_setting_payload(char *json_string, size_t length, filament_info *info, int scanner_id, int ams_id){

    cJSON *root = cJSON_CreateObject();
    // Create the "print" object
    cJSON *print = cJSON_CreateObject();

    // Add key-value pairs to the "print" object
    cJSON_AddStringToObject(print, "sequence_id", "0");
    cJSON_AddStringToObject(print, "command", "ams_filament_setting");
    cJSON_AddNumberToObject(print, "ams_id", ams_id);
    int tray_id = (info->surface[0] == 'A') ? (( scanner_id * 2 ) - 1) : (( scanner_id * 2 ) - 2);
    cJSON_AddNumberToObject(print, "tray_id", tray_id);
    
    char colorsrt[9];
    memset(colorsrt,0,(size_t)9);
    sprintf(colorsrt,"%02X%02X%02X%02X",(uint8_t)((info->color_hex>>16)&0xff),(uint8_t)((info->color_hex>>8)&0xff),(uint8_t)((info->color_hex)&0xff),0xff);
    cJSON_AddStringToObject(print, "tray_color", colorsrt);
    cJSON_AddNumberToObject(print, "nozzle_temp_min", info->min_temp);
    cJSON_AddNumberToObject(print, "nozzle_temp_max", info->max_temp);
    cJSON_AddStringToObject(print, "tray_type", info->type);
    cJSON_AddStringToObject(print, "setting_id", "");
    cJSON_AddStringToObject(print, "tray_info_idx", find_mapping(info->type));

    // Add the "print" object to the root
    cJSON_AddItemToObject(root, "print", print);

    // Convert JSON structure to string
    char *json_str = cJSON_PrintUnformatted(root);
    strlcpy(json_string,json_str,length);

    cJSON_Delete(print);
    free(json_str);
    
    return 0;
}