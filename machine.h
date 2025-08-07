



#ifndef machine_h
#define machine_h

#include "plane.h"
#include "gcode.h"

class Machine {
private:
    float x, y, z, f, s;
    float new_x, new_y, new_z;
    Plane plane;
    bool absolute;
    float speed_x, speed_y, speed_z;
    float time;

    // finds the speeds for the low level component
    int calc_speeds();
    // 3d distance function
    double dist(float x1, float y1, float z1, float x2, float y2, float z2);
    // function unique to the baremetal and it's control interfaces, has to be written by the bare metal designer/programmer
    // I can't really define a function that's always going to be different
    void run_low();

public:

    Machine();
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