#pragma once

#include <string.h>

typedef struct {
    char protocol[32];
    char version[16];
    char type[16];
    uint32_t color_hex;
    char brand[32];
    int min_temp;
    int max_temp;
} filament_info;

char* parse_command(const char *json_string, size_t length);
int parse_payload(const char *json_string, size_t length, filament_info *info);
