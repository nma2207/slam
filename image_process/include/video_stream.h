#pragma once

#include "cv_includes.h"
#include "image_stream.h"

namespace image_process
{

class VideoStream : public ImageStream
{
public:
    VideoStream(int deviceId = -1, int apiId = cv::CAP_ANY);
    ~VideoStream();

    cv::Mat read() override;

private:
    cv::VideoCapture m_cap;
};

} // namespace image_process
