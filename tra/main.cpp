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

    void move(const Point& b, double phi) {
        x = (x - b.x) * cos(phi) + (y - b.y) * sin(phi) + b.x;
        y = (y - b.y) * cos(phi) - (x - b.x) * sin(phi) + b.y;
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

bool eq(double a, double b) {
    return abs(a - b) < __DBL_EPSILON__;
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
        cout << "_ " << p1 << "\t" << p2 << "\t" << alpha << endl;
        cout << "@ " << p << "\t" << alpha << endl;
        cout << p1.get_dist(p2) << "\t" << b << endl;
        assert(eq(p1.get_dist(p2), b));
        double t, th1, th2, d_th1, d_th2;
        src_file >> t >> th1 >> th2;
        d_th1 = th1 - old_th1;
        d_th2 = th2 - old_th2;
        if (eq(d_th1, 0) && eq(d_th2, 0))
            continue;

        if (d_th1 > d_th2) {
            cout << "swapped\n";
            swap(th1, th2);
            swap(d_th1, d_th2);
            swap(p1, p2);
            old_xx = -old_xx;
            old_yy = -old_yy;
        }

        double phi = (d_th2 - d_th1) / b;
        if (eq(phi, 0)) {
            p1.move(d_th1, alpha);
            p2.move(d_th2, alpha);
            p.x = (p1.x + p2.x) / 2;
            p.y = (p1.y + p2.y) / 2;
            res_file << p << "\t" << alpha << "\n";
            old_th1 = th1;
            old_th2 = th2;
            continue;
        }

        double cur_r = d_th1 / phi;
        double lambda = b / cur_r;
        Point p0((p1.x * (1 + lambda) - p2.x) / lambda, (p1.y * (1 + lambda) - p2.y) / lambda);
        cout << "* " << p0 << "\t" << phi << "\t" << cur_r << "\t" << lambda << endl;
        //if (isnan(phi) || isnan(cur_r) || isnan(lambda)) continue;
        p1.move(p0, phi);
        p2.move(p0, phi);

        p.x = (p1.x + p2.x) / 2;
        p.y = (p1.y + p2.y) / 2;

        double xx = p2.x - p1.x;
        double yy = p2.y - p1.y;
        alpha -= acos((xx * old_xx + yy * old_yy) / (get_dist(xx, yy) * get_dist(old_xx, old_yy)));
        res_file << p << "\t" << alpha << "\n";
        old_th1 = th1;
        old_th2 = th2;
        old_xx = xx;
        old_yy = yy;
        if (t > 0.1) break;
    }
    src_file.close();
    res_file.close();

    cout << p << "\t" << alpha << endl;
    return 0;
}
