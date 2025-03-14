#include "media/QueuePlaying.h"

QueuePlaying::QueuePlaying(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentlyPlaying(-1)
{
}

int QueuePlaying::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_songs.count();
}

QVariant QueuePlaying::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(m_songs.size())) return QVariant();

    const Song& song = m_songs.at(index.row());
    switch (role) {
        case FilePathRole: return song.filePath;
        case TitleRole: return song.title;
        case ArtistRole: return song.artist;
        case AlbumRole: return song.album;
        case DurationRole: return song.duration;
        case CoverImageRole: return song.coverImage;
        case IsFavRole : return song.isFav;
    }
    return QVariant();
}


QHash<int, QByteArray> QueuePlaying::roleNames() const {
    return {
        { FilePathRole, "filePath" },
        { TitleRole, "title" },
        { ArtistRole, "artist" },
        { AlbumRole, "album" },
        { DurationRole, "duration"},
        { CoverImageRole, "coverImage" },
        { IsFavRole , "isFav" }
    };
}

void QueuePlaying::addSong(const QString& path, const QString& title,const QString& artist, const QString& album, const QString& duration, const QString& coverImage, bool isFav)
{
    if (m_currentlyPlaying != -1 && m_songs[m_currentlyPlaying].filePath == path) return;

    auto it = std::find_if(m_songs.begin(), m_songs.end(),
    [&path](const Song& song) { return song.filePath == path; });
    int existingIndex = (it != m_songs.end()) ? std::distance(m_songs.begin(), it) : -1;
    if (existingIndex != -1) {
        // If song exists and it's not already at the target position
        int insertPosition = m_songs.length() - 1 ;
        if (existingIndex != insertPosition) {
            beginMoveRows(QModelIndex(), existingIndex, existingIndex, 
                         QModelIndex(), insertPosition+1);
            m_songs.move(existingIndex, insertPosition);
            m_currentlyPlaying--;
            endMoveRows();
        }
    } else {
        beginInsertRows(QModelIndex(), m_songs.count(), m_songs.count());
        m_songs.append({ path,  title, artist,  album,  duration,  coverImage,  isFav});
        if(m_currentlyPlaying == -1){ // set currently  playing to  the first  song
            setCurrentlyPlaying(0);
        }
        endInsertRows();
    }
}

void QueuePlaying::addSongAfterCurrent(const QString& path, const QString& title,const QString& artist, const QString& album, const QString& duration, const QString& coverImage, bool isFav)
{
    if (m_currentlyPlaying != -1 && m_songs[m_currentlyPlaying].filePath == path) return;
    int insertPosition = m_currentlyPlaying + 1;
    if (insertPosition < 0) insertPosition = 0;

    // Find if song already exists
    auto it = std::find_if(m_songs.begin(), m_songs.end(),
        [&path](const Song& song) { return song.filePath == path; });
    int existingIndex = (it != m_songs.end()) ? std::distance(m_songs.begin(), it) : -1;

    if (existingIndex != -1) {
        // If song exists and it's not already at the target position
        if (existingIndex != insertPosition) {
            beginMoveRows(QModelIndex(), existingIndex, existingIndex, 
                         QModelIndex(), insertPosition);
            m_songs.move(existingIndex, insertPosition);
            if(insertPosition > m_currentlyPlaying){
                m_currentlyPlaying--;
            }
            endMoveRows();
        }
    } else {
        // If song doesn't exist, insert it
        beginInsertRows(QModelIndex(), insertPosition, insertPosition);
        m_songs.insert(insertPosition, { path, title, artist, album, duration, coverImage, isFav});
        endInsertRows();
    }
}

int QueuePlaying::isQueued(const QString& path){
    auto it = std::find_if(m_songs.begin(), m_songs.end(),
        [&path](const Song& song) { return song.filePath == path; });
    return  (it != m_songs.end()) ? std::distance(m_songs.begin(), it) : -1;
}

void QueuePlaying::playSong(const QString& path, const QString& title,const QString& artist, const QString& album, const QString& duration, const QString& coverImage, bool isFav){
    int i =  isQueued(path);
    if (i != -1){
        setCurrentlyPlaying(i);
    }else {
        addSongAfterCurrent(path, title,artist, album, duration,coverImage, isFav);
        next();
    }
}

SongData QueuePlaying::getCurrentSong() const{
    if (m_currentlyPlaying == -1) return {};
    Song current =  m_songs[m_currentlyPlaying];
    return {current.filePath ,current.title,current.artist,current.album,current.duration,current.coverImage,current.isFav};
}
bool QueuePlaying::next()
{
    if (m_currentlyPlaying + 1 >= m_songs.count())
        return false;
        
    setCurrentlyPlaying(m_currentlyPlaying + 1);
    return true;
}
bool QueuePlaying::prev()
{
    if (m_currentlyPlaying - 1 < 0)
        return false;
        
    setCurrentlyPlaying(m_currentlyPlaying - 1);
    return true;
}

void QueuePlaying::reset()
{
    if ( !m_songs.size() ){    
        setCurrentlyPlaying(-1);
    }else {
        setCurrentlyPlaying(0);
    }
}

int QueuePlaying::currentlyPlaying() const
{
    return m_currentlyPlaying;
}

void QueuePlaying::setCurrentlyPlaying(int index)
{
    m_currentlyPlaying = index;
    emit currentlyPlayingChanged();
}