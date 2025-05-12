# ISP 项目文档

## 项目概述

ISP（Image Signal Processing）是一个基于 C++ 的图像处理项目，旨在处理 RAW 格式图像并通过 RKNN（Rockchip Neural Network）模型进行推理，生成 RGB 图像。项目采用模块化设计，将前处理、推理和后处理分离为独立模块，提供清晰的接口和灵活的使用方式。

### 主要功能
- **前处理**：读取 1200x1920 单通道 RAW 图像，提取 Bayer 通道（RGGB），生成 600x960x4（float32）输入。
- **推理**：使用 RKNN 模型处理输入图像，输出 1200x1920x3（float32，NCHW）数据。
- **后处理**：将模型输出转换为 1200x1920x3（uint8，BGR）RGB 图像。
- **整合接口**：提供端到端处理接口，简化调用。

### 模块结构
- `ImagePreprocessor`：负责 RAW 图像前处理。
- `RKNNInferencer`：负责 RKNN 模型加载和推理。
- `ImagePostprocessor`：负责模型输出后处理。
- `RKNNImageProcessor`：整合上述模块，提供便捷接口。

## 依赖

- **OpenCV**：用于图像处理（版本 4.x 推荐）。
- **RKNN API**：Rockchip 提供的神经网络推理库（需包含 `librknnrt.so` 和头文件）。
- **CMake**：版本 3.11 或更高，用于构建项目。
- **C++ 编译器**：支持 C++14（如 g++）。
- **操作系统**：Linux（针对 RK3588 平台，aarch64 架构）。

## 安装

### 1. 安装依赖
#### OpenCV
```bash
sudo apt-get update
sudo apt-get install libopencv-dev
```

#### RKNN API
确保 `librknn_api` 目录包含：
- `include/`：RKNN 头文件（如 `rknn_api.h`）。
- `aarch64/librknnrt.so`：RKNN 运行时库。
从 Rockchip 官方获取，或联系硬件供应商。

#### CMake
```bash
sudo apt-get install cmake
```

### 2. 克隆项目
```bash
git clone <repository_url>
cd isp
```

### 3. 目录结构
```
isp/
├── CMakeLists.txt
├── include/
│   ├── ImagePreprocessor.h
│   ├── RKNNInferencer.h
│   ├── ImagePostprocessor.h
│   ├── RKNNImageProcessor.h
├── src/
│   ├── ImagePreprocessor.cpp
│   ├── RKNNInferencer.cpp
│   ├── ImagePostprocessor.cpp
│   ├── RKNNImageProcessor.cpp
│   ├── main.cpp
├── librknn_api/
│   ├── include/
│   │   ├── rknn_api.h
│   ├── aarch64/
│   │   ├── librknnrt.so
├── README.md
```

### 4. 编译
```bash
mkdir build && cd build
cmake ..
make
```

## 用法

### 1. 准备输入
- **模型文件**：RKNN 模型文件（`.rknn`），如 `model.rknn`。
- **输入图像**：1200x1920 单通道 RAW 图像，如 `input.raw`。
- **输出路径**：指定输出图像路径，如 `output.png`。

### 2. 运行
#### 使用整合接口
```bash
./isp
```

#### 单独调用模块
```cpp
#include "ImagePreprocessor.h"
#include "RKNNInferencer.h"
#include "ImagePostprocessor.h"
#include <iostream>

int main() {
    ImagePreprocessor preprocessor;
    RKNNInferencer inferencer;
    ImagePostprocessor postprocessor;

    std::string model_path = "model.rknn";
    std::string img_path = "input.raw";
    std::string save_path = "output.png";

    try {
        inferencer.initialize(model_path);
        cv::Mat input_img = preprocessor.preprocess(img_path);
        if (input_img.empty()) throw std::runtime_error("前处理失败");
        float* output_data = nullptr;
        int output_size = 0;
        if (!inferencer.infer(input_img, &output_data, output_size)) throw std::runtime_error("推理失败");
        cv::Mat output_img = postprocessor.postprocess(output_data, 1, 3, 1200, 1920);
        inferencer.releaseOutput(output_data);
        if (output_img.empty()) throw std::runtime_error("后处理失败");
        cv::imwrite(save_path, output_img);
        std::cout << "结果已保存至 " << save_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
```

### 3. 输出
- 输出图像：`output.png`（BGR，1200x1920x3）。
- 推理时间会打印到控制台。

## 项目结构

- **include/**：头文件。
- **src/**：源文件。
- **librknn_api/**：RKNN API 库。
- **CMakeLists.txt**：CMake 配置文件。
- **README.md**：项目文档。

## 注意事项
- 输入 RAW 图像必须为 1200x1920 单通道（8 位）。
- RKNN 模型需支持 600x960x4（float32，NHWC）输入和 1200x1920x3（float32，NCHW）输出。
- 推理输出的 `float*` 数据需通过 `RKNNInferencer::releaseOutput` 释放。
- 尺寸硬编码，动态尺寸需修改代码。

## 贡献
欢迎提交 issue 或 pull request。

## 许可证
MIT 许可证（待添加 `LICENSE` 文件）。