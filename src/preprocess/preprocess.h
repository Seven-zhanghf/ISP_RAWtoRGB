#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/core.hpp>
#include <string>

/**
 * @brief 图像前处理器类，负责读取 RAW 图像并提取 Bayer 通道
 */
class Preprocessor {
public:
    // 构造函数
    Preprocessor();

    /**
     * @brief 前处理：读取 RAW 图像并提取 Bayer 通道
     * @param img_path 输入 RAW 图像文件路径
     * @return 预处理后的图像 (CV_32FC4, 600x960x4)，失败时返回空 Mat
     */
    cv::Mat preprocess(const std::string& img_path);

private:
    // 图像尺寸常量
    static constexpr int RAW_HEIGHT = 1200;
    static constexpr int RAW_WIDTH = 1920;
};

#endif // IMAGE_PREPROCESS_H