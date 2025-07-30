// for calculating the steps for any given instruction



#ifndef generatestep_h
#define generatestep_h

#include "gcode.h"
#include "machine.h"

class StepGen {
private:
    enum Mode {
        LINEAR,
        CIRCCOUNT,
        CIRCCLOCK,
        HOME,
        RAPID
    };
    Mode mode;
    GCodeInstruction* command;
    double* math_values = nullptr;
    int steps;
    // generates points along a circle clockwise
    int next_circclock(float* x, float* y, float* z);
    // generates points along a circle counterclockwise
    int next_circcount(float* x, float* y, float* z);
    // tells it to go immediatly too the given point, only generates one instruction, that being the point
    int next_rapid(float* x, float* y, float* z);
    // calculates intermediary steps for the machine to follow
    int next_linear(float* x, float* y, float* z);
    // rapid passes through a point, and then goes to home
    int next_home(float* x, float* y, float* z);
    Machine* machine;
    double angle(float x, float y);
    float* junk;
    double dist(float x1, float y1, float x2, float y2);

public:
    StepGen(GCodeInstruction* instruction, Machine* machine_in);
    ~StepGen();
    // steps are returned as a greater than 0 meaning there is more steps to be calculated
    // 0 is a finished instruction, free the memory
    // -1 is the internal starting flag, probably shouldn't be output, but not an immdiate terminate
    // anything else is an error
    // error codes: -1, starting, flag for later, might get it, -2, missing values from instruction, -3 missing pointers for control, -4, no valid instruction
    // handles translation and being nice so you just need to call next() to get you next step data
    int next(float* x, float* y, float* z);

};



#endif