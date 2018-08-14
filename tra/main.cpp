#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>

#include "point.h"

void rotate(Point& p, double alpha, const Point& center) {
    // go to origin, make a rotation, return back
    p -= center;
    p.rotate(alpha);
    p += center;
}

int main(int argc, char const *argv[]) {
    Point center(0, 0);

    std::ifstream src_file(argv[1]);
    std::ofstream res_file;

    res_file.open("res.txt");

    if (!src_file.is_open() || !res_file.is_open())
        return -1;
    
    double b, r;
    src_file >> b >> r;
    Point wheel1(0, b / 2), wheel2(0, -b / 2);

    double old_th1 = 0, old_th2 = 0, alpha = 0;

    while (!src_file.eof()) {
        double t, th1, th2;
        src_file >> t >> th1 >> th2;

        // length of a sector that wheel rolled
        double d1 = (th1 - old_th1) * r;
        double d2 = (th2 - old_th2) * r;
        // angle and radius of rotation
        double rotationAlpha = (d1 - d2) / b;
        double rotationR = d2 / rotationAlpha;

        Point diff = wheel1 - wheel2;
        Point moveVector(diff.y, -diff.x);
        // center point of the rotation
        Point rotationPoint = wheel2 - diff / diff.getDist() * rotationR;

        if (std::abs(rotationAlpha) > 1e-8) {
            rotate(wheel1, -rotationAlpha, rotationPoint);
            rotate(wheel2, -rotationAlpha, rotationPoint);
        } else {
            // we need to make parallel move
            moveVector *= d1 / moveVector.getDist();
            wheel1 += moveVector;
            wheel2 += moveVector;
        }

        center = (wheel1 + wheel2) / 2;
        alpha = atan2(moveVector.y, moveVector.x);
        res_file << center << "\t" << alpha << "\n";

        old_th1 = th1;
        old_th2 = th2;
    }

    std::cout << center << "\t" << alpha << "\n";
    return 0;
}
