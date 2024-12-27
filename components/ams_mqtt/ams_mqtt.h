#pragma once

#include <string.h>
#include "mqtt_client.h"

typedef struct filament_msg{
    int event_id;
    int msg_id;
    int amd_id;
    char filament_type[32];
    uint32_t color;
}filament_msg_t;

void init_mqtt(char* printer_ip,char* printer_password,char* printer_device_id);
int mqtt_send_filament_setting(char* payload);