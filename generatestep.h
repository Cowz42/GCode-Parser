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
    float* math_values = nullptr;
    int steps;
    int next_circclock(float* x, float* y, float* z);
    int next_circcount(float* x, float* y, float* z);
    int next_rapid(float* x, float* y, float* z);
    int next_linear(float* x, float* y, float* z);
    int next_home(float* x, float* y, float* z);
    Machine* machine;
    double angle(float x, float y);

public:
    StepGen(GCodeInstruction* instruction, Machine* machine_in);
    ~StepGen();
    int next(float* x, float* y, float* z);

};



#endif