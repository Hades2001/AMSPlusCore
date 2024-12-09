# SDK Startup Process

Init Start -> Pull the reset pin -> Initialize the screen -> Initialize the touch -> Initialize the UI driver -> Start the thread to update the UI -> Finish Init

Startup code: components/qmsd_board/xxx/xxx/qmsd_board.c, can be modified according to requirements

# Configurable Options

The SDK has default initialization values that can be modified by the user. The modifications can be made through ***menuconfig->QMSD Hal*** for configuration or by modifying the initialization code. The following are the configurable options:

## Screen Orientation

Default orientation: **BOARD_ROTATION_0**, can be configured as **BOARD_ROTATION_90**, **BOARD_ROTATION_180**, **BOARD_ROTATION_270**. Corresponding initialization code:

```
qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
config.board_dir = BOARD_ROTATION_0;
qmsd_board_init(&config);
```
> [!WARNING]
> RGB screens currently do not support rotation.

## Backlight

Default brightness is 100, delay time is 200ms. Initialization code can be modified as follows:

```
qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
config.backlight.value = 100.0;
config.backlight.delay_ms = 200; // Custom time
qmsd_board_init(&config);
```

Backlight settings in the program:

```
#include "qmsd_board.h"

// light: 0.0 ~ 100.0
void qmsd_board_backlight_set(float light);

// Delay the backlight update to avoid screen flickering during startup
void qmsd_board_backlight_set_delay(float light, uint32_t delay_ms);
```

## GUI

Supports ***LVGL-8.3*** and ***LVGL-7.11***, with ***LVGL-8.3*** being the default.

Switching between LVGL-8.3 and LVGL-7.11:

***menuconfig → Component config → LVGL configuration → Select UI_ENGINE***

GUI itself has configurable options:

***menuconfig → Component config → LVGL configuration → LVGL configuration***

Configurable options:

**config.gui.en** -> 1: Initialize GUI during init, 0: Do not initialize GUI (can be disabled when using other unsupported UI drivers)

**config.gui.buffer_size** -> byte, the size of a single buffer in GUI, 0: full-screen buffer

**config.gui.update_task** -> GUI update thread configuration, can adjust priority, thread size, and enable/disable. If disabled, GUI update events need to be manually called.

**config.gui.refresh_task** -> Screen refresh thread, screen drawing functions are placed in a separate thread for refreshing, does not block GUI calculations and drawing. In single-buffer mode, this option is ineffective.

**config.gui.flags.double_fb** -> Enable/disable double-buffered refresh in GUI, 0: single buffer, 1: double buffer.

**config.gui.flags.fb_in_psram** -> Whether the buffer is placed in PSRAM. If performance is needed, it can be placed in memory, but be mindful of the buffer size.

**config.gui.flags.full_refresh** -> Enable full refresh for every GUI drawing.

**config.gui.flags.direct_mode** -> Enable direct mode for GUI drawing buffer using absolute coordinates. No need to modify normally.

**config.gui.flags.antialiasing** -> Pass-through option for antialiasing in LVGL.

**config.gui.flags.avoid_te** -> Avoid screen tearing. Currently only effective for RGB screens.

## Touch

Supports asynchronous refresh in a thread-safe manner. Configurable options include touch refresh thread stack size and enable/disable, as well as I2C frequency.

```
qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
config.touch.en = 1; // 0: Disable touch functionality, 1: Enable thread-based reading
config.touch.i2c_freq = 400000;
config.touch.update_task.en = 1; // 0: Synchronous refresh, directly read via touch_read_points
config.touch.update_task.priority = 5; // Touch thread priority
qmsd_board_init(&config);
```

# GUI Update Process

***qmsd_board_init -> gui init -> gui_user_init -> gui_update***

gui init: GUI initialization, buffer allocation, binding touch read events, binding screen refresh callbacks.

gui_user_init: Used to initialize the user's GUI entry function, called in app_main.

> [!NOTE]
> gui_user_init runs in the app_main thread. If there is a stack overflow, a one-shot GUI timer (lv_timer) needs to be enabled in gui_user_init, and the initialization code should be placed in the timer (running in the update task). Also, the config.gui.update_task.stack_size needs to be increased. For example, if using freetype, it is recommended to increase it to 48 * 1024 Bytes.

gui_update: Runs in the gui.update_task thread, continuously calls gui update at regular intervals.

> [!NOTE]
> gui_update is a thread call, and GUI refresh needs to ensure thread safety. GUI functions must be executed within GUI callback functions. If they need to be executed in other threads, they need to be wrapped with qmsd_gui_lock and qmsd_gui_unlock.

```
qmsd_gui_lock(portMAX_DELAY);
lv_xxxx_xx();
qmsd_gui_unlock();
```

# GUI Memory Usage

Default: LV_MEM_SIZE_KILOBYTES = 32KB. GUI pre-allocates a contiguous block of memory using internal chip memory.

If the GUI requires too many controls and 32KB is not enough, you can modify the default configuration MEM_SIZE_KILOBYTES in the menuconfig.

If the internal memory is not sufficient, this part can be placed in PSRAM. Modify the CMakeLists.txt file in the project directory, for example:

```
cmake_minimum_required(VERSION 3.5)

# Here,
add_definitions(-DLV_MEM_CUSTOM=1)
  
# LVGL 8.3
add_definitions(-DCONFIG_LV_MEM_POOL_ALLOC=malloc)
add_definitions(-DCONFIG_LV_MEM_POOL_INCLUDE=<stdlib.h>)
# LVGL 7.11
add_definitions(-DCONFIG_LV7_MEM_POOL_ALLOC=malloc)
add_definitions(-DCONFIG_LV7_MEM_POOL_INCLUDE=<stdlib.h>)
# End

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(get-start)
```
