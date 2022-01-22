#pragma once

#include "cv_includes.h"

namespace image_process
{
class ImageStream
{
public:
    virtual ~ImageStream(){}
    virtual cv::Mat read() = 0;
};

} // namespace image_process
