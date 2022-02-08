#pragma once

#include "image_processor.h"
#include "onnx_nextwork.h"

namespace image_process
{

class NeuralNetProcessor : public ImageProcessor
{
public:
    explicit NeuralNetProcessor(const std::string& modelPath);

    // ImageProcessor interface
public:
    cv::Mat process(const cv::Mat &) override;

private:
    OnnxNextwork m_network;
};

} // namespace image_processor;
