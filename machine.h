



#ifndef machine_h
#define machine_h

#include "plane.h"

class Machine {
private:
    float x, y, z, f, s;
    Plane plane;
    bool absolute;

public:

    Machine();
    float get_x();
    float get_y();
    float get_z();
    float get_f();
    float get_s();
    Plane get_plane();

    void set_f(float new_f);
    void set_s(float new_s);
};




#endif