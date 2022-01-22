#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QImage>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "imageholder.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/slam/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.addImageProvider("imageholder", new ImageHolder);
    engine.load(url);

//    cv::VideoCapture cap;
//    int deviceID = -1;             // 0 = open default camera
//    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
//    cap.open(deviceID, apiID);

//    if (!cap.isOpened()) {
//        qDebug() << "Not opened";
//        return -1;
//    }

//    cv::Mat frame;
//    for (;;)
//    {
//        // wait for a new frame from camera and store it into 'frame'
//        cap.read(frame);
//        // check if we succeeded
//        if (frame.empty()) {
//            //cerr << "ERROR! blank frame grabbed\n";
//            break;
//        }
//        // show live and wait for a key with timeout long enough to show images
//        cv::imshow("Live", frame);
//        if (cv::waitKey(5) >= 0)
//            break;
//    }

    return app.exec();
}
