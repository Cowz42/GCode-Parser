#include <iostream>
#include <string>
#include "gcodeparse.h"
#include "commandlineread.h"
using namespace std;





GCodeParse parser = GCodeParse();


// takes the file name of what it is going to run through -f FILENAME.gcode
int main (int argc, char* argv[]) {
    Reader read = Reader(argc, argv);

    parser.set_working_file(read.find_member_next("-f"));
    while(true) {
        GCodeInstruction* current = parser.read_command();
        if (current == nullptr) {
            break;
        }

        std::cout << current->out() << "\n";
    }
    return 0;
};