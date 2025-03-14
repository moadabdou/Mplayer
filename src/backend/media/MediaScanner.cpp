#include <filesystem>
#include "media/MediaScanner.h" 
#include "db/update_db.h"
#include "db/fetch_db.h"

namespace fs = std::filesystem;


MediaScanner::MediaScanner(QObject* parent) : QAbstractListModel(parent) {}

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
        updateDatabase("C:/Users/dell/Downloads"); //to do dynamic look up 
    }

    std::fstream logdb(std::filesystem::current_path().string() + "/logs/dbtransactions_fetch.log", std::ios::out);
    fectchSongs(songs, logdb, query);

    endResetModel();
}

MediaScanner::~MediaScanner() {}
