#include "video_stream.h"

#include <iostream>

image_process::VideoStream::VideoStream(int deviceId, int apiId) {  // 0 = autodetect default API
    m_cap.open(deviceId, apiId);
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
    return result;
}
