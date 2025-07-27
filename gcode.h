


#ifndef gcode_h
#define gcode_h

#include <string>

class GCodeInstruction {
public:
    float x, y, z, f, i, j;
    int command_number;
    char command_char;
    std::string out();
};



#endif