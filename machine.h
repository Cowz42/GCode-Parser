



#ifndef machine_h
#define machine_h

class Machine {
private:
    float x, y, z, f, s;
    enum Plane {
        XY,
        YZ,
        XZ,
    };
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
};




#endif