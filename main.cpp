#include <iostream>
#include <fstream>
#include <string>
#include "gcodeparse.h"
#include "gcode.h"
using namespace std;





GCodeParse parser = GCodeParse();


// takes the file name of what it is going to run through -f FILENAME.gcode
int main (int argc, char* argv[]) {
    parser.set_working_file(argv[1]);
    parser.read_command();
    parser.read_command();
    return 0;
};