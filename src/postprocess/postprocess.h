#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <opencv2/core.hpp>

/**
 * @brief 图像后处理器类，负责将模型输出转换为 RGB 图像
 */
class Postprocessor {
public:
    // 构造函数
    Postprocessor();

    /**
     * @brief 后处理：将模型输出转换为 RGB 图像
     * @param output_data 推理输出数据
     * @param N 批次大小（默认 1）
     * @param C 通道数（默认 3）
     * @param H 输出高度（默认 1200）
     * @param W 输出宽度（默认 1920）
     * @return 处理后的图像 (CV_8UC3, 1200x1920x3)
     */
    cv::Mat postprocess(float* output_data, int N = 1, int C = 3, int H = 1200, int W = 1920);
};

#endif // IMAGE_POSTPROCESSOR_H