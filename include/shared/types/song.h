#ifndef SONG_H
#define SONG_H

#include <QString>  // Or other relevant types

struct Song {
    QString filePath;
    QString title;
    QString artist;
    QString album;
    QString duration;
    QString coverImage;  // Path to the cover image
    bool isFav;  
};

#endif // SONG_H