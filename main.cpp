#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


using arguments_t = std::vector<std::pair<std::string, std::string>>;

std::string code2string(const int code, const std::string &action = "sorted!") {
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

class Arguments {
public:
    std::string sortMode;
    std::string filepath;

    Arguments(const int argc, const char* argv[]) : _argc(argc), _argv(argv) {}


    void parse() {
        if (_argc == 1) {
            std::cout << "No args provided!\n";
            std::exit(1);
        }
        sortMode = _argv[1];
        if (_argc < 2) {
            filepath = ".";
        }
        //return {{"mode", sortMode}, {"filepath", filepath}};
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
    static int byType(const char* filepath) {
        std::vector<std::pair<std::string, dirent*>> dirFiles;
        DIR *d;
        struct dirent *entry;
        if ((d = opendir(".")) == nullptr)
            perror("opendir() error");
        else {
            while ((entry = readdir(d)) != nullptr)
                if (isDir(entry->d_name) != 0) {
                    dirFiles.push_back({"file", entry});
                }
            closedir(d);
        }
        for (int i = 0; i < dirFiles.size(); i++) {

            std::cout << i + 1<< ". " << dirFiles.at(i).second->d_name << "\n";
        }

        return 200;
    }
};



int main(const int argc, const char * argv[]) {
    Arguments args(argc, argv);
    args.parse();

    if (args.sortMode == "--type" || args.sortMode == "-t") {
        std::string result = code2string(SortFiles::byType(args.filepath.c_str()));
        std::cout << "Status: " << result << "\n";
    }
    else if (args.sortMode == "--help" ||args.sortMode == "-h") {
        std::cout << argv[0] << "<sort mode> <filepath>\n";
        std::cout << "Sort mode options:\n --type, -t -> Sorts by type.\n";
        std::cout << "--help, -h -> Shows help message.";
    }
    else {
        std::cout << argv[0] << "<sort mode> <filepath>\n";
        std::cout << "Sort mode options:\n --type, -t -> Sorts by type.\n";
        std::cout << "--help, -h -> Shows help message.";
    }

    return 0;
}
