# Keil工程配置操作指南

## 一、创建新工程

### 步骤1：启动Keil uVision
1. 打开Keil uVision软件
2. 点击菜单 `Project` → `New μVision Project...`

### 步骤2：保存工程
1. 选择工程保存位置：`c:\Users\30376\Desktop\new2\`
2. 输入工程名称：`RadarSystem`
3. 点击保存

### 步骤3：选择芯片
1. 在设备选择窗口中搜索 `STM32F103C8`
2. 选择 `STMicroelectronics` → `STM32F103C8` (或STM32F103C8T6)
3. 点击OK

### 步骤4：添加启动文件
1. 弹出"Copy STM32 Startup Code to Project Folder"对话框
2. 选择 `Yes`
3. 选择 `startup_stm32f10x_md.s` (中等容量设备)
4. 点击添加

## 二、添加源文件到工程

### 步骤1：创建文件组
在工程窗口中右键点击 `Target 1`，选择 `Manage Project Items`

创建以下组：
- `Source` - 存放.c文件
- `Include` - 存放.h文件
- `Library` - 存放库文件

### 步骤2：添加源文件
在 `Source` 组中添加以下文件：
- `main.c` (或 `main_extended.c`)
- `delay.c`
- `usart.c`
- `oled.c`
- `at24c64.c`
- `ultrasonic.c`
- `servo.c`
- `key.c`
- `led.c`
- `json_parser.c`
- `font.c`

**操作方法**：
1. 右键点击 `Source` 组
2. 选择 `Add Existing Files to Group 'Source'`
3. 选择上述所有.c文件
4. 点击添加

### 步骤3：添加STM32标准库文件

在 `Library` 组中添加以下库文件（需要先下载STM32标准库）：

**必需的库文件**：
- `stm32f10x_gpio.c`
- `stm32f10x_rcc.c`
- `stm32f10x_usart.c`
- `stm32f10x_tim.c`
- `stm32f10x_exti.c`
- `stm32f10x_misc.c`
- `stm32f10x_afio.c`

**库文件位置**：
通常在STM32标准库的 `STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\STM32F10x_StdPeriph_Driver\src` 目录下

## 三、配置工程选项

### 步骤1：打开工程选项
1. 点击菜单 `Project` → `Options for Target 'Target 1'`
2. 或按快捷键 `Alt+F7`

### 步骤2：配置Target选项卡

**Device选项卡**：
- 确认选择的是 `STM32F103C8`

**Target选项卡**：
- `Xtal (MHz)`: 设置为 `8` (外部晶振频率)
- 勾选 `Use MicroLIB` (重要！支持printf函数)

**Output选项卡**：
- 勾选 `Create HEX File` (生成HEX文件用于烧录)
- `Name of Executable`: `RadarSystem`

**C/C++选项卡**：
- `Define`: 添加 `STM32F10X_MD,USE_STDPERIPH_DRIVER`
- `Include Paths`: 添加以下路径：
  ```
  .\Include
  .\Library\inc
  ```
  (根据实际库文件路径调整)

**Debug选项卡**：
- `Use`: 选择 `ST-Link Debugger` (如果使用ST-Link)
- 或选择 `J-LINK/J-Trace Cortex` (如果使用J-Link)

## 四、添加头文件路径

### 步骤1：设置头文件路径
在 `Options for Target` → `C/C++` → `Include Paths` 中添加：

```
.\Include
```

确保所有.h文件都在Include文件夹中，或者在工程根目录下。

## 五、编译工程

### 步骤1：编译
1. 点击菜单 `Project` → `Build Target` (F7)
2. 或点击工具栏的编译按钮

### 步骤2：检查编译结果
- 查看Output窗口的编译信息
- 确认 `0 Error(s), 0 Warning(s)`
- 如果有错误，根据提示修改代码

## 六、下载程序到STM32

### 步骤1：连接硬件
1. 使用ST-Link连接STM32开发板
2. ST-Link的SWDIO连接到开发板的SWDIO
3. ST-Link的SWCLK连接到开发板的SWCLK
4. ST-Link的3.3V连接到开发板的3.3V
5. ST-Link的GND连接到开发板的GND

### 步骤2：下载程序
1. 点击菜单 `Flash` → `Download`
2. 或按快捷键 `F8`
3. 等待下载完成

### 步骤3：复位运行
1. 点击开发板上的复位按钮
2. 或在Keil中点击 `Debug` → `Run` (F5)

## 七、调试程序

### 步骤1：进入调试模式
1. 点击菜单 `Debug` → `Start/Stop Debug Session` (Ctrl+F5)

### 步骤2：设置断点
1. 在代码行号左侧点击，设置断点（红色圆点）
2. 建议在main函数的while(1)循环处设置断点

### 步骤3：单步执行
- `F10`: 单步执行（跳过函数）
- `F11`: 单步执行（进入函数）
- `Ctrl+F11`: 跳出当前函数

### 步骤4：查看变量
1. 打开 `Watch` 窗口
2. 输入变量名查看值
3. 例如：`current_angle`, `current_distance`

### 步骤5：查看串口输出
1. 打开串口助手（如XCOM、SecureCRT）
2. 配置：COM口、波特率115200
3. 观察printf输出

## 八、常见问题解决

### 问题1：编译错误 "undefined symbol"
**原因**：缺少库文件
**解决**：
1. 检查是否添加了所有必需的库文件
2. 检查头文件路径是否正确
3. 检查Define中是否添加了 `STM32F10X_MD`

### 问题2：printf不输出
**原因**：未勾选Use MicroLIB
**解决**：
1. 在 `Options for Target` → `Target` 中勾选 `Use MicroLIB`
2. 重新编译

### 问题3：下载失败
**原因**：ST-Link连接问题或驱动未安装
**解决**：
1. 检查ST-Link连接是否正确
2. 安装ST-Link驱动
3. 在 `Debug` 选项卡中重新选择调试器并点击 `Settings`

### 问题4：TIM1 PWM不输出
**原因**：TIM1是高级定时器，需要使能主输出
**解决**：
- 确保代码中调用了 `TIM_CtrlPWMOutputs(TIM1, ENABLE);`

### 问题5：串口无数据
**原因**：TX/RX引脚配置错误
**解决**：
- 检查usart.c中的引脚配置
- 当前配置：TX-PA10, RX-PA9

## 九、测试顺序建议

### 第一阶段：基础功能测试
1. 先测试LED（修改main.c，只初始化LED）
2. 测试按键（添加按键扫描）
3. 测试串口（添加printf输出）

### 第二阶段：外设测试
1. 测试舵机（单独测试servo模块）
2. 测试超声波（单独测试ultrasonic模块）
3. 测试OLED（单独测试oled模块）

### 第三阶段：集成测试
1. 整合所有模块
2. 测试自动扫描模式
3. 测试手动模式
4. 测试数据存储

### 第四阶段：上位机测试
1. 安装Node-RED
2. 导入nodered_flow.json
3. 测试数据上传
4. 测试远程控制

## 十、硬件连接检查清单

在烧录程序前，请确认以下连接：

- [ ] LED: PC13 → LED正极，LED负极→GND
- [ ] 按键: PA0 → 按键一端，按键另一端→GND
- [ ] 舵机: PA8 → 橙线，5V→红线，GND→棕线
- [ ] 超声波: PA2→Trig, PA3→Echo, 5V→VCC, GND→GND
- [ ] OLED: PB6→SCL, PB7→SDA, 3.3V→VCC, GND→GND
- [ ] AT24C64: PB6→SCL, PB7→SDA, 3.3V→VCC, GND→GND, A0-A2→GND, WP→GND
- [ ] USART1: PA10→TX, PA9→RX (连接USB转串口模块)

## 十一、注意事项

1. **电源电压**：
   - STM32工作电压3.3V
   - 舵机和超声波需要5V供电
   - OLED和AT24C64可以用3.3V或5V

2. **I2C共用**：
   - OLED和AT24C64共用I2C总线
   - 确保两者的I2C地址不同（OLED:0x78, AT24C64:0xA0）

3. **TIM1特殊配置**：
   - TIM1是高级定时器
   - 需要调用 `TIM_CtrlPWMOutputs()` 使能主输出

4. **串口交叉连接**：
   - STM32的TX(PA10)应连接到USB转串口的RX
   - STM32的RX(PA9)应连接到USB转串口的TX

5. **按键消抖**：
   - 代码中已包含软件消抖
   - 如果按键不灵敏，可以调整消抖延时

## 十二、快速开始

如果只想快速测试，可以按以下步骤：

1. 创建Keil工程
2. 只添加 `main.c`, `delay.c`, `led.c`
3. 修改main.c，只测试LED闪烁
4. 编译下载，确认LED正常
5. 逐步添加其他模块

这样可以快速定位问题，避免一次性添加太多模块导致难以调试。
