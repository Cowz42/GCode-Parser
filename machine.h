/*
Machine Class, handles various varibles having to do with the 

Machine::run_low() needs to be user written, as it deals with the low level parts of the system

also need to supply the bounds of the working enviroment to the Machine constructor, so it will error itself out instead of breaking things

*/



#ifndef machine_h
#define machine_h

#include "plane.h"
#include "gcode.h"
#include "bounds.h"

class Machine {
private:
    // function unique to the baremetal and it's control interfaces, has to be written by the bare metal designer/programmer
    // I can't really define a function that's always going to be different
    void run_low(float in_x, float in_y, float in_z, float s_x, float s_y, float s_z);

    // NEEDS run_low() to change the x, y, z

    // current system position
    float x, y, z, f, s;

    // speeds to follow for the postion
    float speed_x, speed_y, speed_z;

    // new position to go to 
    float new_x, new_y, new_z;

    // time it would take to get their based off of position
    float time;

    Plane plane;
    bool absolute;
    Bounds bounds;
    

    // finds the speeds for the low level component
    int calc_speeds();
    // 3d distance function
    double dist(float x1, float y1, float z1, float x2, float y2, float z2);
    // checks to make sure everything is in range for the movement
    bool check_bounds();


public:

    Machine(Bounds bounds_in);
    // boiler plate
    float get_x();
    float get_y();
    float get_z();
    float get_f();
    float get_s();
    Plane get_plane();

    // basic operation called to run through a command, the other parts will have to be done in the main file
    int run(GCodeInstruction* command);

    void set_f(float new_f);
    void set_s(float new_s);
};




#endif