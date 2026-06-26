#!/usr/bin/env bash
#===============================================================================
# build.sh — 命令行编译 + 烧录 STM32F103C8
#
# 用法:
#   ./build.sh             编译（默认 main.c）
#   ./build.sh extended    编译 main_extended.c
#   ./build.sh flash       烧录到 STM32
#   ./build.sh clean       清理
#   ./build.sh rebuild     重新编译
#
# 依赖: STM32CubeCLT (自动检测路径)
#===============================================================================
set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_DIR"

# ---- 自动检测 STM32CubeCLT 安装路径 ----
if [ -d "/d/ST-stm32-clt/STM32CubeCLT_1.21.0" ]; then
    TOOLCHAIN_DIR="/d/ST-stm32-clt/STM32CubeCLT_1.21.0"
elif [ -d "/c/ST/STM32CubeCLT_1.21.0" ]; then
    TOOLCHAIN_DIR="/c/ST/STM32CubeCLT_1.21.0"
elif [ -d "/c/Program Files/STMicroelectronics/STM32CubeCLT" ]; then
    TOOLCHAIN_DIR="/c/Program Files/STMicroelectronics/STM32CubeCLT"
else
    echo "错误: 找不到 STM32CubeCLT"
    echo "请编辑 build.sh，手动设置 TOOLCHAIN_DIR"
    exit 1
fi

# 关键工具目录
CMAKE_DIR="$TOOLCHAIN_DIR/CMake/bin"
GCC_DIR="$TOOLCHAIN_DIR/GNU-tools-for-STM32/bin"
NINJA_DIR="$TOOLCHAIN_DIR/Ninja/bin"
PROGRAMMER_DIR="$TOOLCHAIN_DIR/STM32CubeProgrammer/bin"

# 把工具加入 PATH，让 cmake 能自动找到
export PATH="$GCC_DIR:$NINJA_DIR:$CMAKE_DIR:$PATH"

# ---- CMake 参数 ----
BUILD_DIR="$PROJECT_DIR/build"
CMAKE="$CMAKE_DIR/cmake"

CMAKE_OPTS=(
    -G Ninja
    -DCMAKE_TOOLCHAIN_FILE="$PROJECT_DIR/arm-none-eabi.cmake"
    -DCMAKE_MAKE_PROGRAM="$NINJA_DIR/ninja"
    -S "$PROJECT_DIR"
    -B "$BUILD_DIR"
)

if [ "$1" = "extended" ]; then
    CMAKE_OPTS+=(-DUSE_EXTENDED_MAIN=ON)
fi

# ---- 函数 ----
cmd_build() {
    echo "==> 配置 cmake ..."
    "$CMAKE" "${CMAKE_OPTS[@]}"

    echo ""
    echo "==> 编译中 ..."
    "$CMAKE" --build "$BUILD_DIR"

    echo ""
    echo "==> 完成！输出文件:"
    ls -lh "$BUILD_DIR/test.elf" "$BUILD_DIR/test.hex" "$BUILD_DIR/test.bin" 2>/dev/null
}

cmd_flash() {
    local ELF_FILE="$BUILD_DIR/test.elf"
    if [ ! -f "$ELF_FILE" ]; then
        # 如果没有 elf，试试 hex
        HEX_FILE="$BUILD_DIR/test.hex"
        if [ ! -f "$HEX_FILE" ]; then
            echo "错误: 未找到 $ELF_FILE 或 $HEX_FILE"
            echo "请先执行: ./build.sh"
            exit 1
        fi
        ELF_FILE="$HEX_FILE"
    fi

    echo "==> 烧录 $ELF_FILE 到 STM32 ..."
    "$PROGRAMMER_DIR/STM32_Programmer_CLI.exe" \
        -c port=SWD \
        -w "$ELF_FILE" \
        -v

    echo ""
    echo "==> 烧录完成！按复位键运行。"
}

cmd_clean() {
    echo "==> 清理 build 目录 ..."
    rm -rf "$BUILD_DIR"
}

cmd_rebuild() {
    cmd_clean
    cmd_build
}

# ---- 分发 ----
case "${1:-build}" in
    build)    cmd_build ;;
    extended) cmd_build ;;
    flash)    cmd_flash ;;
    clean)    cmd_clean ;;
    rebuild)  cmd_rebuild ;;
    *)
        echo "用法: $0 [build|extended|flash|clean|rebuild]"
        exit 1
        ;;
esac
