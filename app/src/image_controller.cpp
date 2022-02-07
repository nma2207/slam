#include "image_controller.h"

#include <QImage>
#include <QPixmap>
#include <QDebug>

#include <opencv2/features2d.hpp>

app::ImageController::ImageController(QObject *parent)
    : QObject{parent}
    , m_stream{0}
{
    m_timer.setInterval(100);
    m_timer.setSingleShot(false);
    QObject::connect(&m_timer, &QTimer::timeout, this, &ImageController::imageChanged);
    m_timer.start();

}

QPixmap app::ImageController::getImage()
{
    cv::Mat temp = m_stream.read();

    if (m_extractor)
    {
        auto [keypoints, _] = m_extractor->detect(temp);
        cv::drawKeypoints(temp, keypoints, temp, cv::Scalar{0, 255, 0});
    }
    QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_BGR888);
    return QPixmap::fromImage(dest);
}

void app::ImageController::setExtractor(const std::shared_ptr<image_process::FeatureExtractor> &newExtractor)
{
    m_extractor = newExtractor;
}


