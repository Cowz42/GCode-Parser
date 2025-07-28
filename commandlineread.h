


#ifndef commandlineread_h
#define commandlineread_h

#include <string>
#include <vector>

class Reader {
public:
    // contents of teh command line
    std::vector<std::string> commandline;
    // constructor takes the command line arguments straight through
    Reader(int argc, char* argv[]);
    // finds a specific argument, e.g. finds a -f to trigger something
    bool find_member(std::string str);
    // finds an argument with it's value, e.g. -a RETURNS_THIS
    std::string find_member_next(std::string str);
};




#endif