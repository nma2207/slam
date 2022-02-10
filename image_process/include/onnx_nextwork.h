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
public:
    explicit OnnxNextwork(const std::string& modelPath);
    //void loadModel(const std::string& modelPath);
    cv::Mat process(const cv::Mat& image) override;

private:
    cv::Mat preprocess(const cv::Mat& image);
    cv::Mat postprocess(const std::vector<cv::Mat>& predicts);
    //std::vector<cv::Mat> predict(const cv::Mat& input);

    Ort::Env m_env;
    Ort::Session m_session;
    Ort::AllocatorWithDefaultOptions m_allocator;
    Ort::MemoryInfo m_memoryInfo;

    std::vector<const char*> m_inputName;
    std::vector<int64_t> m_inputShape;

    std::vector<const char*> m_outputName;
    std::vector<int64_t> m_outputShape;
};

} // namespace image_process
