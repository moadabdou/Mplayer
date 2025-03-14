#include <filesystem>
#include <QDir>
#include "media/MediaScanner.h" 
#include "db/update_db.h"
#include "db/fetch_db.h"

namespace fs = std::filesystem;

int getRandomNumber(int min, int max) {
    return QRandomGenerator::global()->bounded(min, max);
}

MediaScanner::MediaScanner(QObject* parent) : QAbstractListModel(parent), m_isEmpty(false) {}



bool MediaScanner::isEmpty(){
    return m_isEmpty;
}

void MediaScanner::setIsEmpty(bool state){
    if (state == m_isEmpty) return;
    m_isEmpty = state ;
    emit isEmptyChanged();
}


int MediaScanner::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(songs.size());
}

QVariant MediaScanner::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(songs.size())) return QVariant();

    const Song& song = songs.at(index.row());
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

QHash<int, QByteArray> MediaScanner::roleNames() const {
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

void MediaScanner::scanFolders(const QString& query, bool update ) {
    beginResetModel();
    songs.clear();
    if (update){
        std::vector<QString> userDirs;
        QString userPath = qgetenv("USERPROFILE");
        QDir dir(userPath);
        
        QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < entries.size(); ++i) {
            if (entries[i].isDir()) {
                userDirs.push_back(entries[i].absoluteFilePath());
            }
        }

        if (userDirs.size()) updateDatabase(userDirs);
    }

    std::fstream logdb(std::filesystem::current_path().string() + "/logs/dbtransactions_fetch.log", std::ios::out);
    fectchSongs(songs, query);
    if (!songs.size()){
        setIsEmpty(true);
    }else {
        setIsEmpty(false);
    }
    endResetModel();
}

Song MediaScanner::getRandomSong(){
    if (!songs.size()) return {};
    int i = getRandomNumber(0, songs.size());
    return songs[i];
} 

MediaScanner::~MediaScanner() {}
