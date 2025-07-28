


#ifndef commandlineread_h
#define commandlineread_h

#include <string>
#include <vector>

class Reader {
public:
    std::vector<std::string> commandline;
    Reader(int argc, char* argv[]);
    bool find_member(std::string str);
    std::string find_member_next(std::string str);
};




#endif