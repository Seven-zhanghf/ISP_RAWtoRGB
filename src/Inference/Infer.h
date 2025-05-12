#ifndef RKNN_INFERENC_H
#define RKNN_INFERENC_H

#include <opencv2/core.hpp>
#include <string>
#include "../../librknn_api/include/rknn_api.h"

class RKNNInferencer {

public:
    // 构造函数
    RKNNInferencer();
    // 析构函数
    ~RKNNInferencer();

    /**
     * @brief 初始化模型
     * @param model_path RKNN 模型文件路径
     */
    void initialize(const std::string& model_path);

    /**
     * @brief 推理：使用 RKNN 模型处理输入图像
     * @param input_img 预处理后的图像 (CV_32FC4, 600x960x4)
     * @param output_data 推理输出数据（由 RKNN 分配，需通过 releaseOutput 释放）
     * @param output_size 输出数据大小（字节）
     * @return 是否成功
     */
    bool infer(const cv::Mat& input_img, float** output_data, int& output_size);

    /**
     * @brief 释放推理输出数据
     * @param output_data 推理输出数据
     */
    void releaseOutput(float* output_data);

private:
    // 加载模型文件
    unsigned char* load_model(const char* filename, int* model_size);

    // RKNN 上下文和模型数据
    rknn_context ctx;
    unsigned char* model;
    int model_len;

    // 图像尺寸常量
    static constexpr int MODEL_IN_HEIGHT = 600;
    static constexpr int MODEL_IN_WIDTH = 960;
    static constexpr int MODEL_IN_CHANNELS = 4;

};




#endif // RKNN_INFERENC_H
