#pragma once

#include <string.h>

typedef struct {
    char protocol[32];
    char version[16];
    char code[16];
    char type[16];
    uint32_t color_hex;
    char brand[32];
    int min_temp;
    int max_temp;
    char surface[1];
    int cali_idx;
} filament_info;

char* parse_command(const char *json_string, size_t length);
int parse_payload(const char *json_string, size_t length, filament_info *info);
int filament_setting_payload(char *json_string, size_t length, filament_info *info, int scanner_id, int ams_id);
int cali_idx_setting_payload(char *json_string, size_t length, filament_info *info, int scanner_id, int ams_id);
