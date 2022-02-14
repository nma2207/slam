#include "image_controller.h"

#include <QImage>
#include <QPixmap>
#include <QDebug>

#include <opencv2/features2d.hpp>

#include "orb_extractor.h"
#include "onnx_nextwork.h"

namespace
{
std::unique_ptr<image_process::ImageProcessor> getProcessor(app::ProcessorType::State type)
{
    switch (type)
    {
    case app::ProcessorType::None:
        return nullptr;
        break;

    case app::ProcessorType::OrbExtractor:
        return std::make_unique<image_process::OrbExtractor>();
        break;

    case app::ProcessorType::Yolo:
        return std::make_unique<image_process::OnnxNextwork>("yolov5n.onnx");
        break;
    }

    return nullptr;
}

} // namespace

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

    if (m_processor)
    {
        temp = m_processor->process(temp);
    }

    QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_BGR888);
    return QPixmap::fromImage(dest);
}

void app::ImageController::onProcessorTypeChanged(int t)
{
    ProcessorType::State type = static_cast<ProcessorType::State>(t);
    qDebug() << type;

    auto newProcessor = getProcessor(type);
    if (newProcessor != nullptr)
        m_processor = std::move(newProcessor);
    else
        m_processor.release();
}

//void app::ImageController::setExtractor(const std::shared_ptr<image_process::FeatureExtractor> &newExtractor)
//{
//    m_extractor = newExtractor;
//}


