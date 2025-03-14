#include <iostream>
#include <cstdlib>

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

int main() {
    std::string filePath = "song.mp3";
    int duration = getAudioDuration(filePath);

    if (duration > 0)
        std::cout << "Duration: " << duration << " seconds\n";
    else
        std::cout << "Failed to get duration.\n";

    return 0;
}
