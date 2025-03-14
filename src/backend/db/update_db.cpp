#include <sqlite3.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <QDir> 
#include <QDebug>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QEventLoop>
#include <taglib/fileref.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include "shared/types/song.h"

int getAudioDuration(const std::string &filePath) {
    std::string command = "ffprobe -i \"" + filePath + "\" -show_entries format=duration -v quiet -of csv=\"p=0\"";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return -1;

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    try {
        return static_cast<int>(std::stod(result)); // Convert to integer seconds
    } catch (...) {
        return -1; // Failed to parse
    }
}

bool isSongExist(sqlite3* db, const char *path, std::fstream& dblogs){
    const char *sql = "SELECT COUNT(*) FROM songs WHERE path = ?;";
    sqlite3_stmt *stmt;
    bool exists = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = (sqlite3_column_int(stmt, 0) > 0);
        }

    } else {
        dblogs << "Error checking record: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return exists;
}
void updateLastScanTime(sqlite3* db, const std::vector<QString>& songPaths, size_t time, std::fstream& dblogs) {
    std::string sql = "UPDATE songs SET last_time_scanned = ?, is_old = 1 WHERE path IN (";
    for (size_t i = 0; i < songPaths.size(); i++) {
        sql += "?";
        if (i < songPaths.size() - 1) {
            sql += ",";
        }
    }
    sql += ");";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the timestamp first
        sqlite3_bind_int64(stmt, 1, time);
        
        // Bind all paths
        for (size_t i = 0; i < songPaths.size(); i++) {
            sqlite3_bind_text(stmt, i + 2, songPaths[i].toStdString().c_str(), -1, SQLITE_TRANSIENT);
        }
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            dblogs << "Error updating last scan time: " << sqlite3_errmsg(db) << std::endl;
        }
    } else {
        dblogs << "Error preparing in last scan time: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}


bool registerNewSongs(sqlite3* db,std::vector<Song> songs ,size_t scantime, std::fstream& dblogs) {
    std::string sql = "INSERT INTO songs (path, title, artist, album,duration,songCover, last_time_scanned) VALUES ";
    
    // Build the query with multiple value sets
    for (size_t i = 0; i < songs.size(); i++) {
        sql += "(?, ?, ?, ?, ?, ?, ?)";
        if (i < songs.size() - 1) {
            sql += ",";
        }
    }
    sql += ";";
    
    sqlite3_stmt *stmt;
    bool success = false;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int bindIndex = 1;
        for (const Song& song : songs) {
            sqlite3_bind_text(stmt, bindIndex++, song.filePath.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, bindIndex++, song.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, bindIndex++, song.artist.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, bindIndex++, song.album.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, bindIndex++, song.duration.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, bindIndex++, song.coverImage.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int64(stmt, bindIndex++, scantime);
        }

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            success = true;
        } else {
            dblogs << "Error inserting records: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_finalize(stmt);
    } else {
        dblogs << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return success;
}

bool isSongMetaDataChanged(sqlite3* db, const QString& path, const QString& title, 
                          const QString& artist, const QString& album, std::fstream& dblogs) {
    const char *sql = "SELECT title, artist, album FROM songs WHERE path = ?;";
    sqlite3_stmt *stmt;
    bool changed = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, path.toStdString().c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            QString dbTitle = sqlite3_column_text(stmt, 0) ? QString((const char*)sqlite3_column_text(stmt, 0)) : "";
            QString dbArtist = sqlite3_column_text(stmt, 1) ? QString((const char*)sqlite3_column_text(stmt, 1)) : "";
            QString dbAlbum = sqlite3_column_text(stmt, 2) ? QString((const char*)sqlite3_column_text(stmt, 2)) : "";
            
            changed = (dbTitle != title || dbArtist != artist || dbAlbum != album);
        }
    } else {
        dblogs << "Error checking metadata: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return changed;
}

void deleteRemovedSongs(sqlite3* db, size_t scantime, std::fstream& dblogs) {
    const char *sql = "DELETE FROM songs WHERE last_time_scanned < ? RETURNING path, songCover;"; 
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, scantime);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            
            const char* path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            const char* cover = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            
            dblogs << "Removed song : " << path << std::endl;
            if (cover && strncmp(cover, "file:///", 8) == 0) {
                std::string coverPath = cover + 8; // Skip "file:///"
                if (std::filesystem::exists(coverPath)) {
                    std::filesystem::remove(coverPath);     
                    dblogs << "Removed cover: " << cover << std::endl;
                }
            }
        }
    } else {
        dblogs << "Error preparing delete statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}


void updateDatabase(const std::vector<QString>&  folderPaths) {

    
    std::fstream logSongs(std::filesystem::current_path().string() + "/logs/songsScan.log", std::ios::out);
    std::fstream logdb(std::filesystem::current_path().string() + "/logs/dbtransactions.log", std::ios::out);
    

    const std::string dbPath = std::filesystem::current_path().string() + "/data/songs.db";
    sqlite3 *db;

    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        //qDebug() << "Error opening database: " << sqlite3_errmsg(db) << Qt::endl;
        //qDebug() << std::filesystem::current_path().string();
        return;
    }

    const char *createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS songs (
            path TEXT PRIMARY KEY,     -- Absolute path as the unique ID
            title TEXT NOT NULL,                -- Song title
            album TEXT NOT NULL,                -- Album name
            artist TEXT NOT NULL,               -- Artist name
            songCover TEXT NOT NULL,            -- cover image
            duration TEXT NOT NULL,
            is_fav INTEGER DEFAULT 0,  -- 0 = not favorite, 1 = favorite
            play_count INTEGER DEFAULT 0, -- Number of times played
            is_old INTEGER DEFAULT 0,  
            last_time_scanned INTEGER, -- Unix timestamp of the last scan
            recent_play  INTEGER DEFAULT 0 -- Unix timestamp of the last play
        );
    )";
    
    if (sqlite3_exec(db, createTableSQL, 0, 0, nullptr) != SQLITE_OK) {
        //qDebug() << "Error creating table: " << sqlite3_errmsg(db) << Qt::endl;
        sqlite3_close(db);
        return;
    }

    QStringList filters = { "*.mp3"};
    std::vector<Song> newSongs;
    std::vector<QString>  oldSongs;
    size_t scantime = QDateTime::currentSecsSinceEpoch();

    std::vector<std::pair<QString, QString>> files;
    for (const QString& folderPath : folderPaths) {
        QDir directory(folderPath);
        QStringList fileList = directory.entryList(filters, QDir::Files);
        for (const QString& file : fileList) {
            files.push_back({folderPath, file});
        }
    }


    for (const auto& filePair : files) {
        QString filePath = filePair.first + "/" + filePair.second;
        QString file = filePair.second;

        bool isSongExists = isSongExist(db, filePath.toStdString().c_str() , logdb);

        // Open file with TagLib using wide string for Unicode support
        #ifdef _WIN32
            TagLib::FileRef f(filePath.toStdWString().c_str());
        #else
            TagLib::FileRef f(filePath.toStdString().c_str());
        #endif
        QString title = file;
        QString artist = "Unknown Artist";
        QString album = "Unknown Album";
        QString duration = "0:00";
        QString coverImage = "../../../res/images/defaultCover.jpg";

        if (!f.isNull() && f.tag()) {
            TagLib::Tag* tag = f.tag();
            title = !tag->title().isEmpty()? QString::fromStdString(tag->title().to8Bit(true)):title;
            artist = !tag->artist().isEmpty()? QString::fromStdString(tag->artist().to8Bit(true)):artist;
            album =  !tag->album().isEmpty()? QString::fromStdString(tag->album().to8Bit(true)):album;
            
            size_t totalSeconds = 0;
            if (f.audioProperties() && f.audioProperties()->lengthInSeconds() > 0) {
                totalSeconds = f.audioProperties()->lengthInSeconds();
            }

            size_t hours = totalSeconds / 3600;
            size_t minutes = (totalSeconds % 3600) / 60;
            size_t seconds = totalSeconds % 60;
            duration = (hours > 0 ? QString("%1:").arg(hours) : "") +
                      QString("%1:%2")
                          .arg(minutes, hours > 0 ? 2 : 1, 10, QChar('0'))
                          .arg(seconds, 2, 10, QChar('0'));

            if (!isSongExists){ //retrieve  image only  if the song is new 
                TagLib::MPEG::File *mpegFile = dynamic_cast<TagLib::MPEG::File*>(f.file());
                std::vector<TagLib::ByteVector> pictures;
                if (mpegFile && mpegFile->ID3v2Tag()) {
                    TagLib::ID3v2::FrameList frameList = mpegFile->ID3v2Tag()->frameList("APIC");
                    if (!frameList.isEmpty()) {
                        auto *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
                        pictures.push_back(pic->picture());
                    }
                }
                if (!pictures.empty()) {
                    TagLib::ByteVector coverArt = pictures[0];
                    QString coverFileName = QString::number(QRandomGenerator::global()->generate()) + ".jpg";
                    QString coverPath =  QString::fromStdString(std::filesystem::current_path().string()) + "\\data\\covers\\" + coverFileName;
                    
                    // Create covers directory if it doesn't exist
                    QDir().mkpath(QString::fromStdString(std::filesystem::current_path().string()) + "\\data\\covers");
                    
                    // Save the image data to file
                    QByteArray imageData(coverArt.data(), coverArt.size());
                    QFile file(coverPath);
                    if (file.open(QIODevice::WriteOnly)) {
                        file.write(imageData);
                        file.close();
                        coverImage = "file:///" + coverPath; // Update coverImage path for database
                    }
                }
            }
        }

        logSongs << "path : " << filePath.toStdString() << "\n" 
            << "title: " << title.toStdString()    << "\n"
            << "artist: " << artist.toStdString()    << "\n"
            << "album: " << album.toStdString()    << "\n"
            << "duration: " << duration.toStdString() << "\n"
            << "cover: " << coverImage.toStdString() <<"\n";
        
  
        if (isSongExists){
            oldSongs.push_back(filePath);
            logSongs << "exists !\n";
            // if (isSongMetaDataChanged(db, filePath, title, artist, album, logdb)){
            //     logSongs << "exists but changed!\n" ; 
            // }else {
            //     logSongs << "exists not changed!\n" ;  
            // } 
        }else {
            newSongs.push_back((Song){
                .filePath = filePath,
                .title = title ,
                .artist = artist,
                .album = album, 
                .duration = duration,
                .coverImage = coverImage,
                .isFav = 0
            });
            logSongs << "NEW !\n";
        }

        logSongs << "----------------" <<"\n";

    }

    if (newSongs.size()){    
        bool res = registerNewSongs(db, newSongs, scantime,  logdb);
    }
    if (oldSongs.size()){
        updateLastScanTime(db, oldSongs , scantime, logdb);
    }
    deleteRemovedSongs(db, scantime, logdb);

    logSongs.close();
    logdb.close();

    sqlite3_close(db);
}
