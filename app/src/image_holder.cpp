#include "image_holder.h"

#include <QDebug>

app::ImageHolder::ImageHolder()
    : QQuickImageProvider(Pixmap)
    , m_controller{nullptr}
{
}

app::ImageHolder::~ImageHolder()
{

}

QPixmap app::ImageHolder::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap result = m_controller->getImage();
    *size = QSize(result.width(), result.height());

    return result;
}
