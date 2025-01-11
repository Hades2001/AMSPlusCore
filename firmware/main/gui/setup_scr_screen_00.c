/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"



void setup_scr_screen_00(lv_ui *ui){
#if CRE_TILE
	//Write codes screen_00
	ui->screen_00 = lv_obj_create(ui->screen_5_tileview_1_tile_01);
#endif
	lv_obj_set_scrollbar_mode(ui->screen_00, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_00_main_main_default
	static lv_style_t style_screen_00_main_main_default;
	if (style_screen_00_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_00_main_main_default);
	else
		lv_style_init(&style_screen_00_main_main_default);
	lv_style_set_bg_color(&style_screen_00_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_bg_opa(&style_screen_00_main_main_default, 255);
	lv_obj_add_style(ui->screen_00, &style_screen_00_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_00_img_1
	ui->screen_00_img_1 = lv_img_create(ui->screen_00);
	lv_obj_set_pos(ui->screen_00_img_1, 95, 21);
	lv_obj_set_size(ui->screen_00_img_1, 50, 38);
	lv_obj_set_scrollbar_mode(ui->screen_00_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_00_img_1_main_main_default
	static lv_style_t style_screen_00_img_1_main_main_default;
	if (style_screen_00_img_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_00_img_1_main_main_default);
	else
		lv_style_init(&style_screen_00_img_1_main_main_default);
	lv_style_set_img_recolor(&style_screen_00_img_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_00_img_1_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_00_img_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_00_img_1, &style_screen_00_img_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_00_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_00_img_1,&_wifi_50x38);
	lv_img_set_pivot(ui->screen_00_img_1, 50,50);
	lv_img_set_angle(ui->screen_00_img_1, 0);

	//Write codes screen_00_label_1
	ui->screen_00_label_1 = lv_label_create(ui->screen_00);
	lv_obj_set_pos(ui->screen_00_label_1, 25, 75);
	lv_obj_set_size(ui->screen_00_label_1, 7, 41);
	lv_obj_set_scrollbar_mode(ui->screen_00_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_00_label_1, "");
	lv_label_set_long_mode(ui->screen_00_label_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_00_label_1_main_main_default
	static lv_style_t style_screen_00_label_1_main_main_default;
	if (style_screen_00_label_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_00_label_1_main_main_default);
	else
		lv_style_init(&style_screen_00_label_1_main_main_default);
	lv_style_set_radius(&style_screen_00_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_00_label_1_main_main_default, lv_color_make(0xa7, 0xa7, 0xa7));
	lv_style_set_bg_grad_color(&style_screen_00_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_00_label_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_00_label_1_main_main_default, 255);
	lv_style_set_text_color(&style_screen_00_label_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_00_label_1_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_00_label_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_00_label_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_00_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_00_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_00_label_1_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_00_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_00_label_1, &style_screen_00_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_00_lab_desctext
	ui->screen_00_lab_desctext = lv_label_create(ui->screen_00);
	lv_obj_set_pos(ui->screen_00_lab_desctext, 41, 73);
	lv_obj_set_size(ui->screen_00_lab_desctext, 180, 25);
	lv_obj_set_scrollbar_mode(ui->screen_00_lab_desctext, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_00_lab_desctext, "Wi-Fi SSID:");
	lv_label_set_long_mode(ui->screen_00_lab_desctext, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_00_lab_desctext_main_main_default
	static lv_style_t style_screen_00_lab_desctext_main_main_default;
	if (style_screen_00_lab_desctext_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_00_lab_desctext_main_main_default);
	else
		lv_style_init(&style_screen_00_lab_desctext_main_main_default);
	lv_style_set_radius(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_00_lab_desctext_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_00_lab_desctext_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_00_lab_desctext_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_text_color(&style_screen_00_lab_desctext_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_00_lab_desctext_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_text_align(&style_screen_00_lab_desctext_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_00_lab_desctext_main_main_default, 0);
	lv_obj_add_style(ui->screen_00_lab_desctext, &style_screen_00_lab_desctext_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

#if 0
	//Write codes screen_00_label_3
	ui->screen_00_label_3 = lv_label_create(ui->screen_00);
	lv_obj_set_pos(ui->screen_00_label_3, 133, 73);
	lv_obj_set_size(ui->screen_00_label_3, 75, 25);
	lv_obj_set_scrollbar_mode(ui->screen_00_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_00_label_3, "SSID:");
	lv_label_set_long_mode(ui->screen_00_label_3, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_00_label_3_main_main_default
	static lv_style_t style_screen_00_label_3_main_main_default;
	if (style_screen_00_label_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_00_label_3_main_main_default);
	else
		lv_style_init(&style_screen_00_label_3_main_main_default);
	lv_style_set_radius(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_00_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_00_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_00_label_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_text_color(&style_screen_00_label_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_00_label_3_main_main_default, &lv_font_Teko_Regular_20);
	lv_style_set_text_letter_space(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_text_align(&style_screen_00_label_3_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_00_label_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_00_label_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_00_label_3, &style_screen_00_label_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
#endif

	//Write codes screen_00_label_2
	ui->screen_00_label_2 = lv_label_create(ui->screen_00);
	lv_obj_set_pos(ui->screen_00_label_2, 41, 96);
	lv_obj_set_size(ui->screen_00_label_2, 190, 25);
	lv_obj_set_scrollbar_mode(ui->screen_00_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_00_label_2, "Wi-Fi PWD:88888888");
	lv_label_set_long_mode(ui->screen_00_label_2, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_00_label_2_main_main_default
	static lv_style_t style_screen_00_label_2_main_main_default;
	if (style_screen_00_label_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_00_label_2_main_main_default);
	else
		lv_style_init(&style_screen_00_label_2_main_main_default);
	lv_style_set_radius(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_00_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_00_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_00_label_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_text_color(&style_screen_00_label_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_00_label_2_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_text_align(&style_screen_00_label_2_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_00_label_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_00_label_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_00_label_2, &style_screen_00_label_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
}