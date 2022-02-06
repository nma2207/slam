#include "orb_extractor.h"

image_process::OrbExtractor::OrbExtractor()
{
    m_extractor = cv::ORB::create();
}

std::tuple<std::vector<cv::KeyPoint>, cv::Mat> image_process::OrbExtractor::detect(const cv::Mat& img)
{
    std::vector<cv::KeyPoint> kp;
    cv::Mat descriptors;

    m_extractor->detectAndCompute(img, cv::noArray(), kp, descriptors);

    return {kp, descriptors};
}

cv::Mat image_process::OrbExtractor::process(const cv::Mat &image)
{
    auto [keypoints, _] = this->detect(image);
    cv::Mat result{};
    cv::drawKeypoints(image, keypoints, result, cv::Scalar{0, 255, 0});
    return result;
}
