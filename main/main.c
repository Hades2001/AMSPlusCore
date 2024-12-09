#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "lvgl.h"
#include "gui_guider.h"

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
}
