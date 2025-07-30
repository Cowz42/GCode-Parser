#include "machine.h"




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