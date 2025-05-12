#include "ImageProcessor.h"
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>
#include <iostream>

/**
 * @brief 构造函数，初始化子模块
 */
ImageProcessor::ImageProcessor() {}

/**
 * @brief 完整处理 RAW 图像
 */
cv::Mat ImageProcessor::processImage(const std::string& model_path, const std::string& img_path, const std::string& save_path) {
    try {
        // 步骤 1: 初始化模型
        inferencer.initialize(model_path);

        // 步骤 2: 前处理
        cv::Mat input_img = preprocessor.preprocess(img_path);
        if (input_img.empty()) {
            throw std::runtime_error("图像前处理失败");
        }

        // 步骤 3: 推理
        float* output_data = nullptr;
        int output_size = 0;
        if (!inferencer.infer(input_img, &output_data, output_size)) {
            throw std::runtime_error("推理失败");
        }

        // 步骤 4: 后处理
        cv::Mat output_img = postprocessor.postprocess(output_data, 1, 3, 1200, 1920);
        inferencer.releaseOutput(output_data); // 释放输出数据

        // 步骤 5: 保存输出（如果提供路径）
        if (!save_path.empty()) {
            cv::imwrite(save_path, output_img);
            std::cout << "结果已保存至 " << save_path << std::endl;
        }

        return output_img;
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return cv::Mat();
    }
}