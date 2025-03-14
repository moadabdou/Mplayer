#ifndef PLAY_CONTROLLER_H
#define PLAY_CONTROLLER_H
#include <QObject>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>
#include "media/MediaScanner.h"
#include "media/QueuePlaying.h"
#include "db/SongDatabase.h"

class PlayController: public QObject{
    Q_OBJECT

    //song properties 
    Q_PROPERTY(QString songTitle READ songTitle WRITE setSongTitle NOTIFY songTitleChanged);
    Q_PROPERTY(QString songCover READ songCover WRITE setSongCover NOTIFY songCoverChanged);
    Q_PROPERTY(QString songArtist READ songArtist WRITE setSongArtist NOTIFY songArtistChanged);
    Q_PROPERTY(QString songDuration READ songDuration WRITE setSongDuration NOTIFY songDurationChanged);

    //player properties  
    Q_PROPERTY(QString songPassedDuration READ  songPassedDuration WRITE setSongPassedDuration NOTIFY songPassedDurationChanged);
    Q_PROPERTY(qreal songPosition READ  songPosition WRITE setSongPosition NOTIFY songPositionChanged);
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged);
    Q_PROPERTY(int repeatMode READ repeatMode WRITE setRepeatMode NOTIFY repeatModeChanged);
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged);
    Q_PROPERTY(bool isShown READ isShown WRITE setIsShown NOTIFY isShownChanged);


    enum RepeatMode{
        NOREPEAT = 0,
        REPEAT,
        REPEATCURR,
        SHUFFLE
    };

public: 
    explicit PlayController(MediaScanner* ScannedSongs, QueuePlaying* QueuedSongs, SongDatabase* songDB, QObject *parent = nullptr);


    Q_INVOKABLE void next() const;
    Q_INVOKABLE void prev() const;
    Q_INVOKABLE void pause();
    Q_INVOKABLE void play();
    Q_INVOKABLE void seek(qreal position);


    //geters 
    QString songTitle() const;
    QString songCover() const;
    QString songArtist() const;
    QString songDuration() const;
    QString songPassedDuration() const;
    qreal songPosition() const;
    int repeatMode() const;
    int volume() const;
    bool isPlaying() const;
    bool isShown() const;

    //seters
    void setSongTitle(QString songTitle);
    void setSongCover(QString songCover);
    void setSongArtist(QString songArtist);
    void setSongDuration(QString songDuration);
    void setSongPassedDuration(QString songPassedDuration);
    void setSongPosition(qreal songPosition);
    void setRepeatMode(int repeatMode);
    void setVolume(int volume);
    void setIsPlaying(bool isPalying); 
    void setIsShown(bool isPalying); 
    

signals:
    void songTitleChanged();
    void songCoverChanged();
    void songArtistChanged();
    void songDurationChanged();
    void songPassedDurationChanged();
    void songPositionChanged();
    void repeatModeChanged();
    void volumeChanged();
    void isPlayingChanged();
    void isShownChanged();

public slots:
    void newSongToPlay();
    void updatePosition();
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void handleAudioOutputsChanged();

private:

    MediaScanner* scannedSongs;
    QueuePlaying* queuedSongs;
    SongDatabase* songDB;

    QString songPath;
    QAudioOutput *audioOutput;
    QMediaDevices *devices;
    QMediaPlayer *player;

    QTimer timer;

    QString m_songTitle;
    QString m_songCover;
    QString m_songArtist;
    QString m_songDuration;
    QString m_songPassedDuration;
    qreal m_position;
    int m_repeatMode;
    int m_volume;
    bool m_isPlaying;
    bool m_isShown;

};

#endif //PLAY_CONTROLLER_H 