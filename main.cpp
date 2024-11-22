#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <dirent.h>
#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <cctype>


using arguments_t = std::vector<std::pair<std::string, std::string>>;

std::string code2string(const int code, const std::string action = "sorted!") {
    switch (code) {
        case 200:
            return "Successfully " + action;
        case 404:
            return "Filepath not found!";
        case 400:
            return "Could not complete operation!";
        default:
            return "Bad!";

    }
}

std::string getExtension(std::string string){
    if (string[0] == '.'){
        return "NULL";
    }
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return string.substr(string.rfind('.')+1);
}

struct File_t{
    std::string filename;
    // Extension without .
    std::string extension;
    dirent * File_d;
};

class Arguments {
public:
    std::string sortMode;
    std::string filePath;

    Arguments(const int argc, const char* argv[]) : _argc(argc), _argv(argv) {parse();}


    void parse() {
        if (_argc == 1) {
            std::cout << "No args provided!\n";
            std::exit(1);
        }

        if (_argc <= 2){
            filePath = ".";
        } else {
            filePath = _argv[2];
        }

        sortMode = _argv[1];


    }
private:
    int _argc;
    const char** _argv;
};

class SortFiles {

public:
    static int isDir(const char* fileName)
    {
        struct stat path;

        stat(fileName, &path);

        return S_ISREG(path.st_mode);
    }
    static int byType(const char * filepath, const char *execName) {
        std::vector<struct File_t> dirFiles;
        std::vector<std::string> extensions;
        DIR *d;
        struct dirent *entry;
        std::string currExt;
        std::cout << filepath;
        struct File_t currFile;
        execName += 2;

        if ((d = opendir(filepath)) == nullptr)
            return 404;
        else {
            while ((entry = readdir(d)) != nullptr)
                if (isDir(entry->d_name) != 0) {
                    if (std::string(entry->d_name) == std::string(execName)){
                        continue;
                    }
                    currFile.filename = entry->d_name;
                    currExt = getExtension(entry->d_name);
                    if (currExt == "NULL"){
                        continue;
                    }
                    if (std::find(extensions.begin(), extensions.end(), currExt) == extensions.end()){
                        extensions.push_back(currExt);
                    }
                    currFile.extension = currExt;
                    currFile.File_d = entry;
                    dirFiles.push_back(currFile);
                }
            closedir(d);
        }
        for (int i = 0; i < extensions.size(); i++) {
            std::string createFilepath;
            if (filepath == std::string(".")){
                createFilepath = "";
            } else {
                createFilepath = filepath;
            }

            std::filesystem::path fullPath = std::filesystem::current_path() / std::filesystem::path(createFilepath) / extensions.at(i);

            if (!std::filesystem::create_directory(fullPath)){
                 std::cerr << "Failed to create directory: " << fullPath << std::endl;
                 return 400;
            }
        }

        for (int i = 0; i < dirFiles.size(); i++) {
            std::filesystem::rename(std::filesystem::current_path() / dirFiles.at(i).filename, std::filesystem::current_path() / std::filesystem::path(dirFiles.at(i).extension) / dirFiles.at(i).filename);
        }

        return 200;
    }
};


int main(const int argc, const char *argv[]) {
    Arguments args(argc, argv);

    if (args.sortMode == "--type" || args.sortMode == "-t") {
        std::string result = code2string(SortFiles::byType(args.filePath.c_str(), argv[0]));
        std::cout << result << "\n";
    }
    else if (args.sortMode == "--help" ||args.sortMode == "-h") {
        std::cout << argv[0] << " <sort mode> <filepath>\n";
        std::cout << "Sort mode options:\n --type, -t -> Sorts by type.\n";
        std::cout << "--help, -h -> Shows help message.\n";
    }
    else {
        std::cout << argv[0] << " <sort mode> <filepath>\n";
        std::cout << "Sort mode options:\n --type, -t -> Sorts by type.\n";
        std::cout << "--help, -h -> Shows help message.\n";
    }

    return 0;
}
