#include "video_stream.h"

#include <iostream>

image_process::VideoStream::VideoStream(int deviceId, int apiId)
{  // 0 = autodetect default API
    m_cap.open(deviceId, apiId);
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
}

image_process::VideoStream::~VideoStream()
{
    m_cap.release();
}

cv::Mat image_process::VideoStream::read()
{
    if (!m_cap.isOpened()) {
        std::cerr << "Camera is not open!" << std::endl;
        return {};
    }

    cv::Mat result;

    m_cap.read(result);
    std::cout << result.size << std::endl;
    cv::Rect myRoi(80,0,480,480);
    result = result(myRoi);
    return result;
}
