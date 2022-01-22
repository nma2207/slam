#pragma once

#include "cv_includes.h"

namespace image_process
{
class ImageSource
{
public:
    virtual cv::Mat read() = 0;
};

} // namespace image_process
