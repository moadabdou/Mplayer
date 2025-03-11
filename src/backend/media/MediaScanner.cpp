#include "media/MediaScanner.h" 
#include <QDir> 
#include <QDebug>
#include <filesystem>
#include <taglib/fileref.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>


namespace fs = std::filesystem;


QString imageToBase64(const QByteArray& imageData) {
    return "data:image/jpeg;base64," + imageData.toBase64();
}


MediaScanner::MediaScanner(QObject* parent) : QAbstractListModel(parent) {}

int MediaScanner::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(songs.size());
}

QVariant MediaScanner::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(songs.size())) return QVariant();

    const Song& song = songs.at(index.row());
    switch (role) {
        case FilePathRole: return song.filePath;
        case TitleRole: return song.title;
        case ArtistRole: return song.artist;
        case AlbumRole: return song.album;
        case CoverImageRole: return song.coverImage;
    }
    return QVariant();
}

QHash<int, QByteArray> MediaScanner::roleNames() const {
    return {
        { FilePathRole, "filePath" },
        { TitleRole, "title" },
        { ArtistRole, "artist" },
        { AlbumRole, "album" },
        { CoverImageRole, "coverImage" }
    };
}

void MediaScanner::scanFolder(const QString& folderPath) {
    beginResetModel();
    songs.clear();

    QDir directory(folderPath);
    QStringList filters = { "*.mp3"};
    QStringList fileList = directory.entryList(filters, QDir::Files);

    for (const QString& file : fileList) {
        QString filePath = folderPath + "/" + file;

        // Open file with TagLib
        TagLib::FileRef f(filePath.toStdString().c_str());
        QString title = file;
        QString artist = "Unknown Artist";
        QString album = "Unknown Album";
        QString coverImage = "../../../res/images/defaultCover.jpg";

        if (!f.isNull() && f.tag()) {
            TagLib::Tag* tag = f.tag();
            artist = !tag->artist().isEmpty()? QString::fromStdString(tag->artist().to8Bit(true)):artist;
            album =  !tag->artist().isEmpty()? QString::fromStdString(tag->album().to8Bit(true)):album;
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
                QByteArray imageData(coverArt.data(), coverArt.size());
                coverImage = imageToBase64(imageData);
            }
        }


        songs.push_back({ filePath, title, artist, album,coverImage }); 
    }

    endResetModel();
}

MediaScanner::~MediaScanner() {}
