#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickItem>
#include <QtQml/qqml.h>

#include "image_controller.h"
#include "image_holder.h"
#include "orb_extractor.h"



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

    view.show();
    view.setTitle("SLAM");
    return appl.exec();
}

