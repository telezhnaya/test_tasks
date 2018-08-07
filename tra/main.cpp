#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <assert.h>
using namespace std;

void update_coords(double dist, double alpha, double& x, double& y) {
    x = x + dist * cos(alpha);
    y = y + dist * sin(alpha);
}

double get_dist(double d_x, double d_y) {
    return sqrt(d_x * d_x + d_y * d_y);
}

void fix_coords(double dist, double alpha, double& x1, double& y1, double& x2, double& y2) {
    double fix = (get_dist(x2 - x1, y2 - y1) - dist) / 2;
    update_coords(fix, alpha - M_PI_2, x1, y1);
    update_coords(fix, alpha + M_PI_2, x2, y2);
}

int main(int argc, char const *argv[])
{
    double x = 0, y = 0, alpha = 0;
    ifstream src_file(argv[1]);
    ofstream res_file;
    // I put lines "x y" here to make a plot with a path of a car
    res_file.open("res.txt");

    if (!src_file.is_open())
        return -1;
    
    double b, r;
    src_file >> b >> r;
    double x1 = 0, y1 = b / 2;
    double x2 = 0, y2 = -b / 2;

    double old_th1 = 0, old_th2 = 0;
    double old_xx = 0, old_yy = -b;

    while (!src_file.eof()) {
        double t, th1, th2;
        src_file >> t >> th1 >> th2;
        // It's easier to work with deltas of theta rather than theta itself.
        // Current distance is delta (in radians) * radius.
        update_coords((th1 - old_th1) * r, alpha, x1, y1);
        update_coords((th2 - old_th2) * r, alpha, x2, y2);

        x = (x1 + x2) / 2;
        y = (y1 + y2) / 2;

        double xx = x2 - x1;
        double yy = y2 - y1;
        alpha -= acos((xx * old_xx + yy * old_yy) / (get_dist(xx, yy) * get_dist(old_xx, old_yy)));
        // Our wheels will move away from each other,
        // so we need to make a small correction.
        // I put it closer so that distance between them remains the same.
        fix_coords(b, alpha, x1, y1, x2, y2);

        res_file << x << "\t" << y << "\t" << alpha << "\n";
        old_th1 = th1;
        old_th2 = th2;
        old_xx = xx;
        old_yy = yy;
    }
    src_file.close();
    res_file.close();

    cout << x << "\t" << y << "\t" << alpha << endl;
    return 0;
}
