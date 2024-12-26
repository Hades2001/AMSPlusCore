/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"



void setup_scr_screen_21(lv_ui *ui){
#if CRE_TILE
	//Write codes screen_21
	ui->screen_21 = lv_obj_create(ui->screen_5_tileview_1_tile_20);
#endif
	lv_obj_set_scrollbar_mode(ui->screen_21, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_21_main_main_default
	static lv_style_t style_screen_21_main_main_default;
	if (style_screen_21_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_21_main_main_default);
	else
		lv_style_init(&style_screen_21_main_main_default);
	lv_style_set_bg_color(&style_screen_21_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_bg_opa(&style_screen_21_main_main_default, 255);
	lv_obj_add_style(ui->screen_21, &style_screen_21_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_21_img_1
	ui->screen_21_img_1 = lv_img_create(ui->screen_21);
	lv_obj_set_pos(ui->screen_21_img_1, 103, 47);
	lv_obj_set_size(ui->screen_21_img_1, 33, 25);
	lv_obj_set_scrollbar_mode(ui->screen_21_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_21_img_1_main_main_default
	static lv_style_t style_screen_21_img_1_main_main_default;
	if (style_screen_21_img_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_21_img_1_main_main_default);
	else
		lv_style_init(&style_screen_21_img_1_main_main_default);
	lv_style_set_img_recolor(&style_screen_21_img_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_21_img_1_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_21_img_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_21_img_1, &style_screen_21_img_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_21_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_21_img_1,&_wifi_s_33x25);
	lv_img_set_pivot(ui->screen_21_img_1, 50,50);
	lv_img_set_angle(ui->screen_21_img_1, 0);

	//Write codes screen_21_lab_SSID
	ui->screen_21_lab_SSID = lv_label_create(ui->screen_21);
	lv_obj_set_pos(ui->screen_21_lab_SSID, 0, 107);
	lv_obj_set_size(ui->screen_21_lab_SSID, 240, 25);
	lv_obj_set_scrollbar_mode(ui->screen_21_lab_SSID, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_21_lab_SSID, "SSID:xxxxxxx");
	lv_label_set_long_mode(ui->screen_21_lab_SSID, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_21_lab_SSID_main_main_default
	static lv_style_t style_screen_21_lab_SSID_main_main_default;
	if (style_screen_21_lab_SSID_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_21_lab_SSID_main_main_default);
	else
		lv_style_init(&style_screen_21_lab_SSID_main_main_default);
	lv_style_set_radius(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_21_lab_SSID_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_21_lab_SSID_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_21_lab_SSID_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_text_color(&style_screen_21_lab_SSID_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_21_lab_SSID_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_text_align(&style_screen_21_lab_SSID_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_21_lab_SSID_main_main_default, 0);
	lv_obj_add_style(ui->screen_21_lab_SSID, &style_screen_21_lab_SSID_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_21_spinner_1
	ui->screen_21_spinner_1 = lv_spinner_create(ui->screen_21, 1000, 120);
	lv_obj_set_pos(ui->screen_21_spinner_1, 82, 21);
	lv_obj_set_size(ui->screen_21_spinner_1, 76, 76);

	//Write style state: LV_STATE_DEFAULT for style_screen_21_spinner_1_main_main_default
	static lv_style_t style_screen_21_spinner_1_main_main_default;
	if (style_screen_21_spinner_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_21_spinner_1_main_main_default);
	else
		lv_style_init(&style_screen_21_spinner_1_main_main_default);
	lv_style_set_bg_color(&style_screen_21_spinner_1_main_main_default, lv_color_make(0xee, 0xee, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_21_spinner_1_main_main_default, lv_color_make(0xee, 0xee, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_21_spinner_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_21_spinner_1_main_main_default, 0);
	lv_style_set_pad_left(&style_screen_21_spinner_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_21_spinner_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_21_spinner_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_21_spinner_1_main_main_default, 0);
	lv_style_set_arc_color(&style_screen_21_spinner_1_main_main_default, lv_color_make(0xd5, 0xd6, 0xde));
	// lv_style_set_arc_color(&style_screen_21_spinner_1_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_arc_width(&style_screen_21_spinner_1_main_main_default, 10);
	lv_obj_add_style(ui->screen_21_spinner_1, &style_screen_21_spinner_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_21_spinner_1_main_indicator_default
	static lv_style_t style_screen_21_spinner_1_main_indicator_default;
	if (style_screen_21_spinner_1_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_21_spinner_1_main_indicator_default);
	else
		lv_style_init(&style_screen_21_spinner_1_main_indicator_default);
	lv_style_set_arc_color(&style_screen_21_spinner_1_main_indicator_default, lv_color_make(0x4d, 0x97, 0xF6));
	lv_style_set_arc_width(&style_screen_21_spinner_1_main_indicator_default, 10);
	lv_style_set_opa(&style_screen_21_spinner_1_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_21_spinner_1, &style_screen_21_spinner_1_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
}