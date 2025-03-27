# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")
set(RTOS_PATH "./FreeRTOS/Source")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Include RTOs kernel
include(${RTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(tosks C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(tosks tosks.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/pico-ssd1306/ssd1306.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/doglab/doglab.c
    ${CMAKE_CURRENT_LIST_DIR}/modules/display/display.c

)

pico_set_program_name(tosks "tosks")
pico_set_program_version(tosks "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(tosks 0)
pico_enable_stdio_usb(tosks 0)

# Add the standard library to the build
target_link_libraries(tosks
    pico_stdlib)

# Add the standard include files to the build
target_include_directories(tosks PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib
    ${CMAKE_CURRENT_LIST_DIR}/lib/pico-ssd1306
    ${CMAKE_CURRENT_LIST_DIR}/lib/doglab
    ${CMAKE_CURRENT_LIST_DIR}/modules/display
)

# Add any user requested libraries
target_link_libraries(tosks
    hardware_spi
    hardware_i2c
    hardware_adc
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
    pico_cyw43_arch_lwip_threadsafe_background
)

pico_add_extra_outputs(tosks)

