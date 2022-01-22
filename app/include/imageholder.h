#ifndef IMAGEHOLDER_H
#define IMAGEHOLDER_H
//#include <Q
#include <QObject>
#include <QQuickImageProvider>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class ImageHolder : public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ImageHolder();
    ~ImageHolder()
    {
        qDebug() << "RELEASE";
        m_cap.release();
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize& requestedSize) override;

private:
    cv::VideoCapture m_cap;

};

#endif // IMAGEHOLDER_H
