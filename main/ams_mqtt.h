#pragma once

#include <string.h>

typedef struct filament_msg{
    int amd_id;
    char filament_type[32];
    uint32_t color;
}filament_msg_t;

void init_mqtt(void);