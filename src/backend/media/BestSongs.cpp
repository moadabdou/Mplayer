#include "media/BestSongs.h"
#include "db/fetch_db.h"


BestSongs::BestSongs(QObject* parent) : QAbstractListModel(parent) {}

int BestSongs::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(songs.size());
}

QVariant BestSongs::data(const QModelIndex& index, int role) const {
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

QHash<int, QByteArray> BestSongs::roleNames() const {
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

void BestSongs::update() {
    beginResetModel();
    songs.clear();

    fectchSongs(songs , " 1 ORDER BY play_count DESC LIMIT 20");

    endResetModel();
}

BestSongs::~BestSongs() {}