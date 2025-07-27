#include "gcode.h"
#include <string>





std::string GCodeInstruction::out() {
    if (command_char == 'G') {
        if (command_number == 0 || command_number == 1) {
            return command_char + (command_number<10?"0" + command_number : std::to_string(command_number)) + " X" + std::to_string(x)+ " Y" + std::to_string(y)+ " Z" + std::to_string(z)+ " F" + std::to_string(f);
        }
    }
    return "idk";
};

