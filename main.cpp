#include <iostream>
#include <filesystem>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstdlib>

#define PDF_FILE ".pdf"

// 1. i will look for the path, in my case, the download folder ✅
// 2. i wanna store all files in a vector, only files
// 3. then i will check for each file's extension
// 4. for each file, i want to store it in the corresponding folder, so .pdf will go to pdf, 
// .img will go to images, but also. jpg will go to images
// 5. i will check if a file's extension directory does not exist, i will create a folder for them
// 6. i will not create a seperate folder for every new extension, anything that is not a pdf, img, etc...
// will go to "others" folder


namespace fs = std::filesystem;
using namespace std;

typedef std::vector<std::string> stringvec; 
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

        fs::path filePath(file);
        string sourceFile = filePath.string();
        string destDir = downloadsPath + "/" + ext;
        string destFile = destDir + "/" + filePath.filename().string();

        if(std::find(directories.begin(), directories.end(), ext) == directories.end())
            if(ext == "jpg" || "png" || "jpeg" || ".webp"){
                if(fs::create_directory(downloadsPath + "/" + "images")) {
                    cout << "created directory: " << "images" << " successfully" << endl;
                    directories.push_back(downloadsPath + "/" + "images");
                } else {
                    cout << "directory images already exists" << endl;
                }
            } else {
            if(fs::create_directory(downloadsPath + "/" + ext)){
                cout << "created directory: " << ext << " successfully" << endl;
                directories.push_back(downloadsPath + "/" + ext);
            }
            else{
                cout << "directory already exist: " << ext << endl; 
            }
            }

         try {
            if(ext == "jpg" || "png" || "jpeg" || ".webp") {
                fs::rename(sourceFile, downloadsPath + "/" + "images" + "/" + filePath.filename().string());
            } else
                fs::rename(sourceFile, destFile);
        } catch (const fs::filesystem_error &e) {
            cout << "error moving file: " << e.what() << endl;
        }
    }
    
    return 0;
}