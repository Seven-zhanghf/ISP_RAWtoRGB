#include "postprocess.h"
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <iostream>

/**
 * @brief 构造函数
 */
Postprocessor::Postprocessor() {}

/**
 * @brief 后处理：将模型输出转换为 RGB 图像
 */
cv::Mat Postprocessor::postprocess(float* output_data, int N, int C, int H, int W) {
    try {
        if (!output_data) {
            throw std::runtime_error("无效的输出数据");
        }

        // 创建输出图像
        cv::Mat output_img(H, W, CV_32FC3);
        // 填充 RGB 通道
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W; ++w) {
                for (int c = 0; c < C; ++c) {
                    int idx = c * H * W + h * W + w; // NCHW
                    output_img.at<cv::Vec3f>(h, w)[c] = output_data[idx];
                }
            }
        }
        // 转换为 8 位图像（归一化）
        output_img = output_img * 255;
        output_img.convertTo(output_img, CV_8UC3);
        // 将 RGB 图像从 RGB 转换到 BGR（OpenCV 默认 BGR 格式）
        cv::cvtColor(output_img, output_img, cv::COLOR_RGB2BGR);
        return output_img;
    } catch (const std::exception& e) {
        std::cerr << "后处理错误: " << e.what() << std::endl;
        return cv::Mat();
    }
}