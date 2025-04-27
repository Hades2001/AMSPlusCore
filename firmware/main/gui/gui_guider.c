/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"

lv_obj_t *filament_boxlist[4];
lv_obj_t *filament_textlist[4];

void init_scr_del_flag(lv_ui *ui){
	ui->screen_del = true;
	ui->screen_01_del = true;
	ui->screen_00_del = true;
	ui->screen_21_del = true;
	ui->screen_5_del = true;
	ui->screen_31_del = true;
	ui->screen_41_del = true;
	/*
		const lv_obj_t *filament_boxlist[4] = {
		ams_ui.screen_31_box_filament_1,
		ams_ui.screen_31_box_filament_2,
		ams_ui.screen_31_box_filament_3,
		ams_ui.screen_31_box_filament_4,
	};
	const lv_obj_t *filament_textlist[4] = {
		ams_ui.screen_31_lab_filament_1,
		ams_ui.screen_31_lab_filament_2,
		ams_ui.screen_31_lab_filament_3,
		ams_ui.screen_31_lab_filament_4,
	};
	*/

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
	setup_scr_screen_41(ui);

	filament_boxlist[0] = ams_ui.screen_31_box_filament_1;
	filament_boxlist[1] = ams_ui.screen_31_box_filament_2;
	filament_boxlist[2] = ams_ui.screen_31_box_filament_3;
	filament_boxlist[3] = ams_ui.screen_31_box_filament_4;
	filament_textlist[0] = ams_ui.screen_31_lab_filament_1;
	filament_textlist[1] = ams_ui.screen_31_lab_filament_2;
	filament_textlist[2] = ams_ui.screen_31_lab_filament_3;
	filament_textlist[3] = ams_ui.screen_31_lab_filament_4;
}
