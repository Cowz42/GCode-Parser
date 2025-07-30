// for calculating the steps for any given instruction


#include "generatestep.h"
#include "gcode.h"
#include "machine.h"
#include <cmath>
#include "plane.h"
using namespace std;

#define CURVE_STEPS 50
#define LINEAR_STEPS 5
#define PI 3.1415926535897932



StepGen::StepGen(GCodeInstruction* instruction, Machine* machine_in) {
    machine = machine_in;
    command = instruction;

    steps = -1;
    if (command->command_char == 'G') {
        int num = command->command_number;
        if (num == 00) {
            mode = RAPID;
        } else if (num == 01) {
            math_values = (double*)malloc(sizeof(double) * 3);
            mode = LINEAR;
        } else if (num == 02) {
            math_values = (double*)malloc(sizeof(double) * 4);
            mode = CIRCCLOCK;
        } else if (num == 03) {
            math_values = (double*)malloc(sizeof(double) * 4);
            mode = CIRCCOUNT;
        } else if (num == 28) {
            mode = HOME;
        }
    }
    if (command->_f) {
        machine->set_f(command->f);
    }
    if (command->_s) {
        machine->set_s(command->s);
    }
};

StepGen::~StepGen() {
    free(command);
    if (math_values != nullptr) {
        free(math_values);
    }
};

double StepGen::angle(float x, float y) {
    return atan2(y, x);
};

double StepGen::dist(float x1, float y1, float x2, float y2) {
    return sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
};



// steps are returned as a greater than 0 meaning there is more steps to be calculated
// 0 is a finished instruction, free the memory
// -1 is the internal starting flag, probably shouldn't be output, but not an immdiate terminate
// anything else is an error
// error codes: -1, starting, flag for later, might get it, -2, missing values from instruction, -3 missing pointers for control, -4, no valid instruction
// handles translation and being nice so you just need to call next() to get you next step data
int StepGen::next(float* x_pre, float* y_pre, float* z_pre) {
    // checks to make sure that it can actually call itself and write values
    if (x_pre == nullptr || y_pre == nullptr || z_pre == nullptr) {
        return -3;
    }
    // starting steps for general purposes
    if (steps == -1) {
        // checks to make sure it has enough information in it's instruction
        if (machine->get_plane() == XY) {
            char circ[4] = { 'X', 'Y', 'I', 'J' };
            if ((mode == CIRCCLOCK || mode == CIRCCOUNT) && !command->check_values(circ, 4)) {
                return -2;
            } else if (command->check_values()) {
                return -2;
            }
        } else if (machine->get_plane() == YZ) {
            char circ[4] = { 'Z', 'Y', 'I', 'J' };
            if ((mode == CIRCCLOCK || mode == CIRCCOUNT) && !command->check_values(circ, 4)) {
                return -2;
            } else if (command->check_values()) {
                return -2;
            }
        } else if (machine->get_plane() == XZ) {
            char circ[4] = { 'X', 'Z', 'I', 'J' };
            if ((mode == CIRCCLOCK || mode == CIRCCOUNT) && !command->check_values(circ, 4)) {
                return -2;
            } else if (command->check_values()) {
                return -2;
            }
        }
    }

    // if the command doesn't write to a value, the pointer sent to the other functions is an unused internal pointer to just be somewhere to write to
    if (!command->_x) {
        x_pre = junk;
    }
    if (!command->_y) {
        y_pre = junk;
    }
    if (!command->_z) {
        z_pre = junk;
    }

    // translates the pointers based off of the current working plane
    // very annoying
    float* x; float* y; float* z;
    if (machine->get_plane() == XY) {
        x = x_pre;
        y = y_pre;
        z = z_pre;
    } else if (machine->get_plane() == YZ) {
        x = y_pre;
        y = z_pre;
        z = x_pre;
    } else if (machine->get_plane() == XZ) {
        x = x_pre;
        y = z_pre;
        z = y_pre;
    }

    // executes the correct helper function for the given command, might make this a switch later
    if (mode == CIRCCLOCK) {
        return next_circclock(x, y, z);
    } else if (mode == CIRCCOUNT) {
        return next_circcount(x, y, z);
    } else if (mode == RAPID) {
        return next_rapid(x, y, z);
    } else if (mode == HOME) {
        return next_home(x, y, z);
    } else if (mode == LINEAR) {
        return next_linear(x, y, z);
    }
    return -4;
};

// memory allocation for commands, circ interpolation, [start_angle, end_angle, radius, current_angle, step_angle]

// generates points along a circle clockwise
int StepGen::next_circclock(float* x, float* y, float* z) {
    // calculates the starting and ending angles, and adds them till they make for easy math to follow
    // with radians
    if (steps == - 1) {
        // sets angles
        math_values[0] = angle(machine->get_x() - command->i, machine->get_y() - command->j);
        math_values[1] = angle(command->x - command->i, command->y - command->j);
        // setting a consistent offset, always between 0 and 2pi away
        if (math_values[0] < 0) {
            math_values[0] += 2 * PI;
        }
        if (math_values[1] < 0) {
            math_values[1] += 2 * PI;
        }
        if (math_values[1] < math_values[0]) {
            math_values[1] += 2 * PI;
        }
        // starts it at the starting angle
        math_values[3] = math_values[0];
        // calculates the radians with which the angle should change by, adjust macro for finer or softer edges
        math_values[4] = (math_values[1] - math_values[0]) / CURVE_STEPS;
        // calculates the radius for de normalizing
        math_values[2] = dist(command->x, command->y, machine->get_x(), machine->get_y());
        steps = CURVE_STEPS;
    }
    // moves the current angle
    math_values[3] += math_values[4];

    // checks if the angle is over or not
    // if so just sets it to the end and stops counting steps
    if (math_values[3] > math_values[1]) {
        *x = command->x;
        *y = command->y;
        return 0;
    }
    // otherwise, calculates the cos and sine and multiplies by radius, to get the x and y of the current point on the circle
    *x = command->i + cos(math_values[3]) * math_values[2];
    *y = command->j + sin(math_values[3]) * math_values[2];

    return steps;
};

// generates points along a circle counterclockwise
int StepGen::next_circcount(float* x, float* y, float* z) {
    // calculates the starting and ending angles, and adds them till they make for easy math to follow
    // with radians
    if (steps == - 1) {
        // sets angles
        math_values[0] = angle(machine->get_x() - command->i, machine->get_y() - command->j);
        math_values[1] = angle(command->x - command->i, command->y - command->j);
        // setting a consistent offset, always between 0 and 2pi away
        if (math_values[0] < 0) {
            math_values[0] += 2 * PI;
        }
        if (math_values[1] < 0) {
            math_values[1] += 2 * PI;
        }
        if (math_values[0] < math_values[1]) {
            math_values[0] += 2 * PI;
        }
        // starts it at the starting angle
        math_values[3] = math_values[0];
        // calculates the radians with which the angle should change by, adjust macro for finer or softer edges
        math_values[4] = (math_values[1] - math_values[0]) / CURVE_STEPS;
        // calculates the radius for de normalizing
        math_values[2] = dist(command->x, command->y, machine->get_x(), machine->get_y());
        steps = CURVE_STEPS;
    }
    // moves the current angle
    math_values[3] += math_values[4];

    // checks if the angle is over or not
    // if so just sets it to the end and stops counting steps
    if (math_values[3] > math_values[1]) {
        *x = command->x;
        *y = command->y;
        return 0;
    }
    // otherwise, calculates the cos and sine and multiplies by radius, to get the x and y of the current point on the circle
    *x = command->i + cos(math_values[3]) * math_values[2];
    *y = command->j + sin(math_values[3]) * math_values[2];

    return steps;
};

// tells it to go immediatly too the given point, only generates one instruction, that being the point
int StepGen::next_rapid(float* x, float* y, float* z) {
    *x = command->x;
    *y = command->y;
    *z = command->z;
    return 0;
};

// rapid passes through a point, and then goes to home
int StepGen::next_home(float* x, float* y, float* z) {
    // calls next_rapid move until it makes it to that point, then just tells it to go to 0, 0, 0
    if (steps == -1) {
        if (machine->get_x() == command->x && machine->get_y() == command->y && machine->get_z() == command->z) {
            steps = 0;
        }
        return next_rapid(x, y, z) + 1;
    }
    *x = 0;
    *y = 0;
    *z = 0;
    return 0;
};

// memory [increment x, increment y, increment z]

// calculates intermediary steps for the machine to follow
int StepGen::next_linear(float* x, float* y, float* z) {
    // sets up increments
    if (steps == -1) {
        steps = 1;
        math_values[0] = (command->x - machine->get_x()) / LINEAR_STEPS;
        math_values[0] = (command->y - machine->get_y()) / LINEAR_STEPS;
        math_values[2] = (command->z - machine->get_z()) / LINEAR_STEPS;
    }
    // adds increments continuously till the end when it hits the command point
    *x = *x + math_values[0];
    *y = *y + math_values[1];
    *z = *z + math_values[2];
    if (*x == command->x && *y == command->y && *z == command->z) {
        steps = 0;
    }
    return steps;
};