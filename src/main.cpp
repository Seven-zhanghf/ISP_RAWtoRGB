#include "ImageProcessor.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>

int main(int argc, char **argv) {

    ImageProcessor processor;
    const char *model_path = argv[1];
    const char *img_path = argv[2];
    const char *save_path = argv[3];
    // 方法 1：使用整合接口
    // cv::Mat result = processor.processImage(model_path, img_path, save_path);
    // if (!result.empty()) {
    //     std::cout << "处理成功！" << std::endl;
    // } else {
    //     std::cout << "处理失败。" << std::endl;
    // }


    // 方法 2：单独调用各模块
    Preprocessor preprocessor;
    RKNNInferencer inferencer;
    Postprocessor postprocessor;

    // 初始化模型
    inferencer.initialize(model_path);

    // 前处理
    cv::Mat input_img = preprocessor.preprocess(img_path);
    if (input_img.empty()) {
        std::cerr << "前处理失败" << std::endl;
        return -1;
    }

    // 推理
    float* output_data = nullptr;
    int output_size = 0;
    if (!inferencer.infer(input_img, &output_data, output_size)) {
        std::cerr << "推理失败" << std::endl;
        return -1;
    }

    // 后处理
    cv::Mat output_img = postprocessor.postprocess(output_data, 1, 3, 1200, 1920);
    inferencer.releaseOutput(output_data);
    if (output_img.empty()) {
        std::cerr << "后处理失败" << std::endl;
        return -1;
    }

    // 保存结果
    cv::imwrite(save_path, output_img);
    std::cout << "结果已保存至 " << save_path << std::endl;

    return 0;
}