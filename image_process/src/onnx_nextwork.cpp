#include "onnx_nextwork.h"

#include <opencv2/dnn/dnn.hpp>
image_process::OnnxNextwork::OnnxNextwork(const std::string& modelPath)
    : m_env{ORT_LOGGING_LEVEL_WARNING}
    , m_sessionOptions{}
    , m_session{m_env, modelPath.c_str(), m_sessionOptions}
{
    m_sessionOptions.SetInterOpNumThreads(1);
    m_sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
    m_session =Ort::Session{m_env, modelPath.c_str(), m_sessionOptions};
}

cv::Mat image_process::OnnxNextwork::predict(const cv::Mat &input)
{
    cv::Mat blob = cv::dnn::blobFromImage(input, 1/255, cv::Size{}, cv::Scalar{}, true);
    float* inputTensorValues = reinterpret_cast<float*>(blob.data);
    auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    const size_t inputSize = input.rows * input.dims * 3;
    Ort::TypeInfo typeInfo = m_session.GetInputTypeInfo(0);
    auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
    ONNXTensorElementDataType type = tensorInfo.GetElementType();
    Ort::AllocatorWithDefaultOptions allocator;
    std::vector<const char*> outputNodeNames = {"softmaxout_1"};
    // print input shapes/dims
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo,
                                                             inputTensorValues, inputSize,
                                                             tensorInfo.GetShape().data(),4);
    std::vector<char*> inputName = {m_session.GetInputName(0, allocator)};
    auto outputTensor = m_session.Run(Ort::RunOptions{nullptr}, inputName.data(), &inputTensor, 1,
                                      outputNodeNames.data(), 1);
    m_session.GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    float* floatarr = outputTensor.front().GetTensorMutableData<float>();

    return cv::Mat{};
}
