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
        int x_start = crawl_too(current_command, 'X');
        if (x_start != -1) {
            int x_end = crawl_too(current_command, ' ', x_start);
            if (x_end != -1) {
                string x_str = current_command.substr(x_start + 1,x_end - x_start - 1);
                x = read_float(x_str);
            }
        }

        int y_start = crawl_too(current_command, 'Y');
        if (y_start != -1) {
            int y_end = crawl_too(current_command, ' ', y_start);
            if (y_end != -1) {
                string y_str = current_command.substr(y_start + 1,y_end - y_start - 1);
                y = read_float(y_str);
            }
        }

        int z_start = crawl_too(current_command, 'Z');
        if (z_start != -1) {
            int z_end = crawl_too(current_command, ' ', z_start);
            if (z_end != -1) {
                string z_str = current_command.substr(z_start + 1,z_end - z_start - 1);
                z = read_float(z_str);
            }
        }

        x = crawl_too_number(current_command, 'X');
        y = crawl_too_number(current_command, 'Y');
        z = crawl_too_number(current_command, 'Z');
        f = crawl_too_number(current_command, 'F');

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