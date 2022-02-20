#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickItem>
#include <QtQml/qqml.h>
#include <QVariant>
#include "image_controller.h"
#include "image_holder.h"
#include "orb_extractor.h"

#include <QMetaObject>



int main(int argc, char *argv[])
{
    QGuiApplication appl(argc, argv);
    //qmlRegisterUncreatableType<app::ProcessorType>("info", 1, 0, "ProcessorType", "HUI");

    app::ImageController controller{};
    //controller.setExtractor(std::make_shared<image_process::OrbExtractor>());
    app::ImageHolder holder{};
    holder.setController(&controller);

    QQmlEngine engine;
    engine.addImageProvider("imageholder", &holder);
    QQuickView view{&engine, nullptr};

    const QUrl url{"qrc:/qml/main.qml"};
    view.setSource(url);
    QObject* rootObject = view.rootObject();


    QObject::connect(&controller, SIGNAL(imageChanged()), rootObject, SLOT(updateImage()));
    QObject::connect(rootObject, SIGNAL(processorTypeChanged(int)), &controller, SLOT(onProcessorTypeChanged(int)));

    auto processorTypeMetaEnum = QMetaEnum::fromType<app::ImageController::ProcessorType>();
    for(int i=0; i< processorTypeMetaEnum.keyCount(); i++)
    {
        QString name = QString::fromUtf8(processorTypeMetaEnum.valueToKey(i));
        QMetaObject::invokeMethod(rootObject, "addProcessorType", QGenericReturnArgument{},
                                  Q_ARG(QString,  name));
    }

    view.show();
    view.setTitle("SLAM");
    return appl.exec();
}

