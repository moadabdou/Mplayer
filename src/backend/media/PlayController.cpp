#include <QAudioDevice>
#include <QDebug>
#include "media/PlayController.h"

QString formatTime(size_t total_seconds){
    size_t hours = total_seconds / 3600;
    size_t minutes = (total_seconds % 3600) / 60;
    size_t seconds = total_seconds % 60;
    return  (hours > 0 ? QString("%1:").arg(hours) : "") +
            QString("%1:%2")
                .arg(minutes, hours > 0 ? 2 : 1, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
}

PlayController::PlayController(MediaScanner *ScannedSongs, QueuePlaying *QueuedSongs, SongDatabase* songDB, QObject *parent)
    :scannedSongs(ScannedSongs)
    ,queuedSongs(QueuedSongs)
    ,songDB(songDB)
    ,songPath("")
    ,player(new QMediaPlayer)
    ,audioOutput(new QAudioOutput)
    ,devices(new QMediaDevices)
    ,m_repeatMode(0)
    ,m_volume(50)
    ,QObject(parent){
    setVolume(m_volume);
    player->setAudioOutput(audioOutput);
    connect(&timer, &QTimer::timeout, this, &PlayController::updatePosition);
    connect(devices, &QMediaDevices::audioOutputsChanged, this, &PlayController::handleAudioOutputsChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &PlayController::onPlaybackStateChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &PlayController::onMediaStatusChanged);
    
    timer.start(1000);
}

void PlayController::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    if (state == QMediaPlayer::PausedState) {
        setIsPlaying(false);
    }else {
        setIsPlaying(true);
    }
}


void PlayController::handleAudioOutputsChanged(){
    audioOutput->setDevice( QMediaDevices::defaultAudioOutput());
}

void PlayController::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {

        songDB->editSong(songPath, " play_count = play_count + 1 ");

        if (repeatMode() == RepeatMode::NOREPEAT){
            if (!queuedSongs->next()){
                pause();
            }
        }else if (repeatMode() == RepeatMode::REPEAT){
            if (!queuedSongs->next()){
                queuedSongs->reset();
            }
        }else if (repeatMode() == RepeatMode::REPEATCURR){
            seek(0);
            play();
        }else if (repeatMode() == RepeatMode::SHUFFLE){
            Song song = scannedSongs->getRandomSong();
            queuedSongs->playSong(
                song.filePath,
                song.title,
                song.artist,
                song.album,
                song.duration,
                song.coverImage,
                song.isFav
            );
        }
    }
}

void PlayController::updatePosition(){
    setSongPassedDuration(formatTime(player->position() / 1000));
    setSongPosition((qreal)player->position() / player->duration());
}

void PlayController::next() const{
    if(!queuedSongs->next() && repeatMode() == RepeatMode::REPEAT){
        queuedSongs->reset();
    }
}
void PlayController::prev() const{
    queuedSongs->prev();
}
void PlayController::pause() {
    player->pause();
}
void PlayController::play() {
    if (songPath == "") return;
    player->play();
}

void PlayController::seek(qreal position) {
    qint64 p = position * player->duration();

    player->setPosition(p);
    setSongPosition(position);
}

QString PlayController::songTitle() const{
    return m_songTitle;
}
QString PlayController::songArtist() const{
    return m_songArtist;
}
QString PlayController::songCover() const{
    return m_songCover;
}
QString PlayController::songDuration() const{
    return m_songDuration;
}
QString PlayController::songPassedDuration() const{
    return m_songPassedDuration;
}
qreal PlayController::songPosition() const{
    return m_position;
}
int PlayController::volume() const{
    return m_volume;
}
int PlayController::repeatMode() const{
    return m_repeatMode;
}
bool PlayController::isPlaying() const{
    return m_isPlaying;
}
bool PlayController::isShown() const{
    return m_isShown;
}

void PlayController::setSongTitle(QString songTitle){
    m_songTitle = songTitle;
    emit songTitleChanged();
}

void PlayController::setSongArtist(QString songArtist){
    m_songArtist = songArtist;
    emit songArtistChanged();
}

void PlayController::setSongCover(QString songCover){
    m_songCover = songCover;
    emit songCoverChanged();
}

void PlayController::setSongDuration(QString songDuration){
    m_songDuration = songDuration;
    emit songDurationChanged();
}

void PlayController::setSongPassedDuration(QString songPassedDuration){
    m_songPassedDuration = songPassedDuration;
    emit songPassedDurationChanged();
}

void PlayController::setSongPosition(qreal position){
    m_position = position;
    emit songPositionChanged();
}

void PlayController::setVolume(int volume){
    m_volume = volume;
    audioOutput->setVolume(volume/100.0);
    emit volumeChanged();
}
void PlayController::setRepeatMode(int repeatMode){
    m_repeatMode = repeatMode;
    emit repeatModeChanged();
}
void PlayController::setIsPlaying(bool isPlaying){
    m_isPlaying = isPlaying;
    emit isPlayingChanged();
}
void PlayController::setIsShown(bool isShown){
    m_isShown= isShown;
    emit isShownChanged();
}

void PlayController::newSongToPlay(){


    if (songPosition() >= 0.9 || player->position() >= 60000){
        songDB->editSong(songPath, " play_count = play_count + 1 ");
    }


    SongData song = queuedSongs->getCurrentSong();
    songPath = song.filePath;

    songDB->editSong(songPath, " recent_play = strftime('%s', 'now') "); //update recent play for the new song
    player->setSource(QUrl::fromLocalFile(songPath));

    setSongTitle(song.title);
    setSongArtist(song.artist);
    setSongCover(song.coverImage);
    setSongDuration(song.duration);

    setSongPassedDuration("0:00");
    setSongPosition(0); //not realy neccerry since position get updated directly from the Audio, but this  make it a bit faster to reset to need to  wait the audio  to load
    setIsShown(true);

    play();
}