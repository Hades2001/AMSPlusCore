/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"

void qrcode_create(lv_ui* ui);

void setup_scr_screen_01(lv_ui *ui){
#if CRE_TILE
	//Write codes screen_01
	ui->screen_01 = lv_obj_create(ui->screen_5_tileview_1_tile_00);
#endif	
	lv_obj_set_scrollbar_mode(ui->screen_01, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_01_main_main_default
	static lv_style_t style_screen_01_main_main_default;
	if (style_screen_01_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_01_main_main_default);
	else
		lv_style_init(&style_screen_01_main_main_default);
	lv_style_set_bg_color(&style_screen_01_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_bg_opa(&style_screen_01_main_main_default, 255);
	lv_obj_add_style(ui->screen_01, &style_screen_01_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_01_label_1
	ui->screen_01_label_1 = lv_label_create(ui->screen_01);
	lv_obj_set_pos(ui->screen_01_label_1, 22, 95);
	lv_obj_set_size(ui->screen_01_label_1, 200, 30);
	lv_obj_set_scrollbar_mode(ui->screen_01_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_01_label_1, SERVER_URL);
	lv_label_set_long_mode(ui->screen_01_label_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_01_label_1_main_main_default
	static lv_style_t style_screen_01_label_1_main_main_default;
	if (style_screen_01_label_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_01_label_1_main_main_default);
	else
		lv_style_init(&style_screen_01_label_1_main_main_default);
	lv_style_set_radius(&style_screen_01_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_01_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_01_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_01_label_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_01_label_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_01_label_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_01_label_1_main_main_default, &lv_font_Teko_Regular_17);
	lv_style_set_text_letter_space(&style_screen_01_label_1_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_01_label_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_01_label_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_01_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_01_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_01_label_1_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_01_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_01_label_1, &style_screen_01_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_01_lab_url
	ui->screen_01_lab_url = lv_label_create(ui->screen_01);
	lv_obj_set_pos(ui->screen_01_lab_url, 78, 14);
	lv_obj_set_size(ui->screen_01_lab_url, 84, 84);
	lv_obj_set_scrollbar_mode(ui->screen_01_lab_url, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_01_lab_url, "");
	lv_label_set_long_mode(ui->screen_01_lab_url, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_01_lab_url_main_main_default
	static lv_style_t style_screen_01_lab_url_main_main_default;
	if (style_screen_01_lab_url_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_01_lab_url_main_main_default);
	else
		lv_style_init(&style_screen_01_lab_url_main_main_default);
	lv_style_set_radius(&style_screen_01_lab_url_main_main_default, 10);
	lv_style_set_bg_color(&style_screen_01_lab_url_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_01_lab_url_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_01_lab_url_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_01_lab_url_main_main_default, 255);
	lv_style_set_text_color(&style_screen_01_lab_url_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_01_lab_url_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_01_lab_url_main_main_default, 0);
	lv_style_set_text_align(&style_screen_01_lab_url_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_01_lab_url_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_01_lab_url_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_01_lab_url_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_01_lab_url_main_main_default, 0);
	lv_obj_add_style(ui->screen_01_lab_url, &style_screen_01_lab_url_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	qrcode_create(ui);
}

void qrcode_create(lv_ui* ui)
{
	lv_color_t bg_color = lv_color_hex(0xffffff);
	lv_color_t fg_color = lv_color_hex(0x000000);

    ui->qr_code = lv_qrcode_create(ui->screen_01, 68, fg_color, bg_color);
	lv_obj_set_pos(ui->qr_code, 81, 17);
	
	/*Set data*/
	const char * data = SERVER_URL;
	lv_qrcode_update(ui->qr_code, data, strlen(data));
	
	/*Add a border with bg_color*/
	lv_obj_set_style_border_color(ui->qr_code, bg_color, 0);
	lv_obj_set_style_border_width(ui->qr_code, 5, 0);
}
