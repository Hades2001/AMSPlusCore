/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

#define SERVER_URL	("HTTP://192.168.4.1")

typedef struct
{
	lv_obj_t *screen_11; 	// Open Page
	bool screen_del;
	lv_obj_t *screen_11_label_1;
	lv_obj_t *screen_11_btn_1;
	lv_obj_t *screen_11_btn_1_label;
	lv_obj_t *screen_01;	// QRCode
	bool screen_01_del;
	lv_obj_t *screen_01_label_1;
	lv_obj_t *screen_01_lab_url;
	lv_obj_t *screen_00;	// Wifi PA Info page
	bool screen_00_del;
	lv_obj_t *screen_00_img_1;
	lv_obj_t *screen_00_label_1;
	lv_obj_t *screen_00_lab_desctext;
	// lv_obj_t *screen_00_label_3;
	lv_obj_t *screen_00_label_2;
	lv_obj_t *screen_21;	// Wifi Connect
	bool screen_21_del;
	lv_obj_t *screen_21_img_1;
	lv_obj_t *screen_21_lab_SSID;
	lv_obj_t *screen_21_spinner_1;
	lv_obj_t *screen_5;
	bool screen_5_del;
	lv_obj_t *screen_5_tileview_1;
	lv_obj_t *screen_5_tileview_1_tile_00;
	lv_obj_t *screen_5_tileview_1_tile_01;
	lv_obj_t *screen_5_tileview_1_tile_10;
	lv_obj_t *screen_5_tileview_1_tile_20;
	lv_obj_t *screen_5_tileview_1_tile_30;
	lv_obj_t *screen_5_cont_1;
	lv_obj_t *screen_31;
	bool screen_31_del;
	lv_obj_t *screen_31_box_filament_1;
	lv_obj_t *screen_31_box_filament_2;
	lv_obj_t *screen_31_box_filament_3;
	lv_obj_t *screen_31_box_filament_4;
	lv_obj_t *screen_31_lab_filament_1;
	lv_obj_t *screen_31_lab_filament_2;
	lv_obj_t *screen_31_lab_filament_3;
	lv_obj_t *screen_31_lab_filament_4;
	lv_obj_t *screen_31_img_1;
	lv_obj_t *screen_31_img_2;
	lv_obj_t *screen_31_img_3;
	lv_obj_t *screen_31_img_4;
	lv_obj_t *screen_31_img_wifi_on;
	lv_obj_t *screen_31_img_connect_on;
	lv_obj_t *screen_31_label_5;
	lv_obj_t *screen_31_lab_hum;
	lv_obj_t *screen_31_lab_temp;
	lv_obj_t *screen_31_bar_hum;
	lv_obj_t *screen_31_lab_info;
	lv_anim_t screen_31_anim;

	lv_obj_t *screen_41;
	bool screen_41_del;
	lv_obj_t *screen_41_img_ota;
	lv_obj_t *screen_41_label_ota;
	lv_obj_t *screen_41_bar_ota;

	lv_obj_t *qr_code;
}lv_ui;

void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui ams_ui;
void setup_scr_screen_00(lv_ui *ui);
void setup_scr_screen_01(lv_ui *ui);
void setup_scr_screen_11(lv_ui *ui);
void setup_scr_screen_21(lv_ui *ui);
void setup_scr_screen_31(lv_ui *ui);
void setup_scr_screen_5(lv_ui *ui);
void setup_scr_screen_41(lv_ui *ui);
LV_IMG_DECLARE(_wifi_s_33x25);
LV_IMG_DECLARE(_count_26x26);
LV_IMG_DECLARE(_wifi_s_20x16);
LV_IMG_DECLARE(_icon_18x17);
LV_IMG_DECLARE(_wifi_50x38);
LV_IMG_DECLARE(_ota_32x32);

#ifdef __cplusplus
}
#endif
#endif