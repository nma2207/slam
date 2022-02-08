#pragma once

#include "cv_includes.h"

namespace image_process
{

class ImageProcessor
{
public:
    virtual cv::Mat process(const cv::Mat&) = 0;
};

} //image_process
