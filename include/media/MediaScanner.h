#ifndef MEDIA_SCANNER_H
#define MEDIA_SCANNER_H

#include <QAbstractListModel>
#include <QString>
#include <vector>
#include <QRandomGenerator>
#include <QtQml/qqmlregistration.h>
#include "shared/types/song.h"


class MediaScanner : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty WRITE setIsEmpty NOTIFY isEmptyChanged)
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

    bool isEmpty();
    void setIsEmpty(bool state);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Song getRandomSong();

    Q_INVOKABLE void scanFolders(const QString& query = "1", bool update = true);  // Scan music folder

signals:
    void isEmptyChanged();

private:
    std::vector<Song> songs;
    bool m_isEmpty;
};

#endif // MEDIA_SCANNER_H
