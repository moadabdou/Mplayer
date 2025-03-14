#include <filesystem>
#include "db/SongDatabase.h"


SongDatabase::SongDatabase(QObject *parent) : QObject(parent), db(nullptr) {
    std::string  dbPath = std::filesystem::current_path().string() + "/data/songs.db";
    //logdb = std::fstream(std::filesystem::current_path().string() + "/logs/dbsongs.log", std::ios::out);
    if (sqlite3_open(dbPath.c_str() , &db) != SQLITE_OK) {
        //qWarning() << "Failed to open database:" << sqlite3_errmsg(db);
    }
}

SongDatabase::~SongDatabase() {
    if (db) {
        sqlite3_close(db);
    }
}

bool SongDatabase::editSong(const QString& path, const QString &edits) {
    QString query = "UPDATE songs SET "+ edits +" WHERE  path = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query.toUtf8().constData(), -1, &stmt, nullptr) != SQLITE_OK) {
        //logdb << "prepare problem  : " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    sqlite3_bind_text(stmt, 1, path.toStdString().c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        //logdb << "step problem  : " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    //logdb << "complete !! \n";
    sqlite3_finalize(stmt);
    return true;
}

QStringList SongDatabase::fetchCategoryItems(const QString &category) {
    QStringList items;
    sqlite3_stmt *stmt;
    QString query;

    if (category == "Albums") {
        query = "SELECT DISTINCT album FROM songs ORDER BY album;";
    } else if (category == "Artists") {
        query = "SELECT DISTINCT artist FROM songs ORDER BY artist;";
    } else if (category == "Folders") {
        query = "SELECT DISTINCT rtrim(path, replace(path, '/', '')) FROM songs;";
    } else {
        return {" "};
    }

    if (sqlite3_prepare_v2(db, query.toUtf8().constData(), -1, &stmt, nullptr) != SQLITE_OK) {
        return {" "};
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (text) {
            items.append(QString::fromUtf8(text));
        }
    }

    sqlite3_finalize(stmt);
    return items;
}
