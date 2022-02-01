#pragma once

#include <string>

#include <opencv2/core.hpp>

#include "onnxruntime_cxx_api.h"

namespace image_process
{

class OnnxNextwork
{
public:
    explicit OnnxNextwork(const std::string& modelPath);
    //void loadModel(const std::string& modelPath);
    cv::Mat predict(const cv::Mat& input);

private:
    Ort::Env m_env;
    Ort::Session m_session;

};

} // namespace image_process
