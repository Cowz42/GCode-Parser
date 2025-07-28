#include <fstream>
#include <string>
#include <iostream>
#include "gcodeparse.h"
#include "gcode.h"
using namespace std;





int GCodeParse::set_working_file(string filename) {
    file_name = filename;
    Working_File = ifstream(file_name);
    return 0;
};


// Function that reads the next command in the file, parses it, and outputs a GCodeInstruction with the data, 
// and writes the pointers it has for more direct access
GCodeInstruction* GCodeParse::read_command() {
    float x = 0, y = 0, z = 0, f = 0, i = 0, j = 0;

    // gets working line of gcode
    getline(Working_File, current_command);

    // if the line is empty, then it just returns nullptr as their is no data
    if (current_command.empty()) {
        return nullptr;
    }

    // cuts off the command at the comment marker
    int comment_excluder = crawl_too(current_command, ';');
    if (comment_excluder != current_command.length() - 1) {
        current_command = current_command.substr(0, comment_excluder);
    }

    // wrote before I made a function for this, but this is way faster and I don't care
    command_number = current_command[2] - 48;
    command_number += (current_command[1] - 48) * 10;

    // allocate a GCodeInstruction, the calling function is responsible for freeing the memory, and it could cause memory leaks
    GCodeInstruction* out = (GCodeInstruction*)malloc(sizeof(GCodeInstruction));
    out->reset();

    // subtracted one cause I only need the length -1, and so it just does the math once
    int length = current_command.length() - 1;



    // crawls for every character, if it finds it, then it marks it as present in the output GCodeInstruction, by default they aren't
    // along with also calculating the value, and then storing it for other parts
    if (crawl_too(current_command, 'X') != length) {
        x = crawl_too_number(current_command, 'X');
        out->_x = true;
    }
    if (crawl_too(current_command, 'Y') != length) {
        y = crawl_too_number(current_command, 'Y');
        out->_y = true;
    }
    if (crawl_too(current_command, 'Z') != length) {
        z = crawl_too_number(current_command, 'Z');
        out->_z = true;
    }
    if (crawl_too(current_command, 'I') != length) {
        i = crawl_too_number(current_command, 'I');
        out->_i = true;
    }
    if (crawl_too(current_command, 'J') != length) {
        j = crawl_too_number(current_command, 'J');
        out->_j = true;
    }
    if (crawl_too(current_command, 'F') != length) {
        f = crawl_too_number(current_command, 'F');
        out->_f = true;
    }

    // writes the values to the local stored in the class
    x_val = x;
    y_val = y;
    z_val = z;
    f_val = f;
    i_val = i;
    j_val = j;

    // stores the values to the output GCodeInstruction
    // cout << "X: " << to_string(x) << "\tY: " << to_string(y) << "\tZ: " << to_string(z) << "\tF: " << to_string(f) << "\n";
    out->x = x;
    out->y = y;
    out->z = z;
    out->f = f;
    out->i = i;
    out->j = j;


    
    // default for G commands, with x,y,z,f,i,j all recorded by default, and will mark it if it isn't present
    // IDK how other commands work, but this should work for reading them all
    out->command_char = current_command[0];
    out->command_number = command_number;
    return out;
};

// converts a given string into a float, WARNING: will not check if there are only number chars, will just subtract the ascii values
float GCodeParse::read_float(string str) {
    //sign check
    bool negative = false;
    if (str[0] == '-') {
        negative = true;
        str = str.substr(1,str.length()-1);
    }
    // searches for a decimal point if there is one, and then splits it
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

    // sending the parts to read_int() to parse seperatly
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
    // goes through array in reverse for more convienent power multiplcation
    for (int i = str.length() - 1; i > -1; i--) {
        // makes power of digit based off of index
        int mult = 1;
        for (int j = 0; j < str.length() - 1 - i; j++) {
            mult *= 10;
        }
        // char gets 48 subtracted from it as '0' = int 48, so the conversion is simple
        // also multiplied by the power
        sum += (str[i] - 48) * mult;
    }
    return sum;
};


int GCodeParse::crawl_too(string_view str, char crawl_char, int start_from) {
    // checks for out of range
    if (start_from >= str.length()) {return -1;}
    // searches looking for the value
    for (int i = start_from; i < str.length(); i++) {
        if (str[i] == crawl_char) {
            return i;
        }
    }
    // default return is the end index in the array
    return str.length() - 1;
};

void GCodeParse::write_simple_values(float* x_out = nullptr, float* y_out = nullptr, float* z_out = nullptr, float* f_out = nullptr, float* i_out = nullptr, float* j_out = nullptr) {
    // makes sure that you give it memory addresses to work with
    if (!(x_out == nullptr || y_out == nullptr || z_out == nullptr || f_out == nullptr || i_out == nullptr || j_out == nullptr)) {
        x_ptr = x_out;
        y_ptr = y_out;
        z_ptr = z_out;
        f_ptr = f_out;
        i_ptr = i_out;
        j_ptr = j_out;
    }
    // stores the values to the memory addresses
    *x_ptr = x_val;
    *y_ptr = y_val;
    *z_ptr = z_val;
    *f_ptr = f_val;
    *i_ptr = i_val;
    *j_ptr = j_val;
};

float GCodeParse::crawl_too_number(string str, char crawl_too) {
    float val;
    // finds the search character
    int start = this->crawl_too(str, crawl_too);
    if (start != -1) {
        // attempts to find the end of the number, in gcodes case, a space or a line end
        int end = this->crawl_too(str, ' ', start);
        if (end != -1) {
            // splits str to get only the number part, if it is the line end, then it makes sure to include that value, othewise, the space has to be discarded
            string str_out = str.substr(start + 1,end - start - (end == str.length() - 1? 0 : 1));
            str_out = reverse_crawl(str_out);
            // calls read_float to do the rest
            val = read_float(str_out);
        }
    }
    return val;
};

string GCodeParse::reverse_crawl(string str) {
    // recursively calls it self chopping off the last character till it gets to numbers
    int length = str.length() - 1;
    if (str[length] > 57 || str[length] < 48) {
        return reverse_crawl(str.substr(0, length - 1));
    }
    return str;
};