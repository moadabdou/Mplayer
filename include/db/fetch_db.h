#ifndef FETCH_DB_H
#define FETCH_DB_H
#include <vector>
#include <fstream>
#include "shared/types/song.h"


void fectchSongs(std::vector<Song>& songs, std::fstream& dblogs, const QString& conditions = "1");



#endif //FETCH_DB_H