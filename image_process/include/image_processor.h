#pragma once

#include "cv_includes.h"

class ImageProcessor
{
public:
    virtual cv::Mat process(const cv::Mat&) = 0;
};

