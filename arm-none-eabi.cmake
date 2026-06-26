#===============================================================================
# arm-none-eabi.cmake — CMake toolchain file for ARM bare-metal cross-compilation
#
# Usage: cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake ...
#===============================================================================

# Target system — bare-metal ARM, no OS
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  ARM)

# Compilers
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Skip compiler ABI/working detection — cross-compiler cannot link Windows exes
set(CMAKE_C_COMPILER_WORKS   1 CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER_WORKS 1 CACHE INTERNAL "")
set(CMAKE_ASM_COMPILER_WORKS 1 CACHE INTERNAL "")

# Post-processing tools
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE    arm-none-eabi-size)

# No OS — find only cross tools in the prefix path
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
