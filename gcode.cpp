#include "gcode.h"
#include <string>




// simple out function that outputs what is 
std::string GCodeInstruction::out() {
    if (command_char == 'G') {
        if (command_number == 0 || command_number == 1) {
            std::string str = command_char + (command_number<10? "0" + std::to_string(command_number) : std::to_string(command_number)) + (_x ? (" X" + std::to_string(x)) : "") + (_y? (" Y" + std::to_string(y)) : "") + (_z? (" Z" + std::to_string(z)) : "");
            return str + (command_number == 1? (" F" + std::to_string(f)) : "");
        } else if (command_number == 2 || command_number == 3) {
            std::string str = command_char + (command_number<10? "0" + std::to_string(command_number) : std::to_string(command_number)) + (_x ? (" X" + std::to_string(x)) : "") + (_y? (" Y" + std::to_string(y)) : "") + (_z? (" Z" + std::to_string(z)) : "")+ (_i? (" I" + std::to_string(i)) : "")+ (_j? (" J" + std::to_string(j)) : "");
            return str + (_f ? (" F" + std::to_string(f)) : "");
        }
    }
    return "idk";
};

