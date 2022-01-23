#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickItem>

#include "image_controller.h"
#include "image_holder.h"
#include "orb_extractor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app::ImageController controller{};
    controller.setExtractor(std::make_shared<image_process::OrbExtractor>());
    app::ImageHolder holder{};
    holder.setController(&controller);

    QQmlEngine engine;
    engine.addImageProvider("imageholder", &holder);
    QQuickView view{&engine, nullptr};
    //const QUrl url(u"qrc:/app/slam/qml/main.qml"_qs);
    const QUrl url{"qrc:/qml/main.qml"};
    view.setSource(url);
    QObject* rootObject = view.rootObject();


    QObject::connect(&controller, SIGNAL(imageChanged()), rootObject, SLOT(updateImage()));

    view.show();
    view.setTitle("SLAM");
    return app.exec();
}
