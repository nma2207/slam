#include "orb_extractor.h"

image_process::OrbExtractor::OrbExtractor()
{
    m_extractor = cv::ORB::create();
}

std::pair<std::vector<cv::KeyPoint>, cv::Mat> image_process::OrbExtractor::detect(const cv::Mat& img)
{
    std::vector<cv::KeyPoint> kp;
    cv::Mat descriptors;

    m_extractor->detectAndCompute(img, cv::noArray(), kp, descriptors);

    return {kp, descriptors};
}
