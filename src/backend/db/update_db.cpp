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
        //dblogs << "Error checking record: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return exists;
}


bool registerNewSong(sqlite3* db, const QString& path, const QString& title, 
                    const QString& artist, const QString& album, const QString& coverImage, std::fstream& dblogs) {
    const char *sql = "INSERT INTO songs (path, title, artist, album, songCover, last_time_scanned) "
                      "VALUES (?, ?, ?, ?, ?, strftime('%s','now'));";
    sqlite3_stmt *stmt;
    bool success = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, path.toStdString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, title.toStdString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, artist.toStdString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, album.toStdString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, coverImage.toStdString().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            success = true;
        } else {
            //dblogs << "Error inserting record: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_finalize(stmt);
    } else {
        //dblogs << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
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
        //dblogs << "Error checking metadata: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return changed;
}

void updateDatabase(const QString&  folderPath) {

    const std::string dbPath = std::filesystem::current_path().string() + "/data/songs.db";
    sqlite3 *db;

    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        qDebug() << "Error opening database: " << sqlite3_errmsg(db) << Qt::endl;
        qDebug() << std::filesystem::current_path().string();
        return;
    }

    const char *createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS songs (
            path TEXT PRIMARY KEY,     -- Absolute path as the unique ID
            title TEXT,                -- Song title
            album TEXT,                -- Album name
            artist TEXT,               -- Artist name
            songCover TEXT,            -- Base64 encoded cover image
            is_fav INTEGER DEFAULT 0,  -- 0 = not favorite, 1 = favorite
            play_count INTEGER DEFAULT 0, -- Number of times played
            is_old INTEGER DEFAULT 0,  -- 0 = active, 1 = missing (was scanned before but now gone)
            last_time_scanned INTEGER  -- Unix timestamp of the last scan
        );
    )";
    
    if (sqlite3_exec(db, createTableSQL, 0, 0, nullptr) != SQLITE_OK) {
        qDebug() << "Error creating table: " << sqlite3_errmsg(db) << Qt::endl;
        sqlite3_close(db);
        return;
    }

    QDir directory(folderPath);
    QStringList filters = { "*.mp3"};
    QStringList fileList = directory.entryList(filters, QDir::Files);

    std::fstream logSongs(std::filesystem::current_path().string() + "/logs/songsScan.log", std::ios::out);
    std::fstream logdb(std::filesystem::current_path().string() + "/logs/dbtransactions.log", std::ios::out);

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
            album =  !tag->artist().isEmpty()? QString::fromStdString(tag->album().to8Bit(true)):album;
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

        logSongs << "path : " << filePath.toStdString() << "\n" 
            << "title: " << title.toStdString()    << "\n"
            << "artist: " << artist.toStdString()    << "\n"
            << "album: " << album.toStdString()    << "\n"
            << "cover: " << coverImage.toStdString() <<"\n";


        logSongs << "----------------" <<"\n";
        

        if (isSongExists){
            if (isSongMetaDataChanged(db, filePath, title, artist, album, logdb)){
                logSongs << "exists but changed!\n" ; 
            }else {
                logSongs << "exists not changed!\n" ;  
            } 
        }else {
            bool res = registerNewSong(db, filePath, title, artist, album, coverImage, logdb);
            if  (res){
                logSongs << "registred\n" ;
            }else {
                logSongs << "error registration \n";
            }
        }

    }

    logSongs.close();
    logdb.close();

    sqlite3_close(db);
}
