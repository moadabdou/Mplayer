#ifndef SONG_DB_H
#define SONG_DB_H
#include <fstream>
#include <QObject>
#include <sqlite3.h>

class SongDatabase : public QObject {
    Q_OBJECT
public:
    explicit SongDatabase(QObject *parent = nullptr);
    ~SongDatabase();

    Q_INVOKABLE bool editSong(const QString& path, const QString &edits);
    Q_INVOKABLE QStringList fetchCategoryItems(const QString &category);

private:
    sqlite3 *db;
    std::fstream logdb;
};

#endif // SONG_DB_H