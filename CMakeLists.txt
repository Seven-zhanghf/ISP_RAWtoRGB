# 设置最低版本号
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
# 设置项目名称
project(isp VERSION 0.0.1 LANGUAGES CXX)

# 输出系统信息
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")

# 设置编译器
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库架构
set(LIB_ARCH "aarch64")
set(DEVICE_NAME "RK3588")

# 设置RKNN
#  rknn_api 文件夹路径
set(RKNN_API_PATH ${CMAKE_CURRENT_SOURCE_DIR}/librknn_api)
#  rknn_api include 路径
set(RKNN_API_INCLUDE_PATH ${RKNN_API_PATH}/include)
#  rknn_api lib 路径
set(RKNN_API_LIB_PATH ${RKNN_API_PATH}/${LIB_ARCH}/librknnrt.so)
# 输出RKNN信息
message(STATUS "    RKNN include path: ${RKNN_API_INCLUDE_PATH}")

# OpenCV
find_package(OpenCV REQUIRED)
# 输出OpenCV信息
message(STATUS "    OpenCV include path: ${OpenCV_INCLUDE_DIRS}")

# 用来搜索头文件的目录
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${RKNN_API_INCLUDE_PATH}
)

# 设置源文件和头文件
set(SOURCES
    src/preprocess/preprocess.cpp
    src/Inference/Infer.cpp
    src/postprocess/postprocess.cpp
    src/ImageProcessor.cpp
    src/main.cpp
)
set(HEADERS
    src/preprocess/preprocess.h
    src/Inference/Infer.h
    src/postprocess/postprocess.h
    src/ImageProcessor.h
)
# 测试NPU：rknn
add_executable(isp ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(isp
        ${RKNN_API_LIB_PATH}
        ${OpenCV_LIBS}
)
