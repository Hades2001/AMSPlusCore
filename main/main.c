#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "aht10.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define TAG "QMSD-MAIN"

lv_ui ams_ui;

void gui_user_init() {
    //lv_obj_t* main_page = lv_obj_create(NULL);
    //lv_obj_set_style_bg_color(main_page, lv_color_make(0xff, 0x00, 0x00), 0);
    //lv_obj_t* label = lv_label_create(main_page);
    //lv_label_set_text_static(label, "Hello Smartpanle");
    //lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    //lv_obj_center(label);
    
    setup_scr_screen(&ams_ui);

    //lv_obj_t * img1 = lv_img_create(main_page);
    //lv_img_set_src(img1, &WebUI2_map);
    //lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_size(img1, 240, 135);
    lv_label_set_text(ams_ui.screen_lab_filament_1, "PLA-Basic");
    lv_label_set_text(ams_ui.screen_lab_filament_2, "PETG-CF");
    lv_obj_set_style_bg_color(ams_ui.screen_box_filament_2, lv_color_hex(0xff0000), LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_scr_load(ams_ui.screen);
}

#define RC522_SPI_BUS_GPIO_MISO (7)
#define RC522_SPI_BUS_GPIO_MOSI (6)
#define RC522_SPI_BUS_GPIO_SCLK (5)

#define RC522_SPI_SCANNER_1_GPIO_SDA (16)
#define RC522_SPI_SCANNER_2_GPIO_SDA (4)

void app_main(void) {
    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
    config.gui.en = true;
    config.board_dir = BOARD_ROTATION_90;
    config.touch.en = false;
    qmsd_board_init(&config);
    printf("Fine qmsd!\r\n");


    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    //nvs_open();

    init_aht10(BOARD_I2C_SCL_PIN,BOARD_I2C_SDA_PIN);
    float temp,hum;
    char tempstr[10];
    while(1){
        if(get_aht10_data(&temp,&hum) == 0 ){
            memset(tempstr,'\0',sizeof(tempstr));
            sprintf(tempstr,"%.1f°C",temp);
            lv_label_set_text(ams_ui.screen_lab_temp,tempstr);
        
            memset(tempstr,'\0',sizeof(tempstr));
            sprintf(tempstr,"%d%%",((uint16_t)(hum*100))%100);
            lv_label_set_text(ams_ui.screen_lab_hum, tempstr);

            lv_bar_set_value(ams_ui.screen_bar_hum, ((uint16_t)(hum*100))%100, LV_ANIM_OFF);

            ESP_LOGI(TAG,"T:%.2f,H:%.2f",temp,hum);
        }
        vTaskDelay(100);
    }
}
