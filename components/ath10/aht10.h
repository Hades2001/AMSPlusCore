#pragma once

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define AHT10_IIC_ADDR 0x38

esp_err_t init_aht10(uint8_t scl,uint8_t sda);
int8_t get_aht10_data(float *temp,float *hum);