#include <QMediaPlayer>
#include <QAudioOutput>
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

PlayController::PlayController(MediaScanner *ScannedSongs, QueuePlaying *QueuedSongs, QObject *parent)
    :scannedSongs(ScannedSongs)
    ,queuedSongs(QueuedSongs)
    ,songPath("")
    ,player(new QMediaPlayer)
    ,audioOutput(new QAudioOutput)
    ,QObject(parent){
    
    player->setAudioOutput(audioOutput);
    connect(&timer, &QTimer::timeout, this, &PlayController::updatePosition);
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

void PlayController::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        if (!queuedSongs->next()){
            pause();
        }
    }
}

void PlayController::updatePosition(){
    setSongPassedDuration(formatTime(player->position() / 1000));
    setSongPosition((qreal)player->position() / player->duration());
}

void PlayController::next() const{
    queuedSongs->next();
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
    SongData song = queuedSongs->getCurrentSong();
    songPath = song.filePath;
    player->setSource(QUrl::fromLocalFile(songPath));
    audioOutput->setVolume(0.8);
    setSongTitle(song.title);
    setSongArtist(song.artist);
    setSongCover(song.coverImage);
    setSongDuration(song.duration);
    setSongPassedDuration("0:00");
    setIsShown(true);
    play();
}