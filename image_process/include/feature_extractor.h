#pragma once

#include <utility>

#include "cv_includes.h"

namespace image_process
{

class FeatureExtractor
{
public:
    virtual std::tuple<std::vector<cv::KeyPoint>, cv::Mat> detect(const cv::Mat&) = 0;
};

} // namespace image_process
