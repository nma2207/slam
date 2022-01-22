#pragma once

#include <QObject>

#include "video_stream.h"

namespace app
{
class ImageController : public QObject
{
    Q_OBJECT
public:
    explicit ImageController(QObject *parent = nullptr);
    QPixmap getImage();

signals:
    void imageChanged();

private:
    image_process::VideoStream m_stream;


};

} // namespace app

