#pragma once

#include <functional>
#include <string>

#include <opencv2/core.hpp>

#include <onnxruntime_cxx_api.h>

#include "image_processor.h"

namespace image_process
{

class OnnxNextwork : public ImageProcessor
{
    struct TensorInfo
    {
        using Shape = std::vector<int64_t>;

        std::vector<const char*> names = {};
        std::vector<Shape> shapes = {};
        size_t count = 0;
    };

public:
    explicit OnnxNextwork(const std::string& modelPath);
    //void loadModel(const std::string& modelPath);
    cv::Mat process(const cv::Mat& image) override;

private:
    cv::Mat preprocess(const cv::Mat& image);
    std::function<cv::Mat(const cv::Mat&,const std::vector<cv::Mat>&)> postprocess;
    //cv::Mat postprocess(const std::vector<cv::Mat>& predicts);
    //std::vector<cv::Mat> predict(const cv::Mat& input);

    Ort::Env m_env;
    Ort::Session m_session;
    Ort::AllocatorWithDefaultOptions m_allocator;
    Ort::MemoryInfo m_memoryInfo;

    TensorInfo m_input;
    TensorInfo m_output;
};

} // namespace image_process
