#include "commandlineread.h"
#include <string>
#include <vector>

Reader::Reader(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        commandline.push_back(argv[i]);
    }
};

bool Reader::find_member(std::string str) {
    for (int i = 0; i < commandline.size(); i++) {
        if (str == commandline[i]) {
            return true;
        }
    }
    return false;
}

std::string Reader::find_member_next(std::string str) {
    for (int i = 0; i < commandline.size(); i++) {
        if (str == commandline[i] && i != commandline.size() - 1) {
            return commandline[i+1];
        }
    }
    return "";
}







