/*
Class that just contains a simple class too be tossed around between parts containing all the data, in an already parsed way

*/


#ifndef gcode_h
#define gcode_h

#include <string>

// Class that contains all the information in a gcode line, with varibles and instruction type and number
class GCodeInstruction {
public:
    float x, y, z, f, i, j, s;
    // info on if these values were defined or left blank
    bool _x = false, _y = false, _z = false, _f = false, _i = false, _j = false, _s = false;
    int command_number;
    char command_char;
    // Prints out the line as it was before parsing
    std::string out();
    // resets bools to prevent any weird memory trickery from adding a 0 value in
    void reset();
    bool check_values(char* arr = nullptr, int arr_len = 0);
};



#endif