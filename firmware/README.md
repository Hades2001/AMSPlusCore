# AMS Plus Firmware Compilation Guide

This document provides step-by-step instructions to compile the AMS Plus embedded firmware for ESP32S3. Please ensure you have the required tools and dependencies installed before proceeding.


## Prerequisites

1. **ESP-IDF Environment**:  
   The AMS Plus firmware requires the **ESP-IDF v5.1.3** for compilation. Follow the [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf) to set up the environment.  
   Ensure you have Python, Git, and other necessary tools installed as described in the guide.

2. **QMSD-ESP32-BSP**:  
   The project is built on the QMSD-ESP32-BSP framework. Please ensure all submodules are properly cloned.


## Step-by-Step Guide

### 1. Clone the AMS Plus Firmware Repository
Use the following command to clone the repository and include all submodules:
```bash
git clone --recursive https://github.com/Hades2001/AMSPlusCore
```
If you already cloned the repository without submodules, initialize and update them:

```bash
git submodule update --init --recursive
```

### 2. Set Up ESP-IDF Environment

- Install ESP-IDF v5.1.3 or ensure it is already installed on your system.

- Run the following commands to set up the environment (replace 
  ```bash
  cd <idf_path>
  ./install.sh
  . ./export.sh
  ```

### 3. Configure the Project

Navigate to the project directory and configure the build:

```bash
cd AMSPlus/firmware
idf.py menuconfig
```

Modify the settings as needed for your device and environment.

### 4. Build the Firmware

Compile the firmware with:

```bash
idf.py build
```

### 5. Flash the Firmware

Flash the compiled firmware to the ESP32S3 device:

```bash
idf.py flash
```

Monitor the device output with:

```bash
idf.py monitor
```

You can exit the monitor by pressing `Ctrl+]`.

## Troubleshooting

- Ensure that your ESP-IDF environment matches version 5.1.3. Incompatible versions may lead to build errors.

- If QMSD-ESP32-BSP components are missing, verify submodule updates with:

  ```bash
  git submodule update --init --recursive
  ```

- Refer to the additional resources below for detailed guides on ESP32, LVGL, and QMSD.

## Additional Resources

1. **ESP-IDF Programming Guide**
    Recommended version: **v5.1.3**
    https://docs.espressif.com/projects/esp-idf
2. **ESP32 Programming Examples**
    Ensure compatibility with IDF v5.1.3.
    https://github.com/espressif/esp-idf/tree/master/examples
3. **LVGL Programming Guide**
    https://docs.lvgl.io/8.3/
4. **QMSD-ESP32-BSP Documentation**
    https://github.com/smartpanle/QMSD-ESP32-BSP/tree/master
