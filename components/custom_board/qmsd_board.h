#pragma once

#include "stdint.h"
#include "sdkconfig.h"
#include "qmsd_board_pin.h"
#include "qmsd_board_def.h"
#include "qmsd_board_config.h"
#include "qmsd_board_utils.h"
#include "qmsd_touch.h"
#include "qmsd_gui.h"
#include "screen_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

void qmsd_board_init(qmsd_board_config_t* config);

void qmsd_board_init_screen();

void qmsd_board_init_touch();

//void qmsd_board_init_gui();

void screen_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
void screen_draw_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color);
void screen_draw_pixel(uint16_t x, uint16_t y,uint16_t color);

void scan_i2c(void);

scr_driver_t* qmsd_board_get_screen_driver();

#ifdef __cplusplus
}
#endif
