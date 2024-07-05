#include <iostream>
#include <filesystem>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstdlib>

#define PDF_FILE ".pdf"

namespace fs = std::filesystem;

using namespace std;

typedef std::vector<std::string> stringvec; 


// 1. i will look for the path, in my case, the download folder ✅
// 2. i wanna store all files in a vector, only files
// 3. then i will check for each file's extension
// 4. for each file, i want to store it in the corresponding folder, so .pdf will go to pdf, 
// .img will go to images, but also. jpg will go to images
// 5. i will check if a file's extension directory does not exist, i will create a folder for them
// 6. i will not create a seperate folder for every new extension, anything that is not a pdf, img, etc...
// will go to "others" folder


bool isImage(const string &ext){
    return ext == "png" || ext == "jpeg" || ext == "jpg" || ext == "webp";
}

int createDirectory(stringvec &directories, const string &myDir) {
    if(std::find(directories.begin(), directories.end(), myDir) == directories.end()) {
        if(fs::create_directory(myDir)){
            cout << "a directory has been successfully created: " << myDir;
            directories.push_back(myDir);
        } else {
            cout << "Error: directory already exists";
        }
    }
}

int MoveFromTo (const string &sourceFile, const string &destinationFile){
    fs::path filePath(sourceFile);
    fs::rename(sourceFile, destinationFile + "/" + filePath.filename().string());
    return 0;
}
int main(){

    const char* homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        std::cerr << "Unable to get HOME directory." << std::endl;
        return 1;
    }

    std::string downloadsPath = std::string(homeDir) + "/Downloads";

    stringvec files;
    stringvec directories;
    for(const auto &entry : fs::directory_iterator(downloadsPath)) {
        if(fs::is_regular_file(entry.path()))
            files.push_back(entry.path());
        else if (fs::is_directory(entry.path()))
            directories.push_back(entry.path());
    };


    for(const auto &file : files){
        int pos = file.find(".")+1;
        string ext = file.substr(pos, file.length()-1);

         try {
            if(ext == "pdf") {
                createDirectory(directories, downloadsPath + "/pdf");
                MoveFromTo(file, downloadsPath + "/pdf");
            } 
            else if(ext == "docx" || ext == "doc") {
                createDirectory(directories, downloadsPath + "/docs");
                MoveFromTo(file, downloadsPath + "/docs");

            } 
            else if(ext == "mp3" || ext == "wav") {
                createDirectory(directories, downloadsPath + "/audio");
                MoveFromTo(file, downloadsPath + "/audio");

            } 
            else if(ext == "mp4" || ext == "webm" || ext == "mov" || ext == "avi") {
                createDirectory(directories, downloadsPath + "/videos");
                MoveFromTo(file, downloadsPath + "/videos");
            } 
            else if(isImage(ext)) {
                createDirectory(directories, downloadsPath + "/images");
                MoveFromTo(file, downloadsPath + "/images");

            } 
            else {
                cout << ext << endl;
                createDirectory(directories, downloadsPath + "/others");
                MoveFromTo(file, downloadsPath + "/others");
            }
        } catch (const fs::filesystem_error &e) {
            cout << "error moving file: " << e.what() << endl;
        }
    }
    
    return 0;
}