/*
Class that just contains a simple class too be tossed around between parts containing all the data, in an already parsed way

*/


#ifndef gcode_h
#define gcode_h

#include <string>

class GCodeInstruction {
public:
    float x, y, z, f, i, j;
    // info on if these values were defined or left blank
    bool _x = false, _y = false, _z = false, _f = false, _i = false, _j = false;
    int command_number;
    char command_char;
    std::string out();
};



#endif