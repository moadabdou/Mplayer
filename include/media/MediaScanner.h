#ifndef MEDIA_SCANNER_H
#define MEDIA_SCANNER_H

#include <QAbstractListModel>
#include <QString>
#include <vector>
#include <QtQml/qqmlregistration.h>
#include "shared/types/song.h"


class MediaScanner : public QAbstractListModel {
    Q_OBJECT
public:
    explicit MediaScanner(QObject* parent = nullptr);
    ~MediaScanner();

    enum Roles {
        FilePathRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        AlbumRole,
        DurationRole,
        CoverImageRole,
        IsFavRole
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const ;
    QVariant data(const QModelIndex& index, int role) const ;
    QHash<int, QByteArray> roleNames() const ;

    Q_INVOKABLE void scanFolders(const QString& query = "1", bool update = true);  // Scan music folder

private:
    std::vector<Song> songs;
};

#endif // MEDIA_SCANNER_H
