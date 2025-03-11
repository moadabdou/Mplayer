#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <iostream>
#include <vector>
#include <fstream>

int main() {
    // Load the audio file
    TagLib::FileRef f("C:/Users/dell/Downloads/AUD-20240710-WA0126.mp3");

    // Check if the file is valid
    if (!f.isNull() && f.tag() != nullptr) {
        // Get the tag and check for attached pictures (cover art)
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
            // Extract the first cover art picture (if available)
            TagLib::ByteVector coverArt = pictures[0];

            // Save the cover art as a file (e.g., PNG, JPEG)
            std::ofstream outFile("cover.jpg", std::ios::binary);
            outFile.write(reinterpret_cast<const char*>(coverArt.data()), coverArt.size());
            outFile.close();

            std::cout << "Cover art saved as 'cover.jpg'" << std::endl;
        }
    } 

    return 0;
}
