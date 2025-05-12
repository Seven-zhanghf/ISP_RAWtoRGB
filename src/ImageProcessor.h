#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "./preprocess/preprocess.h"
#include "./Inference/Infer.h"
#include "./postprocess/postprocess.h"
#include <string>

/**
 * @brief RKNN 图像处理器类，整合前处理、推理和后处理
 */
class ImageProcessor {
public:
    // 构造函数
    ImageProcessor();

    /**
     * @brief 完整处理 RAW 图像
     * @param model_path RKNN 模型文件路径
     * @param img_path 输入 RAW 图像文件路径
     * @param save_path 输出图像保存路径（可选，空字符串表示不保存）
     * @return 处理后的图像 (CV_8UC3, 1200x1920x3)，失败时返回空 Mat
     */
    cv::Mat processImage(const std::string& model_path, const std::string& img_path, const std::string& save_path = "");

private:
    Preprocessor preprocessor;
    RKNNInferencer inferencer;
    Postprocessor postprocessor;
};

#endif // RKNN_IMAGE_PROCESSOR_H