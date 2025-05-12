#include "preprocess.h"
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

/**
 * @brief 构造函数
 */
Preprocessor::Preprocessor() {}

/**
 * @brief 前处理：读取 RAW 图像并提取 Bayer 通道
 */

cv::Mat Preprocessor::preprocess(const std::string& img_path) {
    try {
        // 读取 RAW 图像
        std::ifstream file(img_path, std::ios::binary);
        if (!file) {
            std::cerr << "无法打开 RAW 图像: " << img_path << std::endl;
            return cv::Mat();
        }

        std::vector<uchar> buffer(RAW_HEIGHT * RAW_WIDTH);
        file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        file.close();

        // 转换为 cv::Mat
        cv::Mat raw_image(RAW_HEIGHT, RAW_WIDTH, CV_8UC1, buffer.data());

        // 提取 Bayer 通道
        int height = raw_image.rows;
        int width = raw_image.cols;

        cv::Mat ch_R(height / 2, width / 2, CV_8UC1);
        cv::Mat ch_Gr(height / 2, width / 2, CV_8UC1);
        cv::Mat ch_Gb(height / 2, width / 2, CV_8UC1);
        cv::Mat ch_B(height / 2, width / 2, CV_8UC1);

        for (int i = 0; i < height; i += 2) {
            for (int j = 0; j < width; j += 2) {
                ch_R.at<uchar>(i / 2, j / 2) = raw_image.at<uchar>(i, j);
                ch_Gr.at<uchar>(i / 2, j / 2) = raw_image.at<uchar>(i, j + 1);
                ch_Gb.at<uchar>(i / 2, j / 2) = raw_image.at<uchar>(i + 1, j);
                ch_B.at<uchar>(i / 2, j / 2) = raw_image.at<uchar>(i + 1, j + 1);
            }
        }

        // 合并为 4 通道图像
        std::vector<cv::Mat> channels = {ch_R, ch_Gr, ch_Gb, ch_B};
        cv::Mat merged;
        cv::merge(channels, merged);
        merged.convertTo(merged, CV_32FC4);
        return merged;
    } catch (const std::exception& e) {
        std::cerr << "前处理错误: " << e.what() << std::endl;
        return cv::Mat();
    }
}