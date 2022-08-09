# smart-car-deep-learning

20级数科班 2022年暑期科研实训项目

## 概述

- 作业相关文档在 `docs` 目录下。
- 固件在 `firmware` 目录下。
- PC 端软件在 `client` 目录下。

## 初始化

1. 克隆本仓库
2. 更新 Git 子模块: `git submodule update --init` (为了安装 PaddleSeg 和 PaddleDetection)
3. 从 Release 页面下载交通标志识别的权重文件 `model.pdiparams` 到 `signdet` 目录下

## 安装

### 运行环境

- 硬件： 开发板和 ESP32-CAM
- 软件： PC 端 Python 3.10+, 在 Windows/Linux 下测试过

### 开发环境

- Visual Studio Code 
- Git v2+
- VSCode 的 PlatformIO 插件
- Arduino IDE v1.8~v2.0.0-rc9

### 配置

- 打开 `firmware/shared/lie-flat/src/config.h`, 修改 WiFi 的 SSID 和密码
- 按照 `firmware/shared/lie-flat/src/pin.h`，完成开发板接线。

###  接线

#### 摄像头

<img src="README.assets/IMG_20220809_193332.jpg" alt="IMG_20220809_193332" style="zoom:10%;" />

5V 和 GND 接开发板的 5V 和 GND

#### 舵机

舵机接开发板舵机B(注意不要接反)

#### 蜂鸣器

一根杜邦线连接开发板上的中间两个管脚

#### 电机

积木连接

#### 光电传感器/测速计/optocoupler

信号线接 GPIO14, VCC 和 GND 接开发板.

#### MPU6050 六自由度运动传感器

SCL/SDA 接开发板 SCL/SDA

VCC/GND 照旧

### 固件安装

- 使用 Arduino IDE 打开 `firmware/camera_web_server`，修改 WiFi 的 SSID 和密码，与配置步骤中的一样, 下载到 ESP32-CAM 上。
- 使用 VSCode + PlatformIO 打开 `firmware/main`, 构建并下载到开发板上。

## 启动

- PC 连接到同一热点
- 开启电源
- 运行 `client/main.py`, 该脚本会自动连接开发板，连接成功后，开发板蜂鸣器报警 1.5 秒。

