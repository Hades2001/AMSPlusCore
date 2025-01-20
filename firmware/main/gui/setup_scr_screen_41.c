/*
 * Copyright 2025 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"


void setup_scr_screen_41(lv_ui *ui){
#if CRE_TILE
	//Write codes screen_41
	ui->screen_41 = lv_obj_create(ui->screen_5_tileview_1_tile_01);
#endif
	lv_obj_set_scrollbar_mode(ui->screen_41, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_41_main_main_default
	static lv_style_t style_screen_41_main_main_default;
	if (style_screen_41_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_41_main_main_default);
	else
		lv_style_init(&style_screen_41_main_main_default);
	lv_style_set_bg_color(&style_screen_41_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_bg_opa(&style_screen_41_main_main_default, 255);
	lv_obj_add_style(ui->screen_41, &style_screen_41_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_41_img_ota
	ui->screen_41_img_ota = lv_img_create(ui->screen_41);
	lv_obj_set_pos(ui->screen_41_img_ota, 104, 25);
	lv_obj_set_size(ui->screen_41_img_ota, 32, 32);
	lv_obj_set_scrollbar_mode(ui->screen_41_img_ota, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_41_img_ota_main_main_default
	static lv_style_t style_screen_41_img_ota_main_main_default;
	if (style_screen_41_img_ota_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_41_img_ota_main_main_default);
	else
		lv_style_init(&style_screen_41_img_ota_main_main_default);
	lv_style_set_img_recolor(&style_screen_41_img_ota_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_41_img_ota_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_41_img_ota_main_main_default, 255);
	lv_obj_add_style(ui->screen_41_img_ota, &style_screen_41_img_ota_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_41_img_ota, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_41_img_ota,&_ota_32x32);
	lv_img_set_pivot(ui->screen_41_img_ota, 50,50);
	lv_img_set_angle(ui->screen_41_img_ota, 0);

	//Write codes screen_41_label_ota
	ui->screen_41_label_ota = lv_label_create(ui->screen_41);
	lv_obj_set_pos(ui->screen_41_label_ota, 0, 95);
	lv_obj_set_size(ui->screen_41_label_ota, 240, 25);
	lv_obj_set_scrollbar_mode(ui->screen_41_label_ota, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_41_label_ota, "OTA update is in progress");
	lv_label_set_long_mode(ui->screen_41_label_ota, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_41_label_ota_main_main_default
	static lv_style_t style_screen_41_label_ota_main_main_default;
	if (style_screen_41_label_ota_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_41_label_ota_main_main_default);
	else
		lv_style_init(&style_screen_41_label_ota_main_main_default);
	lv_style_set_radius(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_41_label_ota_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_41_label_ota_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_41_label_ota_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_text_color(&style_screen_41_label_ota_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_41_label_ota_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_text_align(&style_screen_41_label_ota_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_41_label_ota_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_41_label_ota_main_main_default, 0);
	lv_obj_add_style(ui->screen_41_label_ota, &style_screen_41_label_ota_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_41_bar_ota
	ui->screen_41_bar_ota = lv_bar_create(ui->screen_41);
	lv_obj_set_pos(ui->screen_41_bar_ota, 28, 71);
	lv_obj_set_size(ui->screen_41_bar_ota, 184, 10);
	lv_obj_set_scrollbar_mode(ui->screen_41_bar_ota, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_41_bar_ota_main_main_default
	static lv_style_t style_screen_41_bar_ota_main_main_default;
	if (style_screen_41_bar_ota_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_41_bar_ota_main_main_default);
	else
		lv_style_init(&style_screen_41_bar_ota_main_main_default);
	lv_style_set_radius(&style_screen_41_bar_ota_main_main_default, 10);
	lv_style_set_bg_color(&style_screen_41_bar_ota_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_41_bar_ota_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_41_bar_ota_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_41_bar_ota_main_main_default, 255);
	lv_obj_add_style(ui->screen_41_bar_ota, &style_screen_41_bar_ota_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_41_bar_ota_main_indicator_default
	static lv_style_t style_screen_41_bar_ota_main_indicator_default;
	if (style_screen_41_bar_ota_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_41_bar_ota_main_indicator_default);
	else
		lv_style_init(&style_screen_41_bar_ota_main_indicator_default);
	lv_style_set_radius(&style_screen_41_bar_ota_main_indicator_default, 10);
	lv_style_set_bg_color(&style_screen_41_bar_ota_main_indicator_default, lv_color_make(0x00, 0x77, 0xff));
	lv_style_set_bg_grad_color(&style_screen_41_bar_ota_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_41_bar_ota_main_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_41_bar_ota_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_41_bar_ota, &style_screen_41_bar_ota_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->screen_41_bar_ota, 1000, 0);
	lv_bar_set_mode(ui->screen_41_bar_ota, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->screen_41_bar_ota, 0, LV_ANIM_OFF);
}