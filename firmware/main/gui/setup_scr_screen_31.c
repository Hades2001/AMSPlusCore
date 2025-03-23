/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"


void anim_y_callback(void * obj, int32_t value) {
    
    lv_obj_set_y((lv_obj_t *)obj, value);  // 设置垂直位置
    lv_obj_set_pos((lv_obj_t *)obj, 0, value);
}

void setup_scr_screen_31(lv_ui *ui){
#if CRE_TILE
	//Write codes screen_31
	ui->screen_31 = lv_obj_create(ui->screen_5_tileview_1_tile_30);
#endif
	lv_obj_set_scrollbar_mode(ui->screen_31, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_main_main_default
	static lv_style_t style_screen_31_main_main_default;
	if (style_screen_31_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_main_main_default);
	else
		lv_style_init(&style_screen_31_main_main_default);
	lv_style_set_bg_color(&style_screen_31_main_main_default, lv_color_make(0x26, 0x26, 0x26));
	lv_style_set_bg_opa(&style_screen_31_main_main_default, 255);
	lv_obj_add_style(ui->screen_31, &style_screen_31_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

#if 1
	//Write codes screen_5_cont_1
	ui->screen_5_cont_1 = lv_obj_create(ui->screen_31);
	lv_obj_set_pos(ui->screen_5_cont_1, 0, 0);//[0, -71]
	lv_obj_set_size(ui->screen_5_cont_1, 240, 206);
	lv_obj_set_scrollbar_mode(ui->screen_5_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_5_cont_1_main_main_default
	static lv_style_t style_screen_5_cont_1_main_main_default;
	if (style_screen_5_cont_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_5_cont_1_main_main_default);
	else
		lv_style_init(&style_screen_5_cont_1_main_main_default);
	lv_style_set_radius(&style_screen_5_cont_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_5_cont_1_main_main_default, lv_color_make(0x26, 0x26, 0x26));
	lv_style_set_bg_grad_color(&style_screen_5_cont_1_main_main_default, lv_color_make(0x26, 0x26, 0x26));
	lv_style_set_bg_grad_dir(&style_screen_5_cont_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_5_cont_1_main_main_default, 255);
	lv_style_set_border_color(&style_screen_5_cont_1_main_main_default, lv_color_make(0x26, 0x26, 0x26));
	lv_style_set_border_width(&style_screen_5_cont_1_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_5_cont_1_main_main_default, 0);
	lv_style_set_pad_left(&style_screen_5_cont_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_5_cont_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_5_cont_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_5_cont_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_5_cont_1, &style_screen_5_cont_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
#endif

	//Write codes screen_31_box_filament_1
	ui->screen_31_box_filament_1 = lv_bar_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_box_filament_1, 8, 0);
	lv_obj_set_size(ui->screen_31_box_filament_1, 51, 105);
	lv_obj_set_scrollbar_mode(ui->screen_31_box_filament_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_1_main_main_default
	static lv_style_t style_screen_31_box_filament_1_main_main_default;
	if (style_screen_31_box_filament_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_1_main_main_default);
	else
		lv_style_init(&style_screen_31_box_filament_1_main_main_default);
	lv_style_set_radius(&style_screen_31_box_filament_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_1_main_main_default, 60);
	lv_obj_add_style(ui->screen_31_box_filament_1, &style_screen_31_box_filament_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_1_main_indicator_default
	static lv_style_t style_screen_31_box_filament_1_main_indicator_default;
	if (style_screen_31_box_filament_1_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_1_main_indicator_default);
	else
		lv_style_init(&style_screen_31_box_filament_1_main_indicator_default);
	lv_style_set_radius(&style_screen_31_box_filament_1_main_indicator_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_1_main_indicator_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_1_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_1_main_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_1_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_31_box_filament_1, &style_screen_31_box_filament_1_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->screen_31_box_filament_1, 1000, 0);
	lv_bar_set_mode(ui->screen_31_box_filament_1, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->screen_31_box_filament_1, 50, LV_ANIM_OFF);

	//Write codes screen_31_box_filament_2
	ui->screen_31_box_filament_2 = lv_bar_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_box_filament_2, 66, 0);
	lv_obj_set_size(ui->screen_31_box_filament_2, 51, 105);
	lv_obj_set_scrollbar_mode(ui->screen_31_box_filament_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_2_main_main_default
	static lv_style_t style_screen_31_box_filament_2_main_main_default;
	if (style_screen_31_box_filament_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_2_main_main_default);
	else
		lv_style_init(&style_screen_31_box_filament_2_main_main_default);
	lv_style_set_radius(&style_screen_31_box_filament_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_2_main_main_default, lv_color_make(0x00, 0xb1, 0x4d));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_2_main_main_default, 60);
	lv_obj_add_style(ui->screen_31_box_filament_2, &style_screen_31_box_filament_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_2_main_indicator_default
	static lv_style_t style_screen_31_box_filament_2_main_indicator_default;
	if (style_screen_31_box_filament_2_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_2_main_indicator_default);
	else
		lv_style_init(&style_screen_31_box_filament_2_main_indicator_default);
	lv_style_set_radius(&style_screen_31_box_filament_2_main_indicator_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_2_main_indicator_default, lv_color_make(0x00, 0xb1, 0x4d));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_2_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_2_main_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_2_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_31_box_filament_2, &style_screen_31_box_filament_2_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->screen_31_box_filament_2, 1000, 0);
	lv_bar_set_mode(ui->screen_31_box_filament_2, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->screen_31_box_filament_2, 50, LV_ANIM_OFF);

	//Write codes screen_31_box_filament_3
	ui->screen_31_box_filament_3 = lv_bar_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_box_filament_3, 124, 0);
	lv_obj_set_size(ui->screen_31_box_filament_3, 51, 105);
	lv_obj_set_scrollbar_mode(ui->screen_31_box_filament_3, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_3_main_main_default
	static lv_style_t style_screen_31_box_filament_3_main_main_default;
	if (style_screen_31_box_filament_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_3_main_main_default);
	else
		lv_style_init(&style_screen_31_box_filament_3_main_main_default);
	lv_style_set_radius(&style_screen_31_box_filament_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_3_main_main_default, lv_color_make(0xf2, 0x4a, 0x61));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_3_main_main_default, 60);
	lv_obj_add_style(ui->screen_31_box_filament_3, &style_screen_31_box_filament_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_3_main_indicator_default
	static lv_style_t style_screen_31_box_filament_3_main_indicator_default;
	if (style_screen_31_box_filament_3_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_3_main_indicator_default);
	else
		lv_style_init(&style_screen_31_box_filament_3_main_indicator_default);
	lv_style_set_radius(&style_screen_31_box_filament_3_main_indicator_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_3_main_indicator_default, lv_color_make(0xf2, 0x4a, 0x61));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_3_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_3_main_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_3_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_31_box_filament_3, &style_screen_31_box_filament_3_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->screen_31_box_filament_3, 1000, 0);
	lv_bar_set_mode(ui->screen_31_box_filament_3, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->screen_31_box_filament_3, 50, LV_ANIM_OFF);

	//Write codes screen_31_box_filament_4
	ui->screen_31_box_filament_4 = lv_bar_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_box_filament_4, 182, 0);
	lv_obj_set_size(ui->screen_31_box_filament_4, 51, 105);
	lv_obj_set_scrollbar_mode(ui->screen_31_box_filament_4, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_4_main_main_default
	static lv_style_t style_screen_31_box_filament_4_main_main_default;
	if (style_screen_31_box_filament_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_4_main_main_default);
	else
		lv_style_init(&style_screen_31_box_filament_4_main_main_default);
	lv_style_set_radius(&style_screen_31_box_filament_4_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_4_main_main_default, lv_color_make(0x3b, 0xb7, 0xff));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_4_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_4_main_main_default, 60);
	lv_obj_add_style(ui->screen_31_box_filament_4, &style_screen_31_box_filament_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_box_filament_4_main_indicator_default
	static lv_style_t style_screen_31_box_filament_4_main_indicator_default;
	if (style_screen_31_box_filament_4_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_box_filament_4_main_indicator_default);
	else
		lv_style_init(&style_screen_31_box_filament_4_main_indicator_default);
	lv_style_set_radius(&style_screen_31_box_filament_4_main_indicator_default, 0);
	lv_style_set_bg_color(&style_screen_31_box_filament_4_main_indicator_default, lv_color_make(0x3b, 0xb7, 0xff));
	lv_style_set_bg_grad_color(&style_screen_31_box_filament_4_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_box_filament_4_main_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_box_filament_4_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_31_box_filament_4, &style_screen_31_box_filament_4_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_anim_time(ui->screen_31_box_filament_4, 1000, 0);
	lv_bar_set_mode(ui->screen_31_box_filament_4, LV_BAR_MODE_NORMAL);
	lv_bar_set_value(ui->screen_31_box_filament_4, 50, LV_ANIM_OFF);

	//Write codes screen_31_lab_filament_1
	ui->screen_31_lab_filament_1 = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_lab_filament_1, 8, 110);
	lv_obj_set_size(ui->screen_31_lab_filament_1, 51, 16);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_filament_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_filament_1, "?");
	lv_label_set_long_mode(ui->screen_31_lab_filament_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_filament_1_main_main_default
	static lv_style_t style_screen_31_lab_filament_1_main_main_default;
	if (style_screen_31_lab_filament_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_filament_1_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_filament_1_main_main_default);
	lv_style_set_radius(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_filament_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_filament_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_filament_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_filament_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_31_lab_filament_1_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_filament_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_filament_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_filament_1, &style_screen_31_lab_filament_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_31_lab_filament_2
	ui->screen_31_lab_filament_2 = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_lab_filament_2, 66, 110);
	lv_obj_set_size(ui->screen_31_lab_filament_2, 51, 16);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_filament_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_filament_2, "PETG");
	lv_label_set_long_mode(ui->screen_31_lab_filament_2, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_filament_2_main_main_default
	static lv_style_t style_screen_31_lab_filament_2_main_main_default;
	if (style_screen_31_lab_filament_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_filament_2_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_filament_2_main_main_default);
	lv_style_set_radius(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_filament_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_filament_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_filament_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_filament_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_31_lab_filament_2_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_filament_2_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_filament_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_filament_2, &style_screen_31_lab_filament_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_31_lab_filament_3
	ui->screen_31_lab_filament_3 = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_lab_filament_3, 124, 110);
	lv_obj_set_size(ui->screen_31_lab_filament_3, 51, 16);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_filament_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_filament_3, "PETG");
	lv_label_set_long_mode(ui->screen_31_lab_filament_3, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_filament_3_main_main_default
	static lv_style_t style_screen_31_lab_filament_3_main_main_default;
	if (style_screen_31_lab_filament_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_filament_3_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_filament_3_main_main_default);

	lv_style_set_radius(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_filament_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_filament_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_filament_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_filament_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_31_lab_filament_3_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_filament_3_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_filament_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_filament_3, &style_screen_31_lab_filament_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_31_lab_filament_4
	ui->screen_31_lab_filament_4 = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_lab_filament_4, 182, 110);
	lv_obj_set_size(ui->screen_31_lab_filament_4, 51, 16);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_filament_4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_filament_4, "PETG");
	lv_label_set_long_mode(ui->screen_31_lab_filament_4, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_filament_4_main_main_default
	static lv_style_t style_screen_31_lab_filament_4_main_main_default;
	if (style_screen_31_lab_filament_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_filament_4_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_filament_4_main_main_default);
	lv_style_set_radius(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_filament_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_filament_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_filament_4_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_filament_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_31_lab_filament_4_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_filament_4_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_filament_4_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_filament_4, &style_screen_31_lab_filament_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_31_img_1
	ui->screen_31_img_1 = lv_img_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_img_1, 20, 40);
	lv_obj_set_size(ui->screen_31_img_1, 26, 26);
	lv_obj_set_scrollbar_mode(ui->screen_31_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_img_1_main_main_default
	static lv_style_t style_screen_31_img_1_main_main_default;
	if (style_screen_31_img_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_img_1_main_main_default);
	else
		lv_style_init(&style_screen_31_img_1_main_main_default);
	lv_style_set_img_recolor(&style_screen_31_img_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_31_img_1_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_31_img_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_31_img_1, &style_screen_31_img_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_31_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_31_img_1,&_count_26x26);
	lv_img_set_pivot(ui->screen_31_img_1, 50,50);
	lv_img_set_angle(ui->screen_31_img_1, 0);

	//Write codes screen_31_img_2
	ui->screen_31_img_2 = lv_img_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_img_2, 78, 40);
	lv_obj_set_size(ui->screen_31_img_2, 26, 26);
	lv_obj_set_scrollbar_mode(ui->screen_31_img_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_img_2_main_main_default
	static lv_style_t style_screen_31_img_2_main_main_default;
	if (style_screen_31_img_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_img_2_main_main_default);
	else
		lv_style_init(&style_screen_31_img_2_main_main_default);
	lv_style_set_img_recolor(&style_screen_31_img_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_31_img_2_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_31_img_2_main_main_default, 255);
	lv_obj_add_style(ui->screen_31_img_2, &style_screen_31_img_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_31_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_31_img_2,&_count_26x26);
	lv_img_set_pivot(ui->screen_31_img_2, 50,50);
	lv_img_set_angle(ui->screen_31_img_2, 0);

	//Write codes screen_31_img_3
	ui->screen_31_img_3 = lv_img_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_img_3, 136, 40);
	lv_obj_set_size(ui->screen_31_img_3, 26, 26);
	lv_obj_set_scrollbar_mode(ui->screen_31_img_3, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_img_3_main_main_default
	static lv_style_t style_screen_31_img_3_main_main_default;
	if (style_screen_31_img_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_img_3_main_main_default);
	else
		lv_style_init(&style_screen_31_img_3_main_main_default);
	lv_style_set_img_recolor(&style_screen_31_img_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_31_img_3_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_31_img_3_main_main_default, 255);
	lv_obj_add_style(ui->screen_31_img_3, &style_screen_31_img_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_31_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_31_img_3,&_count_26x26);
	lv_img_set_pivot(ui->screen_31_img_3, 50,50);
	lv_img_set_angle(ui->screen_31_img_3, 0);

	//Write codes screen_31_img_4
	ui->screen_31_img_4 = lv_img_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_img_4, 194, 40);
	lv_obj_set_size(ui->screen_31_img_4, 26, 26);
	lv_obj_set_scrollbar_mode(ui->screen_31_img_4, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_img_4_main_main_default
	static lv_style_t style_screen_31_img_4_main_main_default;
	if (style_screen_31_img_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_img_4_main_main_default);
	else
		lv_style_init(&style_screen_31_img_4_main_main_default);
	lv_style_set_img_recolor(&style_screen_31_img_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_31_img_4_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_31_img_4_main_main_default, 255);
	lv_obj_add_style(ui->screen_31_img_4, &style_screen_31_img_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_31_img_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_31_img_4,&_count_26x26);
	lv_img_set_pivot(ui->screen_31_img_4, 50,50);
	lv_img_set_angle(ui->screen_31_img_4, 0);

	//Write codes screen_31_img_wifi_on
	ui->screen_31_img_wifi_on = lv_img_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_img_wifi_on, 8, 148);
	lv_obj_set_size(ui->screen_31_img_wifi_on, 20, 16);
	lv_obj_set_scrollbar_mode(ui->screen_31_img_wifi_on, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_img_wifi_on_main_main_default
	static lv_style_t style_screen_31_img_wifi_on_main_main_default;
	if (style_screen_31_img_wifi_on_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_img_wifi_on_main_main_default);
	else
		lv_style_init(&style_screen_31_img_wifi_on_main_main_default);
	lv_style_set_img_recolor(&style_screen_31_img_wifi_on_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_31_img_wifi_on_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_31_img_wifi_on_main_main_default, 255);
	lv_obj_add_style(ui->screen_31_img_wifi_on, &style_screen_31_img_wifi_on_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_31_img_wifi_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_31_img_wifi_on,&_wifi_s_20x16);
	lv_img_set_pivot(ui->screen_31_img_wifi_on, 50,50);
	lv_img_set_angle(ui->screen_31_img_wifi_on, 0);

	//Write codes screen_31_img_connect_on
	ui->screen_31_img_connect_on = lv_img_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_img_connect_on, 10, 176);
	lv_obj_set_size(ui->screen_31_img_connect_on, 18, 17);
	lv_obj_set_scrollbar_mode(ui->screen_31_img_connect_on, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_img_connect_on_main_main_default
	static lv_style_t style_screen_31_img_connect_on_main_main_default;
	if (style_screen_31_img_connect_on_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_img_connect_on_main_main_default);
	else
		lv_style_init(&style_screen_31_img_connect_on_main_main_default);
	lv_style_set_img_recolor(&style_screen_31_img_connect_on_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_31_img_connect_on_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_31_img_connect_on_main_main_default, 255);
	lv_obj_add_style(ui->screen_31_img_connect_on, &style_screen_31_img_connect_on_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_31_img_connect_on, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_31_img_connect_on,&_icon_18x17);
	lv_img_set_pivot(ui->screen_31_img_connect_on, 50,50);
	lv_img_set_angle(ui->screen_31_img_connect_on, 0);

	//Write codes screen_31_label_5
	ui->screen_31_label_5 = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_label_5, 34, 135);
	lv_obj_set_size(ui->screen_31_label_5, 206, 71);
	lv_obj_set_scrollbar_mode(ui->screen_31_label_5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_label_5, "");
	lv_label_set_long_mode(ui->screen_31_label_5, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_label_5_main_main_default
	static lv_style_t style_screen_31_label_5_main_main_default;
	if (style_screen_31_label_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_label_5_main_main_default);
	else
		lv_style_init(&style_screen_31_label_5_main_main_default);
	lv_style_set_radius(&style_screen_31_label_5_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_label_5_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_31_label_5_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_label_5_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_label_5_main_main_default, 255);
	lv_style_set_text_color(&style_screen_31_label_5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_31_label_5_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_31_label_5_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_label_5_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_31_label_5_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_label_5_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_label_5_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_31_label_5_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_label_5, &style_screen_31_label_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_31_lab_hum
	ui->screen_31_lab_hum = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_lab_hum, 50, 130);
	lv_obj_set_size(ui->screen_31_lab_hum, 140, 70);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_hum, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_hum, "88%");
	lv_label_set_long_mode(ui->screen_31_lab_hum, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_hum_main_main_default
	static lv_style_t style_screen_31_lab_hum_main_main_default;
	if (style_screen_31_lab_hum_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_hum_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_hum_main_main_default);
	lv_style_set_radius(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_hum_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_hum_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_hum_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_hum_main_main_default, lv_color_make(0x83, 0xDD, 0xFF));
	lv_style_set_text_font(&style_screen_31_lab_hum_main_main_default, &lv_customer_font_Teko_Regular_70);
	lv_style_set_text_letter_space(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_hum_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_hum_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_hum_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_hum, &style_screen_31_lab_hum_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_31_lab_temp
	ui->screen_31_lab_temp = lv_label_create(ui->screen_5_cont_1);
	lv_obj_set_pos(ui->screen_31_lab_temp, 164, 145);
	lv_obj_set_size(ui->screen_31_lab_temp, 100, 35);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_temp, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_temp, "25°C");
	lv_label_set_long_mode(ui->screen_31_lab_temp, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_temp_main_main_default
	static lv_style_t style_screen_31_lab_temp_main_main_default;
	if (style_screen_31_lab_temp_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_temp_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_temp_main_main_default);
	lv_style_set_radius(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_temp_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_temp_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_temp_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_temp_main_main_default, lv_color_make(0xa7, 0xa7, 0xa7));
	lv_style_set_text_font(&style_screen_31_lab_temp_main_main_default, &lv_customer_font_Teko_Regular_35);
	lv_style_set_text_letter_space(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_temp_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_temp_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_temp_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_temp, &style_screen_31_lab_temp_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	ui->screen_31_lab_info = lv_label_create(ui->screen_5_cont_1);

	lv_obj_set_pos(ui->screen_31_lab_info, 164, 175);
	lv_obj_set_size(ui->screen_31_lab_info, 70, 18);
	lv_obj_set_scrollbar_mode(ui->screen_31_lab_info, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_31_lab_info, "192.168.0.1");
	lv_label_set_long_mode(ui->screen_31_lab_info, LV_LABEL_LONG_SCROLL);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_lab_info_main_main_default
	static lv_style_t style_screen_31_lab_info_main_main_default;
	if (style_screen_31_lab_info_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_31_lab_info_main_main_default);
	else
		lv_style_init(&style_screen_31_lab_info_main_main_default);

	lv_style_set_radius(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_31_lab_info_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_31_lab_info_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_31_lab_info_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_text_color(&style_screen_31_lab_info_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_31_lab_info_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_text_align(&style_screen_31_lab_info_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_31_lab_info_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_31_lab_info_main_main_default, 0);
	lv_obj_add_style(ui->screen_31_lab_info, &style_screen_31_lab_info_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	////Write codes screen_31_bar_hum
	//ui->screen_31_bar_hum = lv_bar_create(ui->screen_5_cont_1);
	//lv_obj_set_pos(ui->screen_31_bar_hum, 165, 183);
	//lv_obj_set_size(ui->screen_31_bar_hum, 70, 12);
	//lv_obj_set_scrollbar_mode(ui->screen_31_bar_hum, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_31_bar_hum_main_main_default
	//static lv_style_t style_screen_31_bar_hum_main_main_default;
	//if (style_screen_31_bar_hum_main_main_default.prop_cnt > 1)
	//	lv_style_reset(&style_screen_31_bar_hum_main_main_default);
	//else
	//	lv_style_init(&style_screen_31_bar_hum_main_main_default);
	//lv_style_set_radius(&style_screen_31_bar_hum_main_main_default, 5);
	//lv_style_set_bg_color(&style_screen_31_bar_hum_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	//lv_style_set_bg_grad_color(&style_screen_31_bar_hum_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	//lv_style_set_bg_grad_dir(&style_screen_31_bar_hum_main_main_default, LV_GRAD_DIR_NONE);
	//lv_style_set_bg_opa(&style_screen_31_bar_hum_main_main_default, 255);
	//lv_obj_add_style(ui->screen_31_bar_hum, &style_screen_31_bar_hum_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	////Write style state: LV_STATE_DEFAULT for style_screen_31_bar_hum_main_indicator_default
	//static lv_style_t style_screen_31_bar_hum_main_indicator_default;
	//if (style_screen_31_bar_hum_main_indicator_default.prop_cnt > 1)
	//	lv_style_reset(&style_screen_31_bar_hum_main_indicator_default);
	//else
	//	lv_style_init(&style_screen_31_bar_hum_main_indicator_default);
	//lv_style_set_radius(&style_screen_31_bar_hum_main_indicator_default, 5);
	//lv_style_set_bg_color(&style_screen_31_bar_hum_main_indicator_default, lv_color_make(0x00, 0x61, 0xF3));
	//lv_style_set_bg_grad_color(&style_screen_31_bar_hum_main_indicator_default, lv_color_make(0x83, 0xDD, 0xFF));
	//lv_style_set_bg_grad_dir(&style_screen_31_bar_hum_main_indicator_default, LV_GRAD_DIR_NONE);
	//lv_style_set_bg_opa(&style_screen_31_bar_hum_main_indicator_default, 255);
	//lv_obj_add_style(ui->screen_31_bar_hum, &style_screen_31_bar_hum_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	//lv_obj_set_style_anim_time(ui->screen_31_bar_hum, 1000, 0);
	//lv_bar_set_mode(ui->screen_31_bar_hum, LV_BAR_MODE_NORMAL);
	//lv_bar_set_value(ui->screen_31_bar_hum, 50, LV_ANIM_OFF);
	//lv_bar_set_range(ui->screen_31_bar_hum, -20, 80);

    lv_anim_init(&ui->screen_31_anim);
    lv_anim_set_var(&ui->screen_31_anim, ui->screen_5_cont_1);
    lv_anim_set_time(&ui->screen_31_anim, 200);          
    lv_anim_set_exec_cb(&ui->screen_31_anim, anim_y_callback); 
    lv_anim_set_path_cb(&ui->screen_31_anim, lv_anim_path_ease_in_out);
}