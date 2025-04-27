#pragma once

#include <string.h>
#include "mqtt_client.h"

typedef enum {
    kEVENT_ID_FILAMENT = 0,
    kEVENT_ID_RECONFIG = 1,
    kEVENT_ID_PRINT_PAUSED = 2,
    kEVENT_ID_PRINT_PRINTING = 3,
    kEVENT_ID_PRINT_FINISHED = 4,
    kEVENT_ID_CALI_GET = 5,
    kEVENT_ID_CALI_SET = 6,
    kEVENT_ID_CALI_LIST = 7,
} event_id_t;

typedef enum {
    kSTATE_MQTT_CONNECTING = 0,
    kSTATE_MQTT_CONNECTED = 1,
    kSTATE_MQTT_DISCONNECTED = 2,
} mqtt_state_t;

typedef struct filament_msg{
    int event_id;
    int sub_event_id;
    int msg_id;
    int ams_id;
    int amd_id;
    char filament_type[32];
    uint32_t color;
    char *extrusion_cali_list_ptr;
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