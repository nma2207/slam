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

int64_t getSize(const std::vector<int64_t> shape)
{
    return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies());
}
cv::Mat onnxPostProcess(const std::vector<cv::Mat>&)
{
    return {};
}

} // namespace

image_process::OnnxNextwork::OnnxNextwork(const std::string& modelPath)
    : m_env{ORT_LOGGING_LEVEL_WARNING}
    , m_session{m_env, modelPath.c_str(), getDefaultSessionOption()}
    , m_memoryInfo{Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)}
    , m_allocator{}
{
    std::cout << "Input count" << m_session.GetInputCount() << std::endl;
    m_input.count = m_session.GetInputCount();
    m_input.shapes.reserve(m_input.count);
    for(size_t i = 0; i < m_input.count; i++)
    {
        Ort::TypeInfo typeInfo = m_session.GetInputTypeInfo(i);
        m_input.shapes.push_back(typeInfo.GetTensorTypeAndShapeInfo().GetShape());
        m_input.names.push_back(m_session.GetInputName(i, m_allocator));
        for(auto& t: m_input.shapes[i])
        {
            if (t < 0)
                t = 1;
        }
    }


    std::cout << "Output count" << m_session.GetOutputCount() << std::endl;
    m_output.count = m_session.GetOutputCount();
    m_output.shapes.reserve(m_output.count);
    for(size_t i = 0; i < m_output.count; i++)
    {
        Ort::TypeInfo typeInfo = m_session.GetOutputTypeInfo(i);
        m_output.shapes.push_back(typeInfo.GetTensorTypeAndShapeInfo().GetShape());
        m_output.names.push_back(m_session.GetOutputName(i, m_allocator));
    }

    postprocess = onnxPostProcess;
}

cv::Mat image_process::OnnxNextwork::process(const cv::Mat &image)
{
    cv::Mat preprocessedImage = preprocess(image);

    const size_t inputTensorSize = m_input.shapes[0][1] * m_input.shapes[0][2] * m_input.shapes[0][3];


    std::vector<float> inputTensorValues(inputTensorSize);
    inputTensorValues.assign(preprocessedImage.begin<float>(), preprocessedImage.end<float>());


    Ort::Value inputTensor = Ort::Value::CreateTensor<float>
            (m_memoryInfo, inputTensorValues.data(), inputTensorSize, m_input.shapes[0].data(), m_input.shapes[0].size());

    std::vector<Ort::Value> outputTensors = m_session.Run(
                Ort::RunOptions{nullptr}, m_input.names.data(), &inputTensor, 1, m_output.names.data(), m_output.count);

    std::vector<cv::Mat> predicts;
    predicts.reserve(outputTensors.size());

    for(auto& tensor : outputTensors)
    {

        auto shape = tensor.GetTensorTypeAndShapeInfo().GetShape();
        int64_t count = getSize(shape);
        std::cout << "Output count" << count;
        const float *arr = tensor.GetTensorData<float>();
        std::vector<float> vec;
        vec.assign(arr, arr+count);
        cv::Mat predict{toInt(shape), CV_32F, vec.data()};
        predicts.push_back(predict);
    }

    //return postprocess(predicts);
    return image;
}

cv::Mat image_process::OnnxNextwork::preprocess(const cv::Mat &image)
{
    const int width = m_input.shapes[0][2];
    const int height = m_input.shapes[0][3];

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

    return preprocessedImage;
}


