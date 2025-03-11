#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "media/MediaScanner.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    MediaScanner media_scanner;
    engine.rootContext()->setContextProperty("MediaScanner", &media_scanner);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("Mplayer", "Main");

    return app.exec();
}
