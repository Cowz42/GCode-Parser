#include <fstream>
#include <string>
#include <iostream>
#include "gcodeparse.h"
#include "gcode.h"
using namespace std;





// #error "cpp file"


int GCodeParse::set_working_file(string filename) {
    file_name = filename;
    Working_File = ifstream(file_name);
    return 0;
};


// Function that reads the next command in the file, parses it, and outputs a GCodeInstruction with the data, 
//and writes the pointers it has for more direct access
GCodeInstruction* GCodeParse::read_command() {
    float x = 0, y = 0, z = 0, f = 0;
    Command_Types command;

    
    getline(Working_File, current_command);
    // std::cout << current_command << "\n";
    if (current_command.empty()) {
        return nullptr;
    }

    if (current_command[0] == 'G') {
        command_number = current_command[2] - 48;
        command_number += (current_command[1] - 48) * 10; 
        command = G;
    } else {return nullptr;}
    
    if (command == G && (command_number == 1 || command_number == 0)) {
        // simple commands, handeled in a really simple way and I'm lazy

        x = crawl_too_number(current_command, 'X');
        y = crawl_too_number(current_command, 'Y');
        z = crawl_too_number(current_command, 'Z');
        f = 0;
        // G00 has no speed, it's just max, so a zero will be returned and indicate max speed
        if (command_number == 1) {
            f = crawl_too_number(current_command, 'F');
        }
        x_val = x;
        y_val = y;
        z_val = z;
        f_val = f;

        // cout << "X: " << to_string(x) << "\tY: " << to_string(y) << "\tZ: " << to_string(z) << "\tF: " << to_string(f) << "\n";
        GCodeInstruction* out = (GCodeInstruction*)malloc(sizeof(GCodeInstruction));
        out->x = x;
        out->y = y;
        out->z = z;
        out->f = f;
        out->command_number = command_number;
        out->command_char = 'G';
        return out;
    }
    return nullptr;
};

// converts a given string into a float, WARNING: will not check if there are only number chars, will just subtract the ascii values
float GCodeParse::read_float(string str) {
    bool negative = false;
    if (str[0] == '-') {
        negative = true;
        str = str.substr(1,str.length()-1);
    }

    int split = 0;
    while(true) {
        if (str[split] == '.') {
            break;
        }
        split++;
        if (split == str.length()) {
            split = -1;
            break;
        }
    }
    float sum = 0;

    // calculates part below zero, then divides it to correct and make it the actual decimal
    // extra check if there even is a decimal part
    if (split != -1) {
        string decimalstr = str.substr(split + 1, str.length() - 1);
        int64_t decimal = read_int(decimalstr);
        int64_t div = 1;
        for (int i = 0; i < decimalstr.length(); i++) {
            div *= 10;
        }
        sum = decimal;
        sum /= div;
        sum += read_int(str.substr(0, split));
        return sum * (negative ? -1 : 1);
    }
    
    // reads part above zero and calculates it
    sum += read_int(str);
    return sum * (negative ? -1 : 1);

};

int64_t GCodeParse::read_int(string_view str) {
    int64_t sum = 0;
    for (int i = str.length() - 1; i > -1; i--) {
        int mult = 1;
        for (int j = 0; j < str.length() - 1 - i; j++) {
            mult *= 10;
        }
        sum += (str[i] - 48) * mult;
    }
    return sum;
};


int GCodeParse::crawl_too(string_view str, char crawl_char, int start_from) {
    if (start_from >= str.length()) {return -1;}
    for (int i = start_from; i < str.length(); i++) {
        if (str[i] == crawl_char) {
            return i;
        }
    }
    return str.length() - 1;
};

void GCodeParse::write_simple_values(float* x_out = nullptr, float* y_out = nullptr, float* z_out = nullptr, float* f_out = nullptr) {
    if (!(x_out == nullptr || y_out == nullptr || z_out == nullptr || f_out == nullptr)) {
        x = x_out;
        y = y_out;
        z = z_out;
        f = f_out;
    }
    *x = x_val;
    *y = y_val;
    *z = z_val;
    *f = f_val;
};

float GCodeParse::crawl_too_number(string str, char crawl_too) {
    float val;
    int start = this->crawl_too(str, crawl_too);
    if (start != -1) {
        int end = this->crawl_too(str, ' ', start);
        if (end != -1) {
            string str_out = str.substr(start + 1,end - start - 1);
            val = read_float(str_out);
        }
    }
    return val;
};