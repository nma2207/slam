#pragma once

#include <utility>

#include "cv_includes.h"

namespace image_process
{

class FeatureExtractor
{
    virtual std::pair<cv::KeyPoint, cv::Mat> detect(const cv::Mat&) = 0;

};

} // namespace image_process
