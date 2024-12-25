/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void init_scr_del_flag(lv_ui *ui){
	ui->screen_del = true;
	ui->screen_01_del = true;
	ui->screen_00_del = true;
	ui->screen_21_del = true;
	ui->screen_5_del = true;
	ui->screen_31_del = true;
}

void setup_ui(lv_ui *ui){
	// init_scr_del_flag(ui);
	// setup_scr_screen(ui);
	// lv_scr_load(ui->screen);
	
	setup_scr_screen_5(ui);
	//lv_scr_load(ui->screen_5);
	setup_scr_screen_00(ui);
	setup_scr_screen_01(ui);
	setup_scr_screen_11(ui);
	setup_scr_screen_21(ui);
	setup_scr_screen_31(ui);
}
