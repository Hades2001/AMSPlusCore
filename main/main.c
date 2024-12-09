#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "lvgl.h"

#define TAG "QMSD-MAIN"

void gui_user_init() {
    lv_obj_t* main_page = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(main_page, lv_color_make(0xff, 0x00, 0x00), 0);
    lv_obj_t* label = lv_label_create(main_page);
    lv_label_set_text_static(label, "Hello Smartpanle");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(label);
    lv_scr_load(main_page);
}

void app_main(void) {
    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
    config.touch.en = false;
    qmsd_board_init(&config);
    printf("Fine qmsd!\r\n");

    screen_draw_fill(0,0,135,80,COLOR_RED);
    screen_draw_fill(0,80,135,80,COLOR_GREEN);
    screen_draw_fill(0,160,135,80,COLOR_BLUE);

}
