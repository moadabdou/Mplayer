#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "media/MediaScanner.h"
#include "db/Song_db.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    MediaScanner media_scanner;
    SongDatabase songdb;
    engine.rootContext()->setContextProperty("MediaScanner", &media_scanner);
    engine.rootContext()->setContextProperty("SongDB", &songdb);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("Mplayer", "Main");

    return app.exec();
}
