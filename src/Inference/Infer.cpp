#include "Infer.h"
#include <stdexcept>
#include <chrono>
#include <iostream>

/**
 * @brief 构造函数，初始化成员变量
 */
RKNNInferencer::RKNNInferencer() : ctx(0), model(nullptr), model_len(0) {}

/**
 * @brief 析构函数，释放资源
 */
RKNNInferencer::~RKNNInferencer() {
    if (ctx > 0) {
        rknn_destroy(ctx);
    }
    if (model) {
        free(model);
    }
}

/**
 * @brief 初始化模型
 */
void RKNNInferencer::initialize(const std::string& model_path) {
    // 加载模型
    model = load_model(model_path.c_str(), &model_len);
    if (!model) {
        throw std::runtime_error("无法加载模型: " + model_path);
    }

    // 初始化 RKNN 模型 输入： ctx(模型句柄)，model(模型权重指针)，model_len(模型大小)
    int ret = rknn_init(&ctx, model, model_len, 0, nullptr);
    if (ret < 0) {
        throw std::runtime_error("rknn_init 失败，错误码: " + std::to_string(ret));
    }

    // 设置推理核心数
    rknn_core_mask core_mask = RKNN_NPU_CORE_0_1_2;
    ret = rknn_set_core_mask(ctx, core_mask);
    if (ret < 0) {
        throw std::runtime_error("rknn_set_core_mask 失败，错误码: " + std::to_string(ret));
    }
}

/**
 * @brief 推理
 */
bool RKNNInferencer::infer(const cv::Mat& input_img, float** output_data, int& output_size) {
    try {
        if (input_img.empty() || input_img.type() != CV_32FC4) {
            throw std::runtime_error("无效的输入图像");
        }

        // 获取模型输入输出信息
        rknn_input_output_num io_num;
        int ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
        if (ret != RKNN_SUCC) {
            throw std::runtime_error("rknn_query 失败，错误码: " + std::to_string(ret));
        }

        // 设置输入
        rknn_input inputs[1];   // 使用rknn_input结构体存储模型输入信息
        memset(inputs, 0, sizeof(inputs));  // 初始化，将inputs中前sizeof(inputs)个字节用0替换

        // 设置模型输入的索引、类型、大小、格式、数据
        inputs[0].pass_through = 0;
        inputs[0].index = 0;
        inputs[0].type = RKNN_TENSOR_FLOAT32;
        inputs[0].size = input_img.cols * input_img.rows * input_img.channels() * sizeof(float);    //float uint16_t
        inputs[0].fmt = RKNN_TENSOR_NHWC;
        inputs[0].buf = input_img.data;

        ret = rknn_inputs_set(ctx, io_num.n_input, inputs);
        if (ret < 0) {
            throw std::runtime_error("rknn_inputs_set 失败，错误码: " + std::to_string(ret));
        }

        // 运行推理
        auto inference_start = std::chrono::high_resolution_clock::now();
        // 使用rknn_run函数运行RKNN模型 输入：ctx(模型句柄)，nullptr(保留参数)，返回值(<0：失败)
        ret = rknn_run(ctx, nullptr);
        if (ret < 0) {
            throw std::runtime_error("rknn_run 失败，错误码: " + std::to_string(ret));
        }
        auto inference_end = std::chrono::high_resolution_clock::now();
        std::cout << "推理时间: " << std::chrono::duration<double>(inference_end - inference_start).count() << " 秒" << std::endl;

        // 获取输出
        rknn_output outputs[1]; // 使用rknn_output结构体存储模型输出信息
        memset(outputs, 0, sizeof(outputs));
        outputs[0].want_float = 1;  // 设置模型输出类型为float
        // 使用rknn_outputs_get函数获取模型输出 -> 输入：ctx(模型句柄)，1(模型输出数量)，outputs(模型输出信息)，nullptr(保留参数)
        ret = rknn_outputs_get(ctx, 1, outputs, nullptr);
        if (ret < 0) {
            throw std::runtime_error("rknn_outputs_get 失败，错误码: " + std::to_string(ret));
        }

        *output_data = (float*)outputs[0].buf;  // 模型输出数据
        output_size = outputs[0].size;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "推理错误: " << e.what() << std::endl;
        *output_data = nullptr;
        output_size = 0;
        return false;
    }
}

/**
 * @brief 释放推理输出数据
 */
void RKNNInferencer::releaseOutput(float* output_data) {
    if (output_data) {
        rknn_output outputs[1];
        memset(outputs, 0, sizeof(outputs));
        outputs[0].buf = output_data;
        outputs[0].want_float = 1;
        rknn_outputs_release(ctx, 1, outputs);
    }
}

/**
 * @brief 加载模型文件
 */
unsigned char* RKNNInferencer::load_model(const char* filename, int* model_size) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        std::cerr << "打开文件 " << filename << " 失败!" << std::endl;
        return nullptr;
    }
    fseek(fp, 0, SEEK_END);
    int model_length = ftell(fp);
    unsigned char* model = (unsigned char*)malloc(model_length);    // 申请模型大小的内存，返回指针
    fseek(fp, 0, SEEK_SET);
    if (model_length != fread(model, 1, model_length, fp)) {    //先将fp读取到model中，再进行if判断
        std::cerr << "读取文件 " << filename << " 失败!" << std::endl;
        free(model);
        fclose(fp);
        return nullptr;
    }
    *model_size = model_length;
    fclose(fp);
    return model;
}
