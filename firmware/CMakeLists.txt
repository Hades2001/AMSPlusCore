# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# In a user project, you can move the This SDK to the same project and rename it to 'qmsd_esp32_sdk'
# set(ENV{QMSD_8MS_PATH} ${CMAKE_SOURCE_DIR}/qmsd_esp32_sdk)

if(NOT DEFINED ENV{QMSD_8MS_PATH})
    get_filename_component(QMSD_8MS_PATH "${CMAKE_SOURCE_DIR}/QMSD-ESP32-BSP" ABSOLUTE)
    set(ENV{QMSD_8MS_PATH} ${QMSD_8MS_PATH})
endif()


# sset(PROJECT_VER "1.0.0")

include($ENV{QMSD_8MS_PATH}/component.cmake)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(AMSPlus)

target_add_binary_data(AMSPlus.elf "ca_certificate.pem" TEXT)
target_add_binary_data(AMSPlus.elf "index.html" TEXT)
