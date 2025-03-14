#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "media/MediaScanner.h"
#include "media/QueuePlaying.h"
#include "media/PlayController.h"
#include "media/BestSongs.h"
#include "db/SongDatabase.h"
  
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    MediaScanner media_scanner;
    QueuePlaying queuePalying;
    SongDatabase songDatabase;
    BestSongs bestSongs;

    PlayController controller(&media_scanner, &queuePalying, &songDatabase);
    
    QObject::connect(&queuePalying, &QueuePlaying::currentlyPlayingChanged,
                     &controller, &PlayController::newSongToPlay);

    engine.rootContext()->setContextProperty("MediaScanner", &media_scanner);
    engine.rootContext()->setContextProperty("QueuePlaying", &queuePalying);
    engine.rootContext()->setContextProperty("SongDatabase", &songDatabase);
    engine.rootContext()->setContextProperty("PlayController", &controller);
    engine.rootContext()->setContextProperty("BestSongs", &bestSongs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []()->void { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
    engine.loadFromModule("Mplayer", "Main");

    return app.exec();
}
