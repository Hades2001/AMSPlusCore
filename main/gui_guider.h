/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_label_bg;
	lv_obj_t *screen_lab_filament_1;
	lv_obj_t *screen_lab_filament_2;
	lv_obj_t *screen_lab_filament_3;
	lv_obj_t *screen_lab_filament_4;
	lv_obj_t *screen_img_wifi_on;
	lv_obj_t *screen_img_connect_on;
	lv_obj_t *screen_bar_hum;
	lv_obj_t *screen_lab_hum;
	lv_obj_t *screen_lab_temp;
	lv_obj_t *screen_box_filament_1;
	lv_obj_t *screen_box_filament_2;
	lv_obj_t *screen_box_filament_3;
	lv_obj_t *screen_box_filament_4;
}lv_ui;

void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_WIFI_alpha_20x20);
LV_IMG_DECLARE(_connection_alpha_18x18);

LV_FONT_DECLARE(lv_font_Teko_Regular_17)
LV_FONT_DECLARE(lv_customer_font_Teko_Regular_70)
LV_FONT_DECLARE(lv_customer_font_Teko_Regular_35)


#ifdef __cplusplus
}
#endif
#endif
