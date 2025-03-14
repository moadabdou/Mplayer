#ifndef BEST_SONGS_H
#define BEST_SONGS_H

#include <QAbstractListModel>
#include <vector>
#include "shared/types/song.h"


class BestSongs : public QAbstractListModel {
    Q_OBJECT
public:
    explicit BestSongs(QObject* parent = nullptr);
    ~BestSongs();

    enum Roles {
        FilePathRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        AlbumRole,
        DurationRole,
        CoverImageRole,
        IsFavRole
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void update(); 

private:
    std::vector<Song> songs;
};

#endif // BEST_SONGS_H
