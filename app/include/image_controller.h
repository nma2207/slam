#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>

#include "video_stream.h"
#include "image_processor.h"

namespace app
{

class ProcessorType : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        None,
        OrbExtractor,
        Yolo
    };
    Q_ENUM(State)
};

class ImageController : public QObject
{
    Q_OBJECT
public:
    explicit ImageController(QObject *parent = nullptr);
    QPixmap getImage();
    //void setExtractor(const std::shared_ptr<image_process::FeatureExtractor> &newExtractor);
public slots:
    void onProcessorTypeChanged(int t);

signals:
    void imageChanged();

private:
    image_process::VideoStream m_stream;
    QTimer m_timer;

    std::unique_ptr<image_process::ImageProcessor> m_processor;
};

} // namespace app

