#include "onnx_nextwork.h"

#include <numeric>

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

namespace {

Ort::SessionOptions getDefaultSessionOption()
{
    Ort::SessionOptions options;
    options.SetInterOpNumThreads(1);
    options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
    return options;
}

template<class T>
void printVector(const std::vector<T>& arr)
{
    for(T a: arr)
        std::cout << a << " ";
    std::cout << std::endl;
}
std::vector<int> toInt(const std::vector<int64_t>& arr)
{
    std::vector<int> res;
    res.reserve(arr.size());
    for(const int64_t &a : arr)
        res.push_back(a);
    return res;
}

} // namespace

image_process::OnnxNextwork::OnnxNextwork(const std::string& modelPath)
    : m_env{ORT_LOGGING_LEVEL_WARNING}
    , m_session{m_env, modelPath.c_str(), getDefaultSessionOption()}
    , m_memoryInfo{Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)}
    , m_allocator{}
{


    Ort::TypeInfo inputTypeInfo = m_session.GetInputTypeInfo(0);
    m_inputShape = inputTypeInfo.GetTensorTypeAndShapeInfo().GetShape();
    m_inputName = {m_session.GetInputName(0, m_allocator)};


    Ort::TypeInfo outputTypeInfo = m_session.GetOutputTypeInfo(0);
    size_t k = m_session.GetOutputCount();
    m_outputShape = outputTypeInfo.GetTensorTypeAndShapeInfo().GetShape();
    m_outputName = {m_session.GetOutputName(0, m_allocator)};

    printVector(m_outputShape);
   m_inputShape[0] = 1;
   m_outputShape[0] = 1;
}

cv::Mat image_process::OnnxNextwork::predict(const cv::Mat &image)
{
    const int width = m_inputShape[2];
    const int height = m_inputShape[3];

    cv::Mat resizedImageBgr, resizedImageRgb, resizedImage, preprocessedImage;

    cv::resize(image, resizedImageBgr, cv::Size{width, height}, cv::INTER_CUBIC);

    cv::cvtColor(resizedImageBgr, resizedImageRgb, cv::COLOR_BGR2RGB);
    resizedImageRgb.convertTo(resizedImage, CV_32F, 1.0/255);

    cv::Mat channels[3];
    cv::split(resizedImage, channels);

    channels[0] = (channels[0] - 0.485) / 0.229;
    channels[1] = (channels[1] - 0.456) / 0.224;
    channels[2] = (channels[2] - 0.406) / 0.225;
    cv::merge(channels, 3, resizedImage);

    cv::dnn::blobFromImage(resizedImage, preprocessedImage);

    const size_t inputTensorSize = m_inputShape[1] *m_inputShape[2] * m_inputShape[3];


    std::vector<float> inputTensorValues(inputTensorSize);
    inputTensorValues.assign(preprocessedImage.begin<float>(), preprocessedImage.end<float>());



    Ort::Value inputTensor = Ort::Value::CreateTensor<float>
            (m_memoryInfo, inputTensorValues.data(), inputTensorSize, m_inputShape.data(), m_inputShape.size());

    std::vector<Ort::Value> outputTensors = m_session.Run(
                Ort::RunOptions{nullptr}, m_inputName.data(), &inputTensor, 1, m_outputName.data(), 1);

    Ort::Value& outputTensor = outputTensors.front();
    float* floatarr = outputTensor.GetTensorMutableData<float>();
    cv::Mat predict{toInt(m_outputShape), CV_32F, floatarr};
    return predict;
}
