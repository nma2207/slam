#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>

#include "video_stream.h"
#include "feature_extractor.h"

namespace app
{

class ProcessorType : public QObject
{
    Q_OBJECT
public:
    enum State
    {
          OrbExtractor
        , Yolo
    };
    Q_ENUM(State)
};

class ImageController : public QObject
{
    Q_OBJECT
public:
    explicit ImageController(QObject *parent = nullptr);
    QPixmap getImage();
    void setExtractor(const std::shared_ptr<image_process::FeatureExtractor> &newExtractor);

signals:
    void imageChanged();
public slots:
    void onProcessorTypeChanged(int t)
    {
        ProcessorType::State type = static_cast<ProcessorType::State>(t);
        qDebug() << type;

    }
private:
    image_process::VideoStream m_stream;
    QTimer m_timer;

    std::shared_ptr<image_process::FeatureExtractor> m_extractor;
};

} // namespace app

