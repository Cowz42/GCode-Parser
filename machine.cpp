#include "machine.h"
#include "plane.h"



Machine::Machine() {
    plane = XY;
    absolute = true;
};

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