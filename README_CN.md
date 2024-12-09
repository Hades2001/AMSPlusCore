# SDK启动流程

Init 开始->拉动复位引脚 -> 初始化屏幕 -> 初始化触摸 -> 初始化UI驱动 -> 开启线程更新UI->结束Init

启动代码：components/qmsd_board/xxx/xxx/qmsd_board.c， 可按需求进行更改

# 可配置项

SDK有默认初始化值，用户可按需求进行修改, 修改都可通过  ***menuconfig->QMSD Hal ***  进行配置, 也可修改初始化代码， 以下为可配置项：

## 屏幕方向

默认方向：  **BOARD_ROTATION_0**  ， 可配置为   **BOARD_ROTATION_90**  ，   **BOARD_ROTATION_180**  ，  **BOARD_ROTATION_270**  ，对应初始化代码：

```
qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
config.board_dir = BOARD_ROTATION_0;
qmsd_board_init(&config);
```
> [!WARNING]
> RGB屏幕目前不支持旋转

## 背光

默认亮度100，delay时间200ms，可修改初始化代码：

```
qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
config.backlight.value = 100.0;
config.backlight.delay_ms = 200; // 自定义时间
qmsd_board_init(&config);
```

程序中进行背光设置：

```
#include "qmsd_board.h"

// light: 0.0 ~ 100.0
void qmsd_board_backlight_set(float light);

// 延迟更新背光，避免开机时花屏
void qmsd_board_backlight_set_delay(float light, uint32_t delay_ms);
```

## GUI

支持  ***LVGL-8.3***  、  ***LVGL-7.11***  ，默认支持  ***LVGL-8.3***

切换LVGL-8.3及LVGL-7.11：

***menuconfig → Component config → LVGL configuration → Select UI_ENGINE***

GUI 自身可配置项：

***menuconfig  → Component config → LVGL configuration → LVGL configuration***

可配置项：

**config.gui.en**  -> 1：init时初始化GUI，0：不初始化GUI，需要使用其它未支持的UI驱动时可关闭GUI

**config.gui.buffer_size**  -> byte，目前为GUI的单个buffer大小，0：全屏幕buffer

**config.gui.update_task**  -> GUI 更新线程配置，可调优先级，线程大小，及使能，使能关闭后需自行调用GUI update 事件

**config.gui.refresh_task**   -> 屏幕刷新线程，屏幕绘制函数放置另外线程刷新，不阻塞GUI计算绘制，单buffer模式无效

**config.gui.flags.double_fb**  -> GUI是否开启双buffer刷新，0：单buffer，1：双buffer

**config.gui.flags.fb_in_psram**   -> buffer 是否放置于psram，需要性能时可放置于内存，但是要注意buffer大小

**config.gui.flags.full_refresh**   -> 使能后GUI每次绘制都为全局刷新

**config.gui.flags.direct_mode**   -> 使能后GUI绘制的buffer为绝对坐标，正常无需修改

**config.gui.flags.antialiasing**   -> 透传到lvgl的antialiasing选项

**config.gui.flags.avoid_te**   -> 避免屏幕撕裂，目前只对RGB屏幕有效

## 触摸

支持线程异步刷新（线程安全），可配置触摸刷新线程的堆栈及使能，可配置i2c频率

```
qmsd_board_config_t config = QMSD_BOARD_DEFAULT_CONFIG;
config.touch.en = 1; // 0: 不开启触摸功能，1：开启线程读取
config.touch.i2c_freq = 400000;
config.touch.update_task.en = 1; // 0：同步刷新，调用touch_read_points时直接进行i2c读取
config.touch.update_task.priority = 5; // 触摸线程优先级
qmsd_board_init(&config);
```

# GUI 更新流程

***qmsd_board_init -> gui init -> gui_user_init -> gui_update***

gui init: gui 的初始化，buffer申请， 绑定触摸读取事件，绑定屏幕刷新回调

gui_user_init：用于初始化用户的gui入口函数，在app_main中调用

> [!NOTE]  
> gui_user_init在app_main线程进行，如果stackoverflow，需要在gui_user_init中开启一次性GUI定时器（lv_timer），将初始化代码放置于定时器（在update task）运行，且需要扩大 config.gui.update_task.stack_size, 例如需要使用freetype建议扩大至 48 * 1024 Byte

gui_update: 位于gui.update_task线程运行，循环定时调用gui update

> [!NOTE]  
> gui_update为线程调用，GUI刷新需要保证有线程安全，gui的函数必须放置于GUI的回调函数中执行，如果需要在其它线程执行，需要使用qmsd_gui_lock及qmsd_gui_unlock包裹gui代码

```
qmsd_gui_lock(portMAX_DELAY);
lv_xxxx_xx();
qmsd_gui_unlock();
```

# GUI内存占用

默认：LV_MEM_SIZE_KILOBYTES = 32KB， GUI预先申请一整块连续内存，使用芯片内部内存

如果GUI需要的控件太多了，32KB不够用，可进入menuconfig进行修改默认配置MEM_SIZE_KILOBYTES 

如果内部内存不够，这部分可放置于PSRAM里，修改项目工程下的CMakeLists.txt, 例如

```
cmake_minimum_required(VERSION 3.5)

# Here，
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
