/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"



void setup_scr_screen_11(lv_ui *ui){
#if CRE_TILE
	//Write codes screen_11
	ui->screen_11 = lv_obj_create(ui->screen_11_5_tileview_1_tile_10);
#endif
	lv_obj_set_scrollbar_mode(ui->screen_11, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_11_main_main_default
	static lv_style_t style_screen_11_main_main_default;
	if (style_screen_11_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_11_main_main_default);
	else
		lv_style_init(&style_screen_11_main_main_default);
	lv_style_set_bg_color(&style_screen_11_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_bg_opa(&style_screen_11_main_main_default, 255);
	lv_obj_add_style(ui->screen_11, &style_screen_11_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_11_label_1
	ui->screen_11_label_1 = lv_label_create(ui->screen_11);
	lv_obj_set_pos(ui->screen_11_label_1, 50, 44);
	lv_obj_set_size(ui->screen_11_label_1, 140, 40);
	lv_obj_set_scrollbar_mode(ui->screen_11_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_11_label_1, "AMS+");
	lv_label_set_long_mode(ui->screen_11_label_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_11_label_1_main_main_default
	static lv_style_t style_screen_11_label_1_main_main_default;
	if (style_screen_11_label_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_11_label_1_main_main_default);
	else
		lv_style_init(&style_screen_11_label_1_main_main_default);
	lv_style_set_radius(&style_screen_11_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_11_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_11_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_11_label_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_11_label_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_11_label_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	// lv_style_set_text_font(&style_screen_11_label_1_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_font(&style_screen_11_label_1_main_main_default, &lv_customer_font_Teko_Regular_35);
	lv_style_set_text_letter_space(&style_screen_11_label_1_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_11_label_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_11_label_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_11_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_11_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_11_label_1_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_11_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_11_label_1, &style_screen_11_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
}
