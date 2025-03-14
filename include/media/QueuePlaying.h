#ifndef QUEUEPLAYING_H
#define QUEUEPLAYING_H
#include <QAbstractListModel>
#include <QString>
#include <QObject>
#include <vector>
#include <QtQml/qqmlregistration.h>
#include "shared/types/song.h"


class SongData {
    Q_GADGET  // Enables QML property access
public:
    Q_PROPERTY(QString filePath MEMBER filePath);
    Q_PROPERTY(QString title MEMBER title);
    Q_PROPERTY(QString artist MEMBER artist);
    Q_PROPERTY(QString album MEMBER album);
    Q_PROPERTY(QString duration MEMBER duration);
    Q_PROPERTY(QString coverImage MEMBER coverImage);  // Path to the cover image
    Q_PROPERTY(bool isFav MEMBER isFav);  
  //current.filePath ,current.title,current.artist,current.album,current.duration,current.coverImage,current.isFav
    QString filePath;
    QString title;
    QString artist;
    QString album;
    QString duration;
    QString coverImage;  // Path to the cover image
    bool isFav;  

};


class QueuePlaying : public QAbstractListModel 
{
    Q_OBJECT
    Q_PROPERTY(int currentlyPlaying READ currentlyPlaying WRITE setCurrentlyPlaying NOTIFY currentlyPlayingChanged)

public:
    enum Roles {
        FilePathRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        AlbumRole,
        DurationRole,
        CoverImageRole,  // Added for coverImage
        IsFavRole       // Added for isFav
    };

    explicit QueuePlaying(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Queue management
    Q_INVOKABLE void addSong(const QString& path, const QString& title,const QString& artist, const QString& album, const QString& duration, const QString& coverImage, bool isFav);
    Q_INVOKABLE void addSongAfterCurrent(const QString& path, const QString& title,const QString& artist, const QString& album, const QString& duration, const QString& coverImage, bool isFav);
    Q_INVOKABLE int isQueued(const QString &path);
    Q_INVOKABLE void playSong(const QString &path, const QString &title, const QString &artist, const QString &album, const QString &duration, const QString &coverImage, bool isFav);
    Q_INVOKABLE SongData getCurrentSong() const;
    Q_INVOKABLE bool next();
    Q_INVOKABLE bool prev();
    Q_INVOKABLE void reset();

    // Currently playing management
    int currentlyPlaying() const;
    void setCurrentlyPlaying(int index);

signals:
    void currentlyPlayingChanged(); 

private:
    QList<Song> m_songs;
    int m_currentlyPlaying;
};

#endif // QUEUEPLAYING_H
