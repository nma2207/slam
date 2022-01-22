#include "../include/image_controller.h"

#include <QImage>
#include <QPixmap>
#include <QDebug>

app::ImageController::ImageController(QObject *parent)
    : QObject{parent}
    , m_stream{}
{

}

QPixmap app::ImageController::getImage()
{
    cv::Mat temp = m_stream.read();
    QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    return QPixmap::fromImage(dest);
}
