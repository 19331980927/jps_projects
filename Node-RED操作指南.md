# Node-RED操作指南

## 一、安装Node-RED

### Windows系统安装

1. **安装Node.js**
   - 访问 https://nodejs.org/
   - 下载LTS版本
   - 安装Node.js

2. **安装Node-RED**
   - 打开命令提示符（CMD）
   - 输入：`npm install -g --unsafe-perm node-red`
   - 等待安装完成

3. **启动Node-RED**
   - 在命令提示符中输入：`node-red`
   - 等待启动完成
   - 浏览器访问：http://localhost:1880

## 二、安装Dashboard插件

1. **安装node-red-dashboard**
   - 在Node-RED目录下打开命令提示符
   - 输入：`npm install node-red-dashboard`
   - 重启Node-RED

2. **验证安装**
   - 访问 http://localhost:1880
   - 左侧面板应该有dashboard节点

## 三、导入流程配置

### 方法1：导入JSON文件

1. **打开Node-RED编辑器**
   - 访问 http://localhost:1880

2. **导入流程**
   - 点击右上角菜单（三条横线）
   - 选择 `Import`
   - 选择 `Clipboard`
   - 复制 `nodered_flow_extended.json` 的内容
   - 粘贴到文本框
   - 点击 `Import`

3. **部署流程**
   - 点击右上角的 `Deploy` 按钮

### 方法2：导入JSON文件

1. **打开Node-RED编辑器**
   - 访问 http://localhost:1880

2. **导入文件**
   - 点击右上角菜单
   - 选择 `Import`
   - 选择 `Select a file to import`
   - 选择 `nodered_flow_extended.json`
   - 点击 `Import`

3. **部署流程**
   - 点击右上角的 `Deploy` 按钮

## 四、配置串口节点

### 修改串口号

1. **找到serial in节点**
   - 在流程图中找到 `serial in` 节点
   - 双击打开配置

2. **配置串口**
   - Serial port: 选择你的COM口（如COM3）
   - Baud rate: 115200
   - Data bits: 8
   - Parity: none
   - Stop bits: 1
   - 点击 `Done`

3. **找到serial out节点**
   - 双击打开配置
   - Serial port: 选择相同的COM口
   - Baud rate: 115200
   - 点击 `Done`

4. **重新部署**
   - 点击 `Deploy` 按钮

## 五、查看Dashboard

1. **访问Dashboard**
   - 浏览器访问：http://localhost:1880/ui
   - 或点击右上角菜单 → `Dashboard`

2. **查看界面**
   - 雷达扫描图
   - 距离仪表盘
   - 预警提示
   - 角度控制滑块
   - 模式切换按钮
   - OLED显示模式切换
   - 远程控制开关

## 六、使用说明

### 基础功能

1. **查看雷达数据**
   - 雷达扫描图实时显示障碍物分布
   - 距离仪表盘显示当前距离
   - 预警提示显示安全状态

2. **控制舵机角度**
   - 拖动角度控制滑块
   - 舵机会转到指定角度

3. **切换扫描模式**
   - 点击"自动扫描"按钮：舵机自动扫描
   - 点击"手动模式"按钮：手动控制角度

### 扩展功能

1. **切换OLED显示模式**
   - 点击"切换OLED显示模式"按钮
   - OLED在文本模式和雷达图模式之间切换

2. **启用远程控制**
   - 打开"启用远程控制"开关
   - STM32会定期发送OLED屏幕状态到Node-RED

## 七、串口命令

### 通过串口助手发送命令

1. **切换自动模式**
   - 发送：`A` 或 `a`

2. **切换手动模式**
   - 发送：`M` 或 `m`

3. **切换OLED显示模式**
   - 发送：`D` 或 `d`

### 通过Node-RED发送JSON命令

1. **设置角度**
   - 使用角度控制滑块
   - 或发送JSON：`{"angle":90}`

2. **切换模式**
   - 使用模式切换按钮
   - 或发送JSON：`{"mode":"auto"}` 或 `{"mode":"manual"}`

3. **切换OLED显示**
   - 使用OLED显示模式按钮
   - 或发送JSON：`{"display_mode":1}` 或 `{"display_mode":0}`

4. **远程控制**
   - 使用远程控制开关
   - 或发送JSON：`{"remote_control":1}` 或 `{"remote_control":0}`

## 八、故障排查

### 问题1：串口连接失败

**原因**：
- COM口被占用
- 串口驱动未安装
- 波特率不匹配

**解决**：
- 检查设备管理器中的COM口
- 关闭其他串口软件
- 重新安装USB转串口驱动

### 问题2：Dashboard无法访问

**原因**：
- node-red-dashboard未安装
- 端口被占用

**解决**：
- 安装node-red-dashboard
- 检查1880端口是否被占用

### 问题3：数据不更新

**原因**：
- STM32未发送数据
- 串口连接问题
- JSON解析失败

**解决**：
- 检查STM32是否正常运行
- 检查串口连接
- 查看Node-RED调试信息

## 九、调试技巧

### 查看串口数据

1. **添加debug节点**
   - 在serial in节点后添加debug节点
   - 选择输出到调试窗口
   - 部署后查看接收到的数据

2. **查看JSON解析结果**
   - 在json节点后添加debug节点
   - 查看解析后的数据格式

### 查看发送数据

1. **在serial out前添加debug节点**
   - 查看即将发送的数据
   - 确认JSON格式正确

## 十、常用快捷键

- `Ctrl+S`: 保存流程
- `Ctrl+Z`: 撤销
- `Ctrl+Y`: 重做
- `Del`: 删除选中节点
- `Ctrl+C`: 复制
- `Ctrl+V`: 粘贴
- `Ctrl+A`: 全选
- `Ctrl+D`: 部署

## 十一、备份和恢复

### 备份流程

1. **导出流程**
   - 点击右上角菜单
   - 选择 `Export`
   - 选择 `Download`
   - 保存为JSON文件

### 恢复流程

1. **导入流程**
   - 点击右上角菜单
   - 选择 `Import`
   - 选择备份的JSON文件
   - 点击 `Import`
