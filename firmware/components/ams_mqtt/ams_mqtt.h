#pragma once

#include <string.h>
#include "mqtt_client.h"

typedef struct filament_msg{
    int event_id;
    int msg_id;
    int ams_id;
    int amd_id;
    char filament_type[32];
    uint32_t color;
}filament_msg_t;

typedef struct {
    char *payload;            // 动态申请的 JSON 字符串
    size_t length;            // JSON 字符串长度
    uint16_t sequence_id;     // 随机生成的 0 ~ 65535 之间的 ID
    uint16_t timeout_cnt;
    //message_type_t msg_type;  // 消息类型
} message_t;

extern QueueHandle_t xqueue_ams_msg; 
extern QueueHandle_t xqueue_cali_get_msg;

void init_mqtt(char* printer_ip,char* printer_password,char* printer_device_id);
int enqueue_print_message(char* _payload, size_t _len,uint16_t _sequence_id);
int mqtt_send_filament_setting();
int mqtt_send_get_cali_list();
void bambu_mqtt_disconnect();