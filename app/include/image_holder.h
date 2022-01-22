#pragma once

#include <QObject>
#include <QQuickImageProvider>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include "image_controller.h"

namespace app
{

class ImageHolder : public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ImageHolder();
    ~ImageHolder();
    void setController(ImageController* controller);


    QPixmap requestPixmap(const QString &id, QSize *size, const QSize& requestedSize) override;

private:
    ImageController* m_controller;

};

} // namespace app
