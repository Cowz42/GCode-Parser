// for calculating the steps for any given instruction


#include "generatestep.h"
#include "gcode.h"
#include "machine.h"
#include <cmath>
using namespace std;
#define PI 3.1415926535897932

enum Plane {
    XY,
    YZ,
    XZ
};

StepGen::StepGen(GCodeInstruction* instruction, Machine* machine_in) {
    machine = machine_in;
    command = instruction;
    steps = -1;
    if (command->command_char == 'G') {
        int num = command->command_number;
        if (num == 00) {
            mode = RAPID;
        } else if (num == 01) {
            mode = LINEAR;
        } else if (num == 02) {
            math_values = (float*)malloc(sizeof(double) * 4);
            mode = CIRCCLOCK;
        } else if (num == 03) {
            math_values = (float*)malloc(sizeof(double) * 4);
            mode = CIRCCOUNT;
        } else if (num == 28) {
            math_values = (float*)malloc(sizeof(double) * 3);
            mode = HOME;
        }
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


// error codes: -1, no valid instruction, -2, missing values from instruction
int StepGen::next(float* x_pre, float* y_pre, float* z_pre) {
    if (x_pre == nullptr || y_pre == nullptr || z_pre == nullptr) {
        return -3;
    }
    if (steps == -1) {
        if (machine->get_plane() == XY) {
            char circ[4] = { 'X', 'Y', 'I', 'J' };
            char lin[3] = { 'X', 'Y', 'Z' };
            if ((mode == CIRCCLOCK || mode == CIRCCOUNT) && !command->check_values(circ, 4)) {
                return -2;
            } else if ((mode == LINEAR || mode == RAPID) && !command->check_values(lin, 3)) {
                return -2;
            }
        } else if (machine->get_plane() == YZ) {
            char circ[4] = { 'Z', 'Y', 'I', 'J' };
            char lin[3] = { 'X', 'Y', 'Z' };
            if ((mode == CIRCCLOCK || mode == CIRCCOUNT) && !command->check_values(circ, 4)) {
                return -2;
            } else if ((mode == LINEAR || mode == RAPID) && !command->check_values(lin, 3)) {
                return -2;
            }
        } else if (machine->get_plane() == XZ) {
            char circ[4] = { 'X', 'Z', 'I', 'J' };
            char lin[3] = { 'X', 'Y', 'Z' };
            if ((mode == CIRCCLOCK || mode == CIRCCOUNT) && !command->check_values(circ, 4)) {
                return -2;
            } else if ((mode == LINEAR || mode == RAPID) && !command->check_values(lin, 3)) {
                return -2;
            }
        }
    }
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
    return -1;
};

// memory allocation for commands, circ interpolation, [start_angle, end_angle, radius, current_angle, step_angle]


int StepGen::next_circclock(float* x, float* y, float* z) {
    if (steps == - 1) {
        math_values[0] = angle(machine->get_x() - command->i, machine->get_y() - command->j);
        math_values[1] = angle(command->x - command->i, command->y - command->j);
        if (math_values[0] < 0) {
            math_values[0] += 2 * PI;
        }
        if (math_values[1] < 0) {
            math_values[1] += 2 * PI;
        }
        if (math_values[1] < math_values[0]) {
            math_values[1] += 2 * PI;
        }
        math_values[3] = math_values[0];
        math_values[4] = (math_values[1] - math_values[0]) / 100;
        math_values[2] = sqrt((command->x - command->i)*(command->x - command->i) + (command->y - command->j)*(command->y - command->j));
        steps = 100;
    }

    math_values[3] += math_values[4];
    *x = cos(math_values[3]) * math_values[2];
    *y = sin(math_values[3]) * math_values[2];

    steps--;
    return steps;
};

int StepGen::next_circcount(float* x, float* y, float* z) {
    if (steps == - 1) {
        math_values[0] = angle(machine->get_x() - command->i, machine->get_y() - command->j);
        math_values[1] = angle(command->x - command->i, command->y - command->j);
        if (math_values[0] < 0) {
            math_values[0] += 2 * PI;
        }
        if (math_values[1] < 0) {
            math_values[1] += 2 * PI;
        }
        if (math_values[0] < math_values[1]) {
            math_values[0] += 2 * PI;
        }
        math_values[3] = math_values[0];
        math_values[4] = (math_values[1] - math_values[0]) / 100;
        math_values[2] = sqrt((command->x - command->i)*(command->x - command->i) + (command->y - command->j)*(command->y - command->j));
        steps = 100;
    } else if (steps == 0) {
        return -1;
    }

    math_values[3] -= math_values[4];
    *x = cos(math_values[3]) * math_values[2];
    *y = sin(math_values[3]) * math_values[2];

    steps--;
    return steps;
};

int StepGen::next_rapid(float* x, float* y, float* z) {
    *x = command->x;
    *y = command->y;
    *z = command->z;
    return 0;
};

int StepGen::next_home(float* x, float* y, float* z) {
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
}