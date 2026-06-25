# STM32伺服超声波雷达测绘系统

## 项目概述
基于STM32F103C8T6、HC-SR04超声波模块、SG90舵机、OLED显示屏、AT24C64存储芯片和Node-RED上位机的雷达测绘系统。

## 硬件连接

### 引脚分配
- **HC-SR04超声波**: 
  - Trigger: PA0
  - Echo: PA1
  
- **SG90舵机**: 
  - PWM: PA1 (TIM2_CH2)
  
- **OLED (SSD1306)**: 
  - SCL: PB6
  - SDA: PB7
  
- **AT24C64**: 
  - SCL: PB6 (与OLED共用)
  - SDA: PB7 (与OLED共用)
  
- **按键**: 
  - PA0 (外部中断EXTI0)
  
- **LED**: 
  - PC13
  
- **USART1**: 
  - TX: PA9
  - RX: PA10
  - 波特率: 115200

## 软件架构

### 文件结构
```
├── main.c                 # 主程序（基础版本）
├── main_extended.c        # 主程序（扩展版本）
├── delay.c/h              # 延时函数
├── usart.c/h             # 串口通信
├── oled.c/h              # OLED显示
├── at24c64.c/h           # EEPROM存储
├── ultrasonic.c/h        # 超声波测距
├── servo.c/h             # 舵机控制
├── key.c/h               # 按键输入
├── led.c/h               # LED控制
├── json_parser.c/h       # JSON命令解析
├── font.c/h              # 字库文件
└── nodered_flow.json     # Node-RED流程配置
```

## 基础功能实现

### 1. GPIO输入输出
- **按键输入**: PA0配置为上拉输入，支持外部中断
- **LED输出**: PC13配置为推挽输出
- **舵机输出**: PA1配置为复用推挽输出，TIM2_CH2 PWM

### 2. 中断
- **外部中断**: PA0按键触发EXTI0中断，切换扫描模式
- **串口中断**: USART1接收中断，处理Node-RED命令

### 3. 定时器
- **TIM2**: 50Hz PWM输出控制舵机
- **TIM3**: 超声波测距计时（1MHz计数频率）

### 4. UART串口通信
- 波特率115200
- 支持JSON格式数据上传
- 支持命令解析（A/M切换模式，角度控制）

### 5. OLED显示
- 128x64 SSD1306
- I2C通信（PB6/PB7）
- 支持文本和雷达图显示

### 6. I2C AT24C64存储
- 8KB EEPROM
- 存储扫描数据（181个角度的距离值）
- 断电数据不丢失

### 7. 超声波测距
- HC-SR04模块
- 测距范围2-400cm
- 精度约0.3cm

### 8. Node-RED上位机
- 串口数据接收
- JSON数据解析
- 雷达图可视化
- 距离仪表盘
- 预警提示
- 角度控制滑块
- 模式切换按钮

## 扩展功能

### 1. OLED画图功能
- 在OLED上绘制雷达极坐标图
- 实时显示障碍物分布
- 支持文本/雷达图模式切换

### 2. Node-RED远程控制
- 通过Node-RED切换OLED显示模式
- 远程控制开关
- OLED屏幕状态回传到Node-RED

## Keil工程配置

### 1. 创建工程
1. 打开Keil uVision
2. Project → New μVision Project
3. 选择STM32F103C8T6
4. 添加所有.c文件到工程
5. 添加STM32标准库文件

### 2. 配置选项
- **Target选项卡**:
  - Xtal: 8MHz
  - Use MicroLIB: 勾选（支持printf）
  
- **Output选项卡**:
  - Create HEX File: 勾选

### 3. 添加库文件
需要添加的STM32标准库文件:
- stm32f10x_gpio.c
- stm32f10x_rcc.c
- stm32f10x_usart.c
- stm32f10x_tim.c
- stm32f10x_exti.c
- stm32f10x_misc.c
- stm32f10x_afio.c

## 测试方案

### 第一阶段：模块单独测试

#### 测试1：LED测试
```c
// 在main.c中添加测试代码
LED_Init();
while(1) {
    LED_On();
    delay_ms(500);
    LED_Off();
    delay_ms(500);
}
```
**预期结果**: LED每500ms闪烁一次

#### 测试2：按键测试
```c
Key_Init();
while(1) {
    if(Key_Scan() == KEY_PRESS) {
        LED_Toggle();
    }
}
```
**预期结果**: 按键按下时LED状态翻转

#### 测试3：舵机测试
```c
Servo_Init();
while(1) {
    Servo_SetAngle(0);
    delay_ms(1000);
    Servo_SetAngle(90);
    delay_ms(1000);
    Servo_SetAngle(180);
    delay_ms(1000);
}
```
**预期结果**: 舵机在0°、90°、180°之间转动

#### 测试4：超声波测试
```c
Ultrasonic_Init();
float distance;
while(1) {
    distance = Ultrasonic_GetDistance();
    printf("Distance: %.1f cm\r\n", distance);
    delay_ms(500);
}
```
**预期结果**: 串口输出实时距离值

#### 测试5：OLED测试
```c
OLED_Init();
OLED_ShowString(0, 0, "Hello STM32", 16);
```
**预期结果**: OLED显示"Hello STM32"

#### 测试6：AT24C64测试
```c
AT24C64_Init();
AT24C64_WriteByte(0, 0x55);
uint8_t data = AT24C64_ReadByte(0);
printf("Read: 0x%02X\r\n", data);
```
**预期结果**: 串口输出0x55

#### 测试7：串口测试
```c
USART1_Init(115200);
printf("USART Test\r\n");
```
**预期结果**: 串口助手输出"USART Test"

### 第二阶段：集成测试

#### 测试8：自动扫描模式
1. 烧录main.c
2. 按键切换到自动模式
3. 观察舵机自动扫描
4. 观察OLED显示角度和距离
5. 观察LED预警状态

**预期结果**: 
- 舵机在0-180°自动扫描
- OLED实时显示角度和距离
- 距离<30cm时LED亮起

#### 测试9：手动模式
1. 按键切换到手动模式
2. 每次按键角度变化10°
3. 观察舵机响应

**预期结果**: 按键控制舵机角度

#### 测试10：数据存储测试
1. 运行自动扫描一段时间
2. 断电重启
3. 观察是否恢复上次数据

**预期结果**: 扫描数据恢复

### 第三阶段：Node-RED测试

#### 测试11：Node-RED配置
1. 安装Node-RED
2. 安装node-red-dashboard
3. 导入nodered_flow.json
4. 配置串口COM端口

#### 测试12：数据上传测试
1. 启动STM32程序
2. 打开Node-RED Dashboard
3. 观察雷达图、仪表盘、预警提示

**预期结果**: 
- 雷达图实时更新
- 仪表盘显示距离
- 预警提示正常

#### 测试13：远程控制测试
1. 使用Node-RED滑块控制角度
2. 使用按钮切换模式
3. 观察STM32响应

**预期结果**: Node-RED控制STM32

### 第四阶段：扩展功能测试

#### 测试14：OLED雷达图显示
1. 烧录main_extended.c
2. 切换到雷达图模式
3. 观察OLED雷达图

**预期结果**: OLED显示雷达图

#### 测试15：Node-RED扩展功能
1. 导入nodered_flow_extended.json
2. 测试OLED显示模式切换
3. 测试远程控制开关

**预期结果**: Node-RED控制OLED显示模式

## 调试方法

### 1. 串口调试
使用串口助手（如XCOM、SecureCRT）查看调试信息：
```c
printf("Debug: angle=%d, distance=%.1f\r\n", angle, distance);
```

### 2. LED调试
使用LED指示程序状态：
```c
LED_On();   // 进入某段代码
// ... 代码 ...
LED_Off();  // 退出某段代码
```

### 3. 逻辑分析仪调试
使用逻辑分析仪查看：
- I2C时序
- PWM波形
- 超声波触发/回响信号

### 4. 断点调试
在Keil中设置断点：
1. 点击行号设置断点
2. 点击Debug按钮
3. 单步执行（F10/F11）
4. 查看变量窗口

### 5. 常见问题排查

#### 问题1：OLED不显示
- 检查I2C地址是否正确（0x78）
- 检查SCL/SDA接线
- 检查上拉电阻

#### 问题2：超声波测距不准
- 检查Trigger脉冲宽度（应为10us）
- 检查Echo引脚配置
- 检查定时器配置

#### 问题3：舵机不转动
- 检查PWM频率（应为50Hz）
- 检查脉冲宽度（0.5-2.5ms）
- 检查电源供电

#### 问题4：串口无数据
- 检查波特率设置
- 检查TX/RX接线
- 检查是否勾选Use MicroLIB

#### 问题5：AT24C64读写失败
- 检查I2C时序
- 检查设备地址（0xA0）
- 检查写延时（10ms）

## 性能优化

### 1. 减少延时
- 使用定时器替代delay_ms
- 优化I2C通信速度

### 2. 数据滤波
- 添加滑动平均滤波
- 去除异常数据点

### 3. 功耗优化
- 空闲时进入低功耗模式
- 关闭不必要的外设

## 扩展建议

1. **增加温湿度传感器**: DHT11/DHT22
2. **增加陀螺仪**: MPU6050
3. **增加WiFi模块**: ESP8266
4. **增加SD卡**: 大容量数据存储
5. **增加语音模块**: 语音控制

## 总结

本项目实现了完整的8个基础功能和2个扩展功能：
- GPIO输入输出（按键、LED、舵机）
- 中断（外部中断、串口中断）
- 定时器（PWM、测距计时）
- UART串口通信（JSON命令解析）
- OLED显示（文本、雷达图）
- I2C AT24C64存储
- 超声波测距
- Node-RED上位机（数据可视化、远程控制）

扩展功能：
- OLED画图（雷达图显示）
- Node-RED远程控制（OLED模式切换、屏幕回传）
