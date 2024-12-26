#include "aht10.h"

//#include "driver/i2c.h"
#include "i2c_device.h"
#include "esp_attr.h"
#include "esp_log.h"

#define TAG "AHT10"

static I2CDevice_t aht10_device;

typedef struct aht10data{
    uint8_t raw_data[6];
    uint8_t flag;
    float temp;
    float hum;
}aht10_d_t;

static aht10_d_t aht10_internal_data;
SemaphoreHandle_t xAHT10Semaphore1 = NULL;

void ath10_read_task(void *arg);
void aht10_reset();
void aht10_start_measure();

esp_err_t init_aht10(uint8_t scl,uint8_t sda)
{
    aht10_device = i2c_malloc_device(I2C_NUM_0, sda, scl, 400000, AHT10_IIC_ADDR);
    if(aht10_device == NULL){
        ESP_LOGE(TAG,"Is no memory to mallo i2c device");
        return ESP_ERR_NO_MEM;
    }
    aht10_reset();
    vTaskDelay(pdMS_TO_TICKS(20));
    uint8_t data[2] = {0x08, 0x00};
    i2c_write_bytes(aht10_device, 0xbe, data, 2);
    vTaskDelay(pdMS_TO_TICKS(10));

    xAHT10Semaphore1 = xSemaphoreCreateBinary();
    xSemaphoreGive(xAHT10Semaphore1);
    
    xTaskCreatePinnedToCore(ath10_read_task, "ath10_read_task", 4 * 1024, NULL, 5, NULL, 1);
    return ESP_OK;
}

void aht10_reset() {
    i2c_write_bytes(aht10_device, 0xba, NULL, 0);
}

void aht10_start_measure() {
    uint8_t data[2] = {0x33, 0x00};
    i2c_write_bytes(aht10_device, 0xac, data, 2);
}

int8_t get_aht10_data(float *temp,float *hum)
{
    if(xSemaphoreTake(xAHT10Semaphore1,(TickType_t)10) == pdTRUE) 
    {
        *temp = aht10_internal_data.temp;
        *hum = aht10_internal_data.hum;
        xSemaphoreGive(xAHT10Semaphore1);
        return 0;
    }
    return -1;
    //if(aht10_internal_data.raw_data[0] & 0x80) return -1;
}

void calculator(aht10_d_t *aht_data){
    uint32_t temp, hum;
    hum = (aht_data->raw_data[1] << 12) | (aht_data->raw_data[2] << 4) | ((aht_data->raw_data[3] >> 4) & 0x0f);
    temp = ((aht_data->raw_data[3] & 0x0f) << 16) | (aht_data->raw_data[4] << 8) | aht_data->raw_data[5];
    aht_data->hum = (float)hum / 1048576.0;
    aht_data->temp = (float)temp / 1048576.0 * 200.0 - 50.0;
}

void ath10_read_task(void *arg) {
    
    aht10_d_t aht_data;
    aht10_start_measure();
    while(1){
        i2c_read_bytes(aht10_device, 0x71, aht_data.raw_data, 6);
        if(aht_data.raw_data[0] & 0x80){
            continue;
        }
        calculator(&aht_data);
        if(xSemaphoreTake(xAHT10Semaphore1,(TickType_t)10) == pdTRUE)
        {
            memcpy(&aht10_internal_data,&aht_data,sizeof(aht10_d_t));
            xSemaphoreGive(xAHT10Semaphore1);
        }
        //ESP_LOGI(TAG,"T:%.2f,H:%.2f",aht_data.temp,aht_data.hum);
        aht10_start_measure();
        vTaskDelay(500);
    }
}
