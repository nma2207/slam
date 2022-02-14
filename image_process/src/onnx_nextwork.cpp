#include "onnx_nextwork.h"

#include <numeric>

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>

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

std::vector<std::string> loadClassNames(const std::string& path)
{
    // load class names
    std::cout << "Load class names";
    std::vector<std::string> classNames;
    std::ifstream infile(path);
    if (infile.good())
    {
        std::string line;
        while (getline (infile, line))
        {
            if (line.back() == '\r')
                line.pop_back();
            classNames.emplace_back(line);
        }
        infile.close();
    }
    else
    {
        std::cerr << "ERROR: Failed to access class name path: " << path << std::endl;
    }

    return classNames;
}

std::pair<int, float> getBestClass(const cv::Mat& output, int row)
{
    constexpr int classCount = 80;
    std::pair<int, float> bestClass = {0, output.at<float>(0, row, 5)};
    for(int i = 1; i<classCount; i++)
    {
        float conf = output.at<float>(0, row, i+5);
        if (conf > bestClass.second)
        {
            bestClass = {i, conf};
        }
    }
    return bestClass;
}

cv::Mat onnxPostProcess(const cv::Mat& input,const std::vector<cv::Mat>& outputs)
{
    static const std::vector<std::string> classNames = loadClassNames("classnames.txt");
    constexpr float confThreshold = 0.25f;
    constexpr float iouThreshold = 0.4f;
    const cv::Mat& output = outputs.front();
    //std::cout << output.size << std::endl;
    const int n = output.size[1];
    int k =0;
    std::vector<cv::Rect> boxes;
    std::vector<float> confs;
    std::vector<int> classes;
    for (int i = 0; i < n; i++)
    {
        float score = output.at<float>(0, i, 4);
        if (score > confThreshold)
        {
            int x = output.at<float>(0, i, 0);
            int y = output.at<float>(0, i, 1);
            int width = output.at<float>(0, i, 2);
            int height = output.at<float>(0, i, 3);
            x -= width /2;
            y -= height/2;
            auto best = getBestClass(output, i);
            boxes.emplace_back(cv::Rect(x, y, width, height));
            confs.emplace_back(best.second * score);
            classes.emplace_back(best.first);
            //std::cout << x << " " << y <<" " << width <<" " << height << std::endl;


        }
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confs, confThreshold, iouThreshold, indices);

    for(int i : indices)
    {
        //std::cout << classNames[classes[i]];
        int x = boxes[i].x;
        int y = boxes[i].y;

        int conf = confs[i] * 100;
        cv::rectangle(input, boxes[i], cv::Scalar((1 - confs[i])* 255,255 * confs[i], 127), 2);
        std::string label =  classNames[classes[i]] + "0." +std::to_string(conf);
        int baseline = 0;
        cv::Size size = cv::getTextSize(label, cv::FONT_ITALIC, 0.8, 2, &baseline);
        cv::rectangle(input,
                      cv::Point(x, y - 25), cv::Point(x + size.width, y),
                      cv::Scalar(229, 160, 21), -1);

        cv::putText(input, label,
                    cv::Point(x, y - 3), cv::FONT_ITALIC,
                    0.8, cv::Scalar(255, 255, 255), 2);
    }
    return input;
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
    }
    m_input.shapes[0][0] = 1;
    m_input.shapes[0][2] = 480;
    m_input.shapes[0][3] = 480;


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

    std::vector<Ort::Value> inputTensors;
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>
            (m_memoryInfo, inputTensorValues.data(), inputTensorSize, m_input.shapes[0].data(), m_input.shapes[0].size());

    inputTensors.emplace_back(std::move(inputTensor));

//    float inputShape[] = {416, 416};

//    Ort::Value shape = Ort::Value::CreateTensor<float>(m_memoryInfo, inputShape, 2, m_input.shapes[1].data(), m_input.shapes[1].size());

//    inputTensors.emplace_back(std::move(shape));
    std::vector<Ort::Value> outputTensors = m_session.Run(
                Ort::RunOptions{nullptr}, m_input.names.data(), inputTensors.data(), m_input.count, m_output.names.data(), m_output.count);

    std::vector<cv::Mat> predicts;
    predicts.reserve(outputTensors.size());

    for(auto& tensor : outputTensors)
    {

        auto shape = tensor.GetTensorTypeAndShapeInfo().GetShape();
        //printVector(shape);
        int64_t count = tensor.GetTensorTypeAndShapeInfo().GetElementCount();
        //int64_t count = getSize(shape);
        //std::cout << "Output count" << count << std::endl;
        const float *arr = tensor.GetTensorData<float>();
        std::vector<float> vec;
        vec.assign(arr, arr+count);
        cv::Mat predict{toInt(shape), CV_32F, vec.data()};
        predicts.push_back(predict);
    }

    return postprocess(image, predicts);
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


