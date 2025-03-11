#include <iostream>
#include <filesystem>
#include <vector>
#include <windows.h>


namespace fs = std::filesystem;

std::vector<std::string> scanMusicFolder(const std::string& folderPath) {
    std::vector<std::string> musicFiles;
    
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        std::cerr << "Invalid folder: " << folderPath << std::endl;
        return musicFiles;
    }

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".mp3" || ext == ".flac" || ext == ".wav" || ext == ".ogg" || ext == ".aac") {
                musicFiles.push_back(entry.path().string());
            }
        }
    }

    return musicFiles;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::string folderPath = "C:/Users/dell/Downloads"; // Change this to your folder
    std::vector<std::string> songs = scanMusicFolder(folderPath);

    std::cout << "Found ✅ " << songs.size() << " audio files:\n";
    for (const auto& song : songs) {
        std::cout << song << std::endl;
    }

    return 0;
}
