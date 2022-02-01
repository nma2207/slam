#include "onnx_nextwork.h"

#include <opencv2/dnn/dnn.hpp>

namespace {

Ort::SessionOptions getDefaultSessionOption()
{
    Ort::SessionOptions options;
    options.SetInterOpNumThreads(1);
    options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
    return options;
}

} // namespace

image_process::OnnxNextwork::OnnxNextwork(const std::string& modelPath)
    : m_env{ORT_LOGGING_LEVEL_WARNING}
    , m_session{m_env, modelPath.c_str(), getDefaultSessionOption()}
{
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
    auto outputShape = m_session.GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    std::vector<int> shape = *reinterpret_cast<std::vector<int>*>(&outputShape);
    float* floatarr = outputTensor.front().GetTensorMutableData<float>();
    cv::Mat predict{shape, CV_32F, floatarr};
    return predict;
}
