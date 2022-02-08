#include "onnx_nextwork.h"

#include <numeric>

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc.hpp>

namespace {

Ort::SessionOptions getDefaultSessionOption()
{
    Ort::SessionOptions options;
    options.SetInterOpNumThreads(1);
    options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
    return options;
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
    m_outputShape = outputTypeInfo.GetTensorTypeAndShapeInfo().GetShape();
    m_outputName = {m_session.GetOutputName(0, m_allocator)};
}

cv::Mat image_process::OnnxNextwork::predict(const cv::Mat &input)
{
    const int width = m_inputShape[2];
    const int height = m_inputShape[3];

    cv::Mat resized;
    cv::resize(input, resized, cv::Size{width, height});
    cv::Mat blob = cv::dnn::blobFromImage(resized, 1/255, cv::Size{width, height}, cv::Scalar{}, true);
    float* inputTensorValues = reinterpret_cast<float*>(blob.data);

    Ort::Value inputTensor = Ort::Value::CreateTensor<float>
            (m_memoryInfo, inputTensorValues, blob.total(), m_inputShape.data(), m_inputShape.size());

    std::vector<Ort::Value> outputTensors = m_session.Run(
                Ort::RunOptions{nullptr}, m_inputName.data(), &inputTensor, 1, m_outputName.data(), 1);

    Ort::Value& outputTensor = outputTensors.front();
    float* floatarr = outputTensor.GetTensorMutableData<float>();
    cv::Mat predict{toInt(m_outputShape), CV_32F, floatarr};
    return predict;
}
