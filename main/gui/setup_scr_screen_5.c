/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"



void setup_scr_screen_5(lv_ui *ui){

	//Write codes screen_5
	ui->screen_5 = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_5, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_5_main_main_default
	static lv_style_t style_screen_5_main_main_default;
	if (style_screen_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_5_main_main_default);
	else
		lv_style_init(&style_screen_5_main_main_default);
	lv_style_set_bg_color(&style_screen_5_main_main_default, lv_color_make(0x1c, 0x1c, 0x1c));
	lv_style_set_bg_opa(&style_screen_5_main_main_default, 255);
	lv_obj_add_style(ui->screen_5, &style_screen_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_5_tileview_1
	ui->screen_5_tileview_1 = lv_tileview_create(ui->screen_5);
	lv_obj_set_pos(ui->screen_5_tileview_1, 0, 0);
	lv_obj_set_size(ui->screen_5_tileview_1, 240, 135);
	lv_obj_set_scrollbar_mode(ui->screen_5_tileview_1, LV_SCROLLBAR_MODE_ON);

	//Write style state: LV_STATE_DEFAULT for style_screen_5_tileview_1_main_main_default
	static lv_style_t style_screen_5_tileview_1_main_main_default;
	if (style_screen_5_tileview_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_5_tileview_1_main_main_default);
	else
		lv_style_init(&style_screen_5_tileview_1_main_main_default);
	lv_style_set_radius(&style_screen_5_tileview_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_5_tileview_1_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_5_tileview_1_main_main_default, lv_color_make(0xf6, 0xf6, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_5_tileview_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_5_tileview_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_5_tileview_1, &style_screen_5_tileview_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_5_tileview_1_main_scrollbar_default
	static lv_style_t style_screen_5_tileview_1_main_scrollbar_default;
	if (style_screen_5_tileview_1_main_scrollbar_default.prop_cnt > 1)
		lv_style_reset(&style_screen_5_tileview_1_main_scrollbar_default);
	else
		lv_style_init(&style_screen_5_tileview_1_main_scrollbar_default);
	lv_style_set_radius(&style_screen_5_tileview_1_main_scrollbar_default, 0);
	lv_style_set_bg_color(&style_screen_5_tileview_1_main_scrollbar_default, lv_color_make(0xea, 0xef, 0xf3));
	lv_style_set_bg_opa(&style_screen_5_tileview_1_main_scrollbar_default, 255);
	lv_obj_add_style(ui->screen_5_tileview_1, &style_screen_5_tileview_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT  );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_CHECKED  );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_FOCUSED  );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_FOCUS_KEY);
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_EDITED   );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_HOVERED  );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_PRESSED  );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_SCROLLED );
	lv_obj_set_style_bg_opa(ui->screen_5_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DISABLED );

#if CRE_TILE
	//add new tile screen_5_tileview_1_tile_00
	ui->screen_5_tileview_1_tile_00 = lv_tileview_add_tile(ui->screen_5_tileview_1, 0 , 0, LV_DIR_RIGHT | LV_DIR_TOP);
	//add new tile screen_5_tileview_1_tile_01
	ui->screen_5_tileview_1_tile_01 = lv_tileview_add_tile(ui->screen_5_tileview_1, 0 , 1, LV_DIR_BOTTOM);
	//add new tile screen_5_tileview_1_tile_10
	ui->screen_5_tileview_1_tile_10 = lv_tileview_add_tile(ui->screen_5_tileview_1, 1 , 0, LV_DIR_LEFT | LV_DIR_RIGHT);
	//add new tile screen_5_tileview_1_tile_20
	ui->screen_5_tileview_1_tile_20 = lv_tileview_add_tile(ui->screen_5_tileview_1, 2 , 0, LV_DIR_LEFT | LV_DIR_RIGHT);
	//add new tile screen_5_tileview_1_tile_30
	ui->screen_5_tileview_1_tile_30 = lv_tileview_add_tile(ui->screen_5_tileview_1, 3, 0, LV_DIR_RIGHT);
#else

	ui->screen_11 = lv_tileview_add_tile(ui->screen_5_tileview_1, 0 , 0, LV_DIR_ALL);

	ui->screen_21 = lv_tileview_add_tile(ui->screen_5_tileview_1, 1 , 0, LV_DIR_ALL);

	ui->screen_01 = lv_tileview_add_tile(ui->screen_5_tileview_1, 1 , 2, LV_DIR_ALL);

	ui->screen_00 = lv_tileview_add_tile(ui->screen_5_tileview_1, 1 , 1, LV_DIR_ALL);
	
	ui->screen_31 = lv_tileview_add_tile(ui->screen_5_tileview_1, 2 , 0, LV_DIR_ALL);
	
#endif
}
