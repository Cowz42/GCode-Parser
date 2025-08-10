#include "gcode.h"
#include <string>



std::string GCodeInstruction::out() {
    std::string str = command_char + (command_number<10? "0" + std::to_string(command_number) : std::to_string(command_number)) + (_x ? (" X" + std::to_string(x)) : "") + (_y? (" Y" + std::to_string(y)) : "") + (_z? (" Z" + std::to_string(z)) : "")+ (_i? (" I" + std::to_string(i)) : "") + (_j? (" J" + std::to_string(j)) : "") + (_s? (" S" + std::to_string(s)) : "");
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
    _s = false;
};

bool GCodeInstruction::check_values(char* arr, int arr_len) {
    if (arr == nullptr) {
        if (!(_x || _y || _z)) {
            return false;
        }
    }
    for (int i = 0; i < arr_len; i++) {
        if (arr[i] == 'X' && !_x) {
            return false;
        } else if (arr[i] == 'Y' && !_y) {
            return false;
        } else if (arr[i] == 'Z' && !_z) {
            return false;
        } else if (arr[i] == 'F' && !_f) {
            return false;
        } else if (arr[i] == 'I' && !_i) {
            return false;
        } else if (arr[i] == 'J' && !_j) {
            return false;
        } else if (arr[i] == 'S' && !_s) {
            return false;
        }
    }
    return true;
};