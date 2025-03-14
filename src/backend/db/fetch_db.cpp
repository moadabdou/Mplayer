#include <sqlite3.h>
#include <filesystem>
#include "db/fetch_db.h"

void fectchSongs(std::vector<Song>& songs, const QString& conditions ){ 
    const std::string dbPath = std::filesystem::current_path().string() + "/data/songs.db";
    sqlite3 *db;
    
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return;
    }

    QString sql = QString("SELECT path, title, artist, album, duration, songCover, is_fav, play_count FROM songs WHERE %1;").arg(conditions);
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.toStdString().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Song song;
            song.filePath = QString((const char*)sqlite3_column_text(stmt, 0));
            song.title = QString((const char*)sqlite3_column_text(stmt, 1));
            song.artist = QString((const char*)sqlite3_column_text(stmt, 2));
            song.album = QString((const char*)sqlite3_column_text(stmt, 3));
            song.duration = QString((const char*)sqlite3_column_text(stmt, 4));
            song.coverImage = QString((const char*)sqlite3_column_text(stmt, 5));
            song.isFav = sqlite3_column_int(stmt, 6) != 0;
            
            songs.push_back(song);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}