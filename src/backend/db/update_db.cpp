#include <sqlite3.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <QDir> 
#include <QDebug>
#include <QRandomGenerator>
#include <taglib/fileref.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include "shared/types/song.h"


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
void updateLastScanTime(sqlite3* db,const QString& path,size_t time, std::fstream& dblogs) {
    const char *sql = "UPDATE songs SET last_time_scanned = ?, is_old=1 WHERE path = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, time);
        sqlite3_bind_text(stmt, 2, path.toStdString().c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            dblogs << "Error updating last scan time: " << sqlite3_errmsg(db) << std::endl;
        }
    }else {
        dblogs << "Error preparing in last scan time: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}


bool registerNewSongs(sqlite3* db,std::vector<Song> songs ,size_t scantime, std::fstream& dblogs) {
    std::string sql = "INSERT INTO songs (path, title, artist, album, songCover, last_time_scanned) VALUES ";
    
    // Build the query with multiple value sets
    for (size_t i = 0; i < songs.size(); i++) {
        sql += "(?, ?, ?, ?, ?, ?)";
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


void updateDatabase(const QString&  folderPath) {

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
            is_fav INTEGER DEFAULT 0,  -- 0 = not favorite, 1 = favorite
            play_count INTEGER DEFAULT 0, -- Number of times played
            is_old INTEGER DEFAULT 0,  
            last_time_scanned INTEGER  -- Unix timestamp of the last scan
        );
    )";
    
    if (sqlite3_exec(db, createTableSQL, 0, 0, nullptr) != SQLITE_OK) {
        //qDebug() << "Error creating table: " << sqlite3_errmsg(db) << Qt::endl;
        sqlite3_close(db);
        return;
    }

    QDir directory(folderPath);
    QStringList filters = { "*.mp3"};
    QStringList fileList = directory.entryList(filters, QDir::Files);

    std::fstream logSongs(std::filesystem::current_path().string() + "/logs/songsScan.log", std::ios::out);
    std::fstream logdb(std::filesystem::current_path().string() + "/logs/dbtransactions.log", std::ios::out);
    
    
    std::vector<Song> newSongs;
    size_t scantime = QDateTime::currentSecsSinceEpoch();


    for (const QString& file : fileList) {
        QString filePath = folderPath + "/" + file;


        bool isSongExists = isSongExist(db, filePath.toStdString().c_str() , logdb);

        // Open file with TagLib
        TagLib::FileRef f(filePath.toStdString().c_str());
        QString title = file;
        QString artist = "Unknown Artist";
        QString album = "Unknown Album";
        QString coverImage = "../../../res/images/defaultCover.jpg";

        if (!f.isNull() && f.tag()) {
            TagLib::Tag* tag = f.tag();
            title = !tag->title().isEmpty()? QString::fromStdString(tag->title().to8Bit(true)):title;
            artist = !tag->artist().isEmpty()? QString::fromStdString(tag->artist().to8Bit(true)):artist;
            album =  !tag->album().isEmpty()? QString::fromStdString(tag->album().to8Bit(true)):album;
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
                    QString coverPath = QString::fromStdString(std::filesystem::current_path().string()) + "\\data\\covers\\" + coverFileName;
                    
                    // Create covers directory if it doesn't exist
                    QDir().mkpath(QString::fromStdString(std::filesystem::current_path().string()) + "\\data\\covers");
                    
                    // Save the image data to file
                    QByteArray imageData(coverArt.data(), coverArt.size());
                    QFile file(coverPath);
                    if (file.open(QIODevice::WriteOnly)) {
                        file.write(imageData);
                        file.close();
                        coverImage = coverPath; // Update coverImage path for database
                    }
                }
            }
        }

        if (!coverImage.endsWith("defaultCover.jpg")){
            coverImage = "file:///" + coverImage;
        }

        logSongs << "path : " << filePath.toStdString().c_str() << "\n" 
            << "title: " << title.toStdString().c_str()    << "\n"
            << "artist: " << artist.toStdString().c_str()    << "\n"
            << "album: " << album.toStdString().c_str()    << "\n"
            << "cover: " << coverImage.toStdString().c_str() <<"\n";
        

        if (isSongExists){
            updateLastScanTime(db, filePath , scantime, logdb);
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
    deleteRemovedSongs(db, scantime, logdb);

    logSongs.close();
    logdb.close();

    sqlite3_close(db);
}
