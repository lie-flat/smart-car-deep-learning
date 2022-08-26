# smart-car-deep-learning

20级数科班 2022年暑期科研实训项目

## 参考资料

- https://github.com/PaddlePaddle/PaddleDetection
- https://github.com/PaddlePaddle/PaddleSeg
- https://github.com/georgesung/advanced_lane_detection
- https://www.masterclass.com/articles/how-to-use-hsv-color-model-in-photography
- https://stackoverflow.com/questions/48109650/how-to-detect-two-different-colors-using-cv2-inrange-in-python-opencv
- https://github.com/emirelesg/Self-Driving-Vehicle/
- https://www.computervision.zone/courses/self-driving-car-using-raspberry-pi/
- https://github.com/PaddleCV-FAQ/PaddleDetection-FAQ/blob/main/Lite%E9%83%A8%E7%BD%B2/yolov3_for_raspi.md
- https://aistudio.baidu.com/aistudio/projectdetail/4043400
- https://docs.arduino.cc/
- https://docs.platformio.org/en/latest/

## 照片展示

<img src="README.assets/IMG_20220826_190447.jpg" alt="地图" style="zoom:30%;" />

<img src="README.assets/IMG_20220826_190538.jpg" style="zoom:33%;" />

<img src="README.assets/IMG_20220826_190551.jpg" style="zoom:33%;" />

<img src="README.assets/IMG_20220826_204604.jpg" style="zoom:33%;" />

<img src="README.assets/IMG_20220826_204613.jpg" style="zoom:33%;" />

## 功能

- 通过 HTTP 请求读取所有传感器/相机
- 自动化设备发现协议, 无需手动配置 IP 地址
- 通过 HTTP 请求控制电机/舵机/蜂鸣器/LED
- 使用游戏手柄遥控小车(游戏手柄连接到电脑, 运行 `client/joystick.py`)
- 人工智能自动巡线驾驶，识别不同的交通标志并控制蜂鸣器发出声音

## 概述

- 作业相关文档在 `docs` 目录下。
  - 在运行/浏览本项目的代码前，我们建议您先阅读[作业说明](docs/作业说明.md)
  - 关于车辆的机械结构/底盘结构/开发板的信息,请参考[此演示文档](docs/VehicleDetails.pptx)
- 固件在 `firmware` 目录下。
- 客户端软件在 `client` 目录下。
- 道路分割模型在 `roadseg` 目录下
- 交通标志识别在 `roadsign` 目录下


## 复现指南

我们了解复现是使用/理解他人代码的重中之重，也是验证该项目真实性的重要手段。
因此我们为本项目提供完整的复现指南，方便您核验本项目的真实性和复用/理解本项目中的代码。


### 初始化

1. 克隆本仓库
2. 更新 Git 子模块: `git submodule update --init` (为了安装 PaddleSeg 和 PaddleDetection)
3. 安装依赖(如果需要使用树莓派运行请参考下方说明编译安装 PaddlePaddle)
    - `pip install -r client/requirements.txt`
    - `pip install -r client/ai/PaddleDetection/requirements.txt`
    - `pip install -r client/ai/PaddleSeg/requirements.txt`

### 安装

#### 运行环境

以下两者均可
- ESP32 + 笔记本电脑
- ESP32 + 树莓派

---

- 硬件： 开发板和 ESP32-CAM
- 软件： PC 端 Python 3.10+, 在 Linux 下测试过，初期在 Windows 下测试过，不保证能在 Windows 上运行
- 热点
  - 方案一： 使用电脑或手机开一个热点
    - 不要离小车太远, 不要使用家里的 WiFi (除非路由器就在小车旁边), 因为 ESP32 板载天线功率不是特别大, 距离远会导致网络延迟过高.
  - 方案二： 将开发板置于 AP 模式， 用电脑连接开发板的热点
    - 注意：这会导致电脑无法连接到互联网(除非你有第二张无线网卡/连接有线网络), 但是 ESP32 摄像头和电脑之见的延迟比较低
    - 你可能需要调整电脑的位置，否则很容易车走着走着，电脑就被断开了 WiFi 连接！
    - 打开 `firmware/shared/lie-flat/src/config.h`, 修改 `AP_SSID` 和 `AP_PASSWORD`
    - 并且在下文的配置中把 `firmware/camera_web_server` 的 SSID 和密码 也一并修改为 `AP_SSID` 和 `AP_PASSWORD`
    - 修改 `firmware/web_ctrl/src/main.cpp`:
      ```diff
      // Be sure to define the WiFi mode before include lie-flat.h
      -#define LIE_FLAT_WIFI_STA
      +#define LIE_FLAT_WIFI_AP
      #include <lie-flat.h>
      ```

如果要在树莓派上运行交通标志识别，您需要手动编译 PaddlePaddle. (当然您也可以修改代码，改用 Paddle Lite)

- 在树莓派上从源代码编译 PaddlePaddle. 注意我们采用的是 Manjaro ARM64 发行版，不能使用 arm32 架构的发行版。
  - Reference: https://qengineering.eu/install-paddlepaddle-on-raspberry-pi-4.html
  - 假设你有良好的国际网络连接
  - 因为我们的树莓派在该项目中仅用于单一用途，所以我们不建立虚拟环境，直接使用系统 pip 安装包
  - `sudo pacman -S base-devel` 安装基础开发工具
  - `sudo pacman -S protobuf patchelf swig unrar cmake cython python-opencv yay`
  - 使用 `micromamba` 或 `conda` 配置一个**不含 Python 的环境**，安装 `gcc`, `gxx`, `gfortran` (前三者均安装 8.5 左右版本)
    - 这是因为 paddle 与系统带的 gcc 12 不兼容，强行使用 gcc 12 编译 paddle 会报错
    - 以下所有操作均在此环境下
  - `sudo pip3 install --upgrade six requests wheel pyyaml numpy protobuf matplotlib nltk scipy`
  - `git clone https://github.com/Qengineering/Paddle --depth=1`
  - 注意设置内存交换文件，我使用的是 4GB 内存版本，系统默认设置了 5.5GB 的 zram, 所以我不用在进行特殊设置了。
    - ```bash
      [kxxt@pi ~]$ swapon
      NAME       TYPE      SIZE USED PRIO
      /dev/zram0 partition 5.5G   0B  100
      ```
  - 进入 Build 目录： `cd Paddle && mkdir build && cd build`
  - cmake： 注意替换你的 Python 版本：
    ```bash
    cmake .. -D PY_VERSION=3.10 \
          -D WITH_PYTHON=ON \
          -D WITH_CONTRIB=OFF \
          -D WITH_MKL=OFF \
          -D WITH_MKLDNN=OFF \
          -D WITH_XBYAK=OFF \
          -D WITH_TESTING=OFF \
          -D WITH_ARM=ON \
          -D ON_INFER=ON \
          -D CMAKE_BUILD_TYPE=Release
    ```
  - `make -j4`
  - 若第一次编译失败，根据此处指示给代码打补丁(注意文件路径稍有不同)： https://github.com/facebookincubator/gloo/issues/332#issuecomment-1146812765
  - 然后再运行 `make -j4`
  - 最后你会得到一个 `whl` 文件，使用 `pip` 来安装它

#### 开发环境

- Visual Studio Code 
- Git v2+
- VSCode 的 PlatformIO 插件
- Arduino IDE v1.8~v2.0.0-rc9

#### 配置

- 打开 `firmware/shared/lie-flat/src/config.h`, 修改 WiFi 的 SSID 和密码
- 按照 `firmware/shared/lie-flat/src/pin.h`，完成开发板接线。

####  接线

##### 摄像头

<img src="README.assets/IMG_20220809_193332.jpg" alt="IMG_20220809_193332" style="zoom:10%;" />

5V 和 GND 接开发板的 5V 和 GND

##### 舵机

舵机接开发板舵机B(注意不要接反)

##### 蜂鸣器

一根杜邦线连接开发板上的中间两个管脚

##### 电机

积木连接

##### 光电传感器/测速计/optocoupler

信号线接 GPIO14, VCC 和 GND 接开发板.

##### MPU6050 六自由度运动传感器

SCL/SDA 接开发板 SCL/SDA

VCC/GND 照旧

#### 固件安装


- 使用 Arduino IDE 打开 `firmware/camera_web_server`，修改 WiFi 的 SSID 和密码，与配置步骤中的一样, 下载到 ESP32-CAM 上。
  - 注意：若要使开发板和摄像头连接到外部 WiFi 网络, 请构建并下载 `commit 100c1e09c726fd642b3a13418302a3cbdd9909ee` 时的 `firmware/camera_web_server` 到 ESP32-CAM， 之后的版本只适用于让电脑/树莓派连接到开发板的热点这一情型。
- 若要使开发板和摄像头连接到外部 WiFi 网络，使用 VSCode + PlatformIO 打开 `firmware/web_ctrl`, 构建并下载到开发板上。
- 若要让电脑/树莓派连接到开发板的热点，使用 VSCode + PlatformIO 打开 `firmware/main`, 构建并下载到开发板上。

## 启动

- PC 连接到同一热点
- 开启电源
- WiFi 配置
  - 方案1: 使用 ESP32 热点，将树莓派/电脑连接到此热点，静态IP，将以下内容写入 `devices.cache.json`: `{"board": "192.168.4.1", "esp32-cam": "192.168.4.2"}`
  - 方案2: 使用手机/电脑热点，开发板和摄像头连接到此热点，动态IP，脚本会自动寻找设备。
    - 在第一次运行之后, 脚本会自动生成设备缓存 `devices.cache.json`
    - 如果遇到连接问题(更换热点/IP变更),请删除 `devices.cache.json` 然后重新启动脚本.
- 运行主程序
  - 不使用树莓派：运行 `client/run.py`,
  - 使用树莓派：在树莓派上运行 `client/start.sh`
- 脚本会自动连接开发板，连接成功后，开发板蜂鸣器报警 1.5 秒。
