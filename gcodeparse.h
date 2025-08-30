/*
GCode parsing class








*/



#ifndef gcodeparse_h
#define gcodeparse_h

#include "gcode.h"
#include <fstream>

// Class that reads a gcode file and outputs the commands using a different class
class GCodeParse {
private:
    std::string file_name;
    std::string current_command;

    // current file that is being used to run the gcode
    std::ifstream Working_File;

    // pointers stored so you can just pass the addresses and it will write them automatically
    int command_number;
    float* x_ptr; float* y_ptr; float* z_ptr; float* f_ptr; float* i_ptr; float* j_ptr; float* s_ptr;
    float x_val, y_val, z_val, f_val, i_val, j_val, s_val;

    // tells the read command to look through the current command again for an additional command
    bool multi_command = false;

    // takes a string and converts it into a float
    float read_float(std::string str);

    // takes a string and converts it into an int
    int64_t read_int(std::string str);

    // takes a string and sequentially tests for the test case, and returns the index of it
    int crawl_too(std::string str, char crawl_char, int start_from = 0);

    // finds a number after a specific character, e.g. return 1.5 from ("I like potatoes H1.5", 'H')
    float crawl_too_number(std::string str, char crawl_too);

    // function to fix a bug involving non allowed characters and how comments are processed
    std::string reverse_crawl(std::string str);

public:
    // sets the working file from current directory, from file name
    int set_working_file(std::string filename);

    // Function that reads the next command in the file, parses it, and outputs a GCodeInstruction with the data, 
    // and writes the pointers it has for more direct access
    // will also parse a string directly
    GCodeInstruction* read_command(std::string parse = "");

    // for pointer stuff if you don't want to deal with the class for the output
    void write_simple_values(float* x_out, float* y_out, float* z_out, float* f_out, float* i_out, float* j_out, float* s_out);
};

#endif