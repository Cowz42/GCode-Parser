#include "gcode.h"
#include <string>




// simple out function that outputs what is 
std::string GCodeInstruction::out() {
    std::string str = command_char + (command_number<10? "0" + std::to_string(command_number) : std::to_string(command_number)) + (_x ? (" X" + std::to_string(x)) : "") + (_y? (" Y" + std::to_string(y)) : "") + (_z? (" Z" + std::to_string(z)) : "")+ (_i? (" I" + std::to_string(i)) : "")+ (_j? (" J" + std::to_string(j)) : "");
    return str + (_f ? (" F" + std::to_string(f)) : "");

    // for if certain characters are later blacklisted, for future use
    // return "Unknown Command: " + command_char + std::to_string(command_number);
};

void GCodeInstruction::reset() {
    _x = false;
    _y = false;
    _z = false;
    _f = false;
    _i = false;
    _j = false;
};