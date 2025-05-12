# ISP 项目文档

## 项目概述

AIISP（AI Image Signal Processing）是一个基于深度学习以及 C++ 推理的图像处理项目，旨在处理 RAW 格式图像并通过 RKNN 模型进行推理，生成 RGB 图像。项目推理部分采用模块化设计，将前处理、推理和后处理分离为独立模块，有清晰的接口和灵活的使用方式，硬件平台为RK3588。

### 主要功能
- **前处理**：读取 1200x1920 单通道 RAW 图像，提取 Bayer 通道（RGGB），生成 600x960x4（float32）输入。
- **推理**：使用 RKNN 模型处理输入图像，输出 1200x1920x3（float32，NCHW）数据。
- **后处理**：将模型输出转换为 1200x1920x3（uint8，RGB） RGB图像。
- **整合接口**：提供端到端处理接口，简化调用。

### 模块结构
- `Preprocessor`：负责 RAW 图像前处理。
- `Inferencer`：负责 RKNN 模型加载和推理。
- `Postprocessor`：负责模型输出后处理。
- `ImageProcessor`：整合上述模块，提供便捷接口。

## 依赖

- **OpenCV**：用于图像处理，本项目使用 4.8.0 版本
- **RKNN API**：Rockchip 提供的神经网络推理库（需包含 `librknnrt.so` 和头文件），本项目使用 rknn-toolkit2-v2.3.0 版本
- **CMake**：版本 3.16.3
- **C++ 编译器**：支持 C++14（如 g++）
- **操作系统**：Linux（针对 RK3588 平台，aarch64 架构）

## 安装

### 1. 安装依赖
#### OpenCV 
- 自行安装

#### RKNN API 

确保 `librknn_api` 目录包含：
- `include/`：RKNN 头文件（如 `rknn_api.h`）。
- `aarch64/librknnrt.so`：RKNN 运行时库。
从 Rockchip 官方获取：https://github.com/airockchip/rknn-toolkit2

#### CMake
```bash
sudo apt-get install cmake
```

### 2. 克隆项目
```bash
git clone <repository_url>
cd ISP_RAWtoRGB
```

### 3. 目录结构
```
ISP_RAWtoRGB/
├── CMakeLists.txt
├── librknn_api/
│   ├── aarch64/
│   │   ├── librknnrt.so
│   ├── include/
│   │   ├── rknn_api.h
│   │   ├── rknn_custom_op.h
│   │   ├── rknn_matmul_api.h
│   ├── rknn_server/
│   │   ├── restart_rknn.sh
│   │   ├── rknn_server
│   │   ├── start_rknn.sh
├── raw_images/
│   ├── 0.raw 1.raw 2.raw 3.raw（示例图片）
├── results/
│   ├── 0.jpg 1.jpg 2.jpg 3.jpg（输出示例）
├── src/
│   ├── Inference/
│   │   ├── Infer.h
│   │   ├── Infer.cpp
│   ├── postprocess/
│   │   ├── postprocess.h
│   │   ├── postprocess.cpp
│   ├── preprocess/
│   │   ├── preprocess.h
│   │   ├── preprocess.cpp
│   ├── ImageProcessor.cpp
│   ├── ImageProcessor.h
│   ├── main.cpp
├── weights
│   ├── ISP_RAWtoRGB.rknn
```

### 4. 编译
```bash
cmake -S . -B build && cmake --build build
```

## 用法

### 1. 准备输入
- **模型文件**：RKNN 模型文件（`.rknn`）， `weight`文件夹下的`ISP_RAWtoRGB.rknn`权重文件。
- **输入图像**：1200x1920 单通道 RAW 图像，`raw_images`文件夹中。
- **输出路径**：指定输出图像路径， `./results`。

### 2. 运行
#### 使用整合接口
```bash
./build/ISP_RAWtoRGB ./weights/ISP_RAWtoRGB.rknn ./raw_images/0.raw ./results/0.jpg
```
分别指定权重文件，RAW文件路径，以及输出图片的路径

### 3. 输出
- 输出图像：`0.jpg`（RGB，1200x1920x3）。
- 推理时间会打印到控制台。

## 项目结构

- **librknn_api/**：RKNN API 库。
- **raw_images/**：RAW格式的图像文件。
- **results/**：推理结果文件。
- **src/**：源文件。
- **weights/**：权重文件。
- **CMakeLists.txt**：CMake 配置文件。
- **README.md**：项目文档。

## 注意事项
- 输入 RAW 图像必须为 1200x1920 单通道（8 位）。
- RKNN 模型需支持 600x960x4（float32，NHWC）输入和 1200x1920x3（float32，NCHW）输出。
- 推理输出的 `float*` 数据需通过 `RKNNInferencer::releaseOutput` 释放。

## 贡献
欢迎提交 issue 或 pull request。

## 许可证
MIT License

Copyright (c) 2025 Seven

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
