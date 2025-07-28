/*
GCode parsing class








*/



#ifndef gcodeparse_h
#define gcodeparse_h

#include "gcode.h"

// Primary class for this
class GCodeParse {
private:
    std::string file_name;
    std::string current_command;
    // current file that is being used to run the gcode
    std::ifstream Working_File;
    // pointers stored so you can just pass the addresses and it will write them automatically
    int command_number;
    float* x_ptr; float* y_ptr; float* z_ptr; float* f_ptr; float* i_ptr; float* j_ptr;
    float x_val, y_val, z_val, f_val, i_val, j_val;
    float read_float(std::string str);
    int64_t read_int(std::string_view str);
    int crawl_too(std::string_view str, char crawl_char, int start_from = 0);
    float crawl_too_number(std::string str, char crawl_too);

public:
    GCodeParse() {}
    int set_working_file(std::string filename);
    GCodeInstruction* read_command();
    void write_simple_values(float* x_out, float* y_out, float* z_out, float* f_out, float* i_out, float* j_out);
};

#endif