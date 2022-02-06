#pragma once

#include "feature_extractor.h"
#include "image_processor.h"

#include <opencv2/features2d.hpp>

namespace image_process
{
class OrbExtractor : public FeatureExtractor, public ImageProcessor
{
public:
    OrbExtractor();

    std::tuple<std::vector<cv::KeyPoint>, cv::Mat> detect(const cv::Mat &) override;
    cv::Mat process(const cv::Mat &) override;

private:

    cv::Ptr<cv::ORB> m_extractor;   
};

} // namespace image_process
