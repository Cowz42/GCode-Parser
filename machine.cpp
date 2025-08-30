#include "machine.h"
#include "gcode.h"
#include "plane.h"
#include "generatestep.h"
#include <cmath>
#include "bounds.h"

float abs(float n) {
    if (n >= 0) {
        return n;
    }
    return -n;
}


Machine::Machine(Bounds bounds_in) {
    // only real modes to run it in
    plane = XY;
    absolute = true;
    bounds = bounds_in;
};

// boiler plate
float Machine::get_x() {
    return x;
};
float Machine::get_y() {
    return y;
};
float Machine::get_z() {
    return z;
};
float Machine::get_f() {
    return f;
};
float Machine::get_s() {
    return s;
};
Plane Machine::get_plane() {
    return plane;
}

void Machine::set_f(float new_f) {
    f = new_f;
};
void Machine::set_s(float new_s) {
    s = new_s;
};

int Machine::run(GCodeInstruction* command) {
    if (command == nullptr) {
        return -1;
    }
    int steps = 1;

    StepGen stepgen = StepGen(command, this);

    // loops through until stepgen is finished with its task
    while(steps > 0) {
        steps = stepgen.next(&new_x, &new_y, &new_z);
        if (!check_bounds()) {
            return -2;
        }
        calc_speeds();
        run_low();
    }
    return 0;
};

// finds the speeds for the low level component
int Machine::calc_speeds() {
    double distance = dist(x, y, z, new_x, new_y, new_z);
    time = distance / f;
    speed_x = abs(x - new_x) / time;
    speed_y = abs(y - new_y) / time;
    speed_z = abs(z - new_z) / time;
    return 0;
};

// 3d distance function
double Machine::dist(float x1, float y1, float z1, float x2, float y2, float z2) {
    return std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));
};

// checks to make sure the machine can actually go there
bool Machine::check_bounds() {
    return (new_x <= bounds.x_max && new_x >= bounds.x_min && new_y <= bounds.y_max && new_y >= bounds.y_min && new_z <= bounds.z_max && new_z >=bounds.z_min &&  f <= bounds.f_max && s <= bounds.s_max);      
};