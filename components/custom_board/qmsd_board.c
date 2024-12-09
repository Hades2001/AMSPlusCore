#include "string.h"
#include "qmsd_board.h"
#include "qmsd_utils.h"
#include "screen_utility.h"
#include "esp_log.h"
#include "hal/spi_ll.h"
#include "qmsd_lcd_wrapper.h"
#include "led_strip.h"
#include "aw9523.h"

#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "i2c_device.h"

#define TAG "QMSD_BOARD"

scr_driver_t* g_lcd_driver = &lcd_st7789_default_driver;

static touch_panel_driver_t* g_touch_driver = &touch_ft5x06_driver;
static qmsd_board_config_t g_board_config;
SemaphoreHandle_t fileSystem_init;

void qmsd_board_init_touch();

void qmsd_board_init(qmsd_board_config_t* config) {

    memcpy(&g_board_config, config, sizeof(qmsd_board_config_t));

    if (BOARD_RESET_PIN > -1) {
        gpio_pad_select_gpio(BOARD_RESET_PIN);
        gpio_set_direction(BOARD_RESET_PIN, GPIO_MODE_OUTPUT);
        gpio_set_level(BOARD_RESET_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(2));
        gpio_set_level(BOARD_RESET_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(120));
    }

    qmsd_board_init_screen();
    if (g_board_config.touch.en) {
        qmsd_board_init_touch();
    }

}

void qmsd_board_init_screen() {
    scr_dir_t screen_dir[] = { QMSD_SCREEN_DIR_0, QMSD_SCREEN_DIR_90, QMSD_SCREEN_DIR_180, QMSD_SCREEN_DIR_270};

    spi_config_t bus_conf = {
        .miso_io_num = LCD_MISO_PIN,
        .mosi_io_num = LCD_MOSI_PIN,
        .sclk_io_num = LCD_CLK_PIN,
        .max_transfer_sz = QMSD_SCREEN_WIDTH * QMSD_SCREEN_HIGHT * 2 + 10,
    };
    spi_bus_handle_t spi_bus = spi_bus_create(SPI2_HOST, &bus_conf);

    scr_interface_spi_config_t spi_lcd_cfg = {
        .spi_bus = spi_bus,
        .pin_num_cs = LCD_CS_PIN,
        .pin_num_dc = LCD_DC_PIN,
        .clk_freq = QMSD_SCREEN_CLK_SPEED,
        .swap_data = true,
    };
    scr_interface_driver_t *iface_drv;
    scr_interface_create(SCREEN_IFACE_SPI, &spi_lcd_cfg, &iface_drv);

    extern esp_err_t qmsd_lcd_reg_config(void);
    g_lcd_driver->init_reg = qmsd_lcd_reg_config;

    scr_controller_config_t lcd_cfg = {
        .interface_drv = iface_drv,
        .pin_num_rst = LCD_RST_PIN,
        .pin_num_bckl = -1,
        .rst_active_level = 0,
        .bckl_active_level = 1,
        .offset_hor = 52,
        .offset_ver = 40,
        .width = QMSD_SCREEN_WIDTH,
        .height = QMSD_SCREEN_HIGHT,
        .rotate = screen_dir[g_board_config.board_dir],
    };
    g_lcd_driver->init(&lcd_cfg);
    if (g_board_config.gui.flags.avoid_te) {
        lcd_dirver_wrapper(LCD_TE_PIN, g_lcd_driver, &lcd_cfg);
    }
}

void qmsd_board_init_touch() {
    touch_panel_dir_t touch_dir[] = {QMSD_TOUCH_DIR_0, QMSD_TOUCH_DIR_90, QMSD_TOUCH_DIR_180, QMSD_TOUCH_DIR_270};
    touch_panel_config_t touch_config = {
        .sda_pin = BOARD_I2C_SDA_PIN,
        .scl_pin = BOARD_I2C_SCL_PIN,
        .rst_pin = -1,
        .intr_pin = -1,
        .i2c_num = I2C_NUM_0,
        .i2c_freq = g_board_config.touch.i2c_freq,
        .task_en = g_board_config.touch.update_task.en,
        .task_priority = g_board_config.touch.update_task.priority,
        .task_core = g_board_config.touch.update_task.core,
        .task_stack_size = g_board_config.touch.update_task.stack_size,
        .width = QMSD_SCREEN_WIDTH,
        .height = QMSD_SCREEN_HIGHT,
        .direction = touch_dir[g_board_config.board_dir],
    };
    touch_init(g_touch_driver, &touch_config);
}

scr_driver_t* qmsd_board_get_screen_driver() {
    return g_lcd_driver;
}

void screen_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    uint16_t limit_h = (SPI_LL_DMA_MAX_BIT_LEN / 8 - 128) / (w * 2);
    uint16_t nums = h / limit_h;
    for (uint8_t i = 0; i < nums; i++) {
        g_lcd_driver->draw_bitmap(x, y + i * limit_h, w, limit_h, bitmap + w * limit_h * i);
    }
    if (h % limit_h) {
        g_lcd_driver->draw_bitmap(x, y + nums * limit_h, w, h % limit_h, bitmap + w * limit_h * nums);
    }
}

void screen_draw_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color) {
    uint32_t buffsize = w*h;
    uint16_t *color_buff = malloc(buffsize*sizeof(uint16_t));
    if(color_buff == NULL) return;
    for (uint32_t i = 0; i < buffsize; i++)
    {
        color_buff[i] = color;
    }
    
    uint16_t limit_h = (SPI_LL_DMA_MAX_BIT_LEN / 8 - 128) / (w * 2);
    uint16_t nums = h / limit_h;
    for (uint8_t i = 0; i < nums; i++) {
        g_lcd_driver->draw_bitmap(x, y + i * limit_h, w, limit_h, color_buff);
    }
    if (h % limit_h) {
        g_lcd_driver->draw_bitmap(x, y + nums * limit_h, w, h % limit_h, color_buff);
    }

    free(color_buff);
}

inline void screen_draw_pixel(uint16_t x, uint16_t y,uint16_t color){
    g_lcd_driver->draw_pixel(x,y,color);
}