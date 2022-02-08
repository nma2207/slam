#include "../include/neuralnet_processor.h"

#include <iostream>

image_process::NeuralNetProcessor::NeuralNetProcessor(const std::string &modelPath)
    : m_network(modelPath)
{

}

cv::Mat image_process::NeuralNetProcessor::process(const cv::Mat& img)
{
    cv::Mat result = m_network.predict(img);
    std::cout << result << std::endl;
    return {};
}
