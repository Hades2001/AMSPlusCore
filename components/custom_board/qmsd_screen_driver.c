#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "screen_driver.h"

extern scr_driver_t* g_lcd_driver;

#define LCD_WRITE_CMD(x)    g_lcd_driver->write_cmd(x)
#define LCD_WRITE_DATA(x)   g_lcd_driver->write_data(x)

esp_err_t qmsd_lcd_reg_config(void)
{

	vTaskDelay(120);                //ms

	LCD_WRITE_CMD(0x11);     //Sleep out

	vTaskDelay(120);                //Delay 120ms

	LCD_WRITE_CMD(0x36);     
	LCD_WRITE_DATA(0x00);  

	LCD_WRITE_CMD(0x3A);     
	LCD_WRITE_DATA(0x05);  //0x05( 65K Color) 

	LCD_WRITE_CMD(0x21);     

	LCD_WRITE_CMD(0xB2);     
	LCD_WRITE_DATA(0x05);   
	LCD_WRITE_DATA(0x05);   
	LCD_WRITE_DATA(0x00);   
	LCD_WRITE_DATA(0x33);   
	LCD_WRITE_DATA(0x33);   

	LCD_WRITE_CMD(0xB7);     
	LCD_WRITE_DATA(0x23);   

	LCD_WRITE_CMD(0xBB);     
	LCD_WRITE_DATA(0x22);   

	LCD_WRITE_CMD(0xC0);     
	LCD_WRITE_DATA(0x2C);   

	LCD_WRITE_CMD(0xC2);     
	LCD_WRITE_DATA(0x01);   

	LCD_WRITE_CMD(0xC3);     
	LCD_WRITE_DATA(0x13);   

	LCD_WRITE_CMD(0xC4);     
	LCD_WRITE_DATA(0x20);   

	LCD_WRITE_CMD(0xC6);     
	LCD_WRITE_DATA(0x0F);   

	LCD_WRITE_CMD(0xD0);     
	LCD_WRITE_DATA(0xA4);   
	LCD_WRITE_DATA(0xA1);   

	LCD_WRITE_CMD(0xD6);     
	LCD_WRITE_DATA(0xA1);   

	LCD_WRITE_CMD(0xE0);
	LCD_WRITE_DATA(0x70);
	LCD_WRITE_DATA(0x06);
	LCD_WRITE_DATA(0x0C);
	LCD_WRITE_DATA(0x08);
	LCD_WRITE_DATA(0x09);
	LCD_WRITE_DATA(0x27);
	LCD_WRITE_DATA(0x2E);
	LCD_WRITE_DATA(0x34);
	LCD_WRITE_DATA(0x46);
	LCD_WRITE_DATA(0x37);
	LCD_WRITE_DATA(0x13);
	LCD_WRITE_DATA(0x13);
	LCD_WRITE_DATA(0x25);
	LCD_WRITE_DATA(0x2A);

	LCD_WRITE_CMD(0xE1);
	LCD_WRITE_DATA(0x70);
	LCD_WRITE_DATA(0x04);
	LCD_WRITE_DATA(0x08);
	LCD_WRITE_DATA(0x09);
	LCD_WRITE_DATA(0x07);
	LCD_WRITE_DATA(0x03);
	LCD_WRITE_DATA(0x2C);
	LCD_WRITE_DATA(0x42);
	LCD_WRITE_DATA(0x42);
	LCD_WRITE_DATA(0x38);
	LCD_WRITE_DATA(0x14);
	LCD_WRITE_DATA(0x14);
	LCD_WRITE_DATA(0x27);
	LCD_WRITE_DATA(0x2C);

	LCD_WRITE_CMD(0x29);     //Display on
	/****************************************************/
	LCD_WRITE_CMD(0x2A);    //Column Address Set
	LCD_WRITE_DATA(0x00);   
	LCD_WRITE_DATA(0x34);   //52
	LCD_WRITE_DATA(0x00);   
	LCD_WRITE_DATA(0xBA);   //186

	LCD_WRITE_CMD(0x2B);    //Row Address Set
	LCD_WRITE_DATA(0x00);   
	LCD_WRITE_DATA(0x28);   //40
	LCD_WRITE_DATA(0x01);   
	LCD_WRITE_DATA(0x17);   //279

	LCD_WRITE_CMD(0x2C);

	/*****************************************************/

	LCD_WRITE_CMD(0x11);     //Sleep out
	vTaskDelay(120);                //Delay 120ms

	/*****************************************************/

	//LCD_WRITE_CMD(0x10);     //Sleep in
	//vTaskDelay(120);                //Delay 120ms

    return ESP_OK;
}