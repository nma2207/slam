#include "imageholder.h"

#include <QDebug>
ImageHolder::ImageHolder()
    : QQuickImageProvider(Pixmap)
{
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    m_cap.open(deviceID, apiID);
}

QPixmap ImageHolder::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (!m_cap.isOpened())
    {
        qDebug() << "Camera is not open ";
        return {};
    }

    cv::Mat temp;
    m_cap.read(temp);
    //*size = QSize(temp.cols, temp.rows);
    QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    *size = QSize(dest.width(), dest.height());
    dest.save("img.png", "PNG");
    qDebug() << id;
    return QPixmap::fromImage(dest);
}
