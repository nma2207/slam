#include "../include/image_controller.h"

#include <QImage>
#include <QPixmap>
#include <QDebug>

app::ImageController::ImageController(QObject *parent)
    : QObject{parent}
    , m_stream{}
{
    m_timer.setInterval(100);
    m_timer.setSingleShot(false);
    QObject::connect(&m_timer, &QTimer::timeout, this, &ImageController::imageChanged);
    m_timer.start();

}

QPixmap app::ImageController::getImage()
{
    cv::Mat temp = m_stream.read();
    QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    return QPixmap::fromImage(dest);
}
