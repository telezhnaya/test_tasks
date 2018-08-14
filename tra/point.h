#include <iostream>
#include <fstream>
#include <math.h>

struct Point {
    double x, y;

    Point(double x, double y) : x(x), y(y) {}

    Point& operator=(Point const &a) {
        x = a.x;
        y = a.y;
        return *this;
    }

    Point& operator+=(Point const &a) {
        x += a.x;
        y += a.y;
        return *this;
    }

    Point& operator*=(double a) {
        x *= a;
        y *= a;
        return *this;
    }

    Point& operator/=(double a) {
        x /= a;
        y /= a;
        return *this;
    }

    Point& operator-=(Point const &a) {
	    x -= a.x;
	    y -= a.y;
        return *this;
    }

    double getDist() const {
        return sqrt(x * x + y * y);
    }

    void rotate(double alpha) {
        *this = Point(x * cos(alpha) - y * sin(alpha), x * sin(alpha) + y * cos(alpha));
    }
};

Point operator+(Point a, Point const &b) {
    return a += b;
}

Point operator-(Point a, Point const &b) {
    return a -= b;
}

double operator*(Point const &a, Point const &b) {
    return a.x * b.x + a.y * b.y;
}

Point operator*(Point a, double const &b) {
    return a *= b;
}

Point operator/(Point a, double b) {
    return a /= b;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << "\t" << p.y;
    return os;
}
