#===============================================================================
# CMakeLists.txt — STM32F103C8 (Cortex-M3) for arm-none-eabi-gcc
# Compatible with CLion and command-line builds.
# Keil uVision5 uses test.uvprojx — this file does not affect it.
#===============================================================================

cmake_minimum_required(VERSION 3.16)
project(test LANGUAGES C ASM)

#----------------------------------------------------------------------
# Toolchain preset — expects arm-none-eabi-gcc in PATH.
# In CLion: File → Settings → Build, Execution, Deployment → Toolchains
# → Add "Custom" → set C Compiler / C++ Compiler to arm-none-eabi-gcc / g++
#----------------------------------------------------------------------

# MCU flags — STM32F103C8 is Cortex-M3, no FPU
set(MCU_FLAGS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
)

# Compile definitions — must match Keil Project → C/C++ → Define
add_definitions(
    -DSTM32F10X_MD
    -DUSE_STDPERIPH_DRIVER
)

# Include paths — must match Keil's Include Paths
set(INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/start
    ${CMAKE_CURRENT_SOURCE_DIR}/User
    ${CMAKE_CURRENT_SOURCE_DIR}/library
)

#----------------------------------------------------------------------
# Source files — identical to the Keil project file list
#----------------------------------------------------------------------

# App sources — switch main / main_extended via -DUSE_EXTENDED_MAIN=ON
option(USE_EXTENDED_MAIN "Use main_extended.c instead of main.c" OFF)

if(USE_EXTENDED_MAIN)
    set(MAIN_SRC main_extended.c)
else()
    set(MAIN_SRC main.c)
endif()

set(APP_SRCS
    ${MAIN_SRC}
    ultrasonic.c
    usart.c
    at24c64.c
    delay.c
    font.c
    json_parser.c
    key.c
    led.c
    oled.c
    servo.c
)

# Peripheral library drivers (Keil "Library" group — all .c files)
file(GLOB LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/library/*.c
)

# GCC retarget — _write() for printf over USART1 (Keil uses fputc in usart.c)
set(GCC_RETARGET_SRC
    retarget_gcc.c
)

# System / CMSIS sources (core_cm3.c excluded — GCC uses header-only inline)
set(SYS_SRCS
    User/stm32f10x_it.c
    start/system_stm32f10x.c
    ${GCC_RETARGET_SRC}
)

# GCC startup file (C-based, toolchain-agnostic)
set(STARTUP_SRC
    startup_stm32f10x_md_gcc.c
)

# Collect all sources
set(ALL_SRCS
    ${APP_SRCS}
    ${LIB_SRCS}
    ${SYS_SRCS}
    ${STARTUP_SRC}
)

#----------------------------------------------------------------------
# Build executable
#----------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${ALL_SRCS})

# Output as test.elf (not just "test")
set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".elf"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -Wall -Wextra -Wno-unused-parameter
    -ffunction-sections -fdata-sections
)

target_link_options(${PROJECT_NAME} PRIVATE
    ${MCU_FLAGS}
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8.ld
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,-u,_printf_float
    -Wl,-u,_scanf_float
)

# Math library (must come after object files in link order)
target_link_libraries(${PROJECT_NAME} PRIVATE m)

#----------------------------------------------------------------------
# Post-build: generate .hex and .bin for flashing
#----------------------------------------------------------------------

# Post-build: generate .hex and .bin for flashing
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMENT "Generating ${PROJECT_NAME}.hex and ${PROJECT_NAME}.bin"
)
