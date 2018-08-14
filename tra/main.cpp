#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <assert.h>
using namespace std;

struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}

    void move(double dist, double alpha) {
        x = x + dist * cos(alpha);
        y = y + dist * sin(alpha);
    }

    double get_dist(const Point& b) const {
        return sqrt(pow(x - b.x, 2) + pow(y - b.y, 2));
    }

    friend ostream& operator<<(ostream& os, const Point& p);
};

ostream& operator<<(ostream& os, const Point& p) {
    os << p.x << "\t" << p.y;
    return os;
}

double get_dist(double d_x, double d_y) {
    return sqrt(d_x * d_x + d_y * d_y);
}

void fix_coords(double dist, double alpha, Point& p1, Point& p2) {
    double fix = (p1.get_dist(p2) - dist) / 2;
    p1.move(fix, alpha - M_PI_2);
    p2.move(fix, alpha + M_PI_2);
}

int main(int argc, char const *argv[])
{
    double alpha = 0;
    Point p(0, 0);
    ifstream src_file(argv[1]);
    ofstream res_file;
    // I put lines "x y alpha" here to make a plot with a path of a car
    res_file.open("res.txt");

    if (!src_file.is_open())
        return -1;
    
    double b, r;
    src_file >> b >> r;
    Point p1(0, b / 2), p2(0, -b / 2);

    double old_th1 = 0, old_th2 = 0;
    double old_xx = 0, old_yy = -b;

    while (!src_file.eof()) {
        double t, th1, th2;
        src_file >> t >> th1 >> th2;
        // It's easier to work with deltas of theta rather than theta itself.
        // Current distance is delta (in radians) * radius.
        p1.move((th1 - old_th1) * r, alpha);
        p2.move((th2 - old_th2) * r, alpha);

        p.x = (p1.x + p2.x) / 2;
        p.y = (p1.y + p2.y) / 2;

        double xx = p2.x - p1.x;
        double yy = p2.y - p1.y;
        alpha -= acos((xx * old_xx + yy * old_yy) / (get_dist(xx, yy) * get_dist(old_xx, old_yy)));
        // Our wheels will move away from each other,
        // so we need to make a small correction.
        // I put it closer so that distance between them remains the same.
        fix_coords(b, alpha, p1, p2);

        res_file << p << "\t" << alpha << "\n";
        old_th1 = th1;
        old_th2 = th2;
        old_xx = xx;
        old_yy = yy;
    }
    src_file.close();
    res_file.close();

    cout << p << "\t" << alpha << endl;
    return 0;
}
