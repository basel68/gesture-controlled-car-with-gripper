# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(nrfTesting C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#freeRTOS directory
include_directories(D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/include)
include_directories(D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/portable/GCC/ARM_CM0)
# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()
# Add the FreeRTOS Kernel source files
set(FREERTOS_KERNEL_SOURCES
    D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/tasks.c
    D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/queue.c
    D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/list.c
    D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/timers.c
   
)

# Add the FreeRTOS port for ARM Cortex-M0
set(FREERTOS_PORT_SOURCES
    D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c
    #also include a memory management file like heap_4.c
    D:/freeRTOS/FreeRTOSv202212.01/FreeRTOS/Source/portable/MemMang/heap_4.c
)
# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    main.c
    Nrf.c
    car.c
    servo.c
    # State_Machine.c
    ${FREERTOS_KERNEL_SOURCES}
    ${FREERTOS_PORT_SOURCES}
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_pwm
    hardware_adc
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)