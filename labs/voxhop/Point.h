#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <functional>

struct Point {
    int x;
    int y;
    int z;

    Point() : x(0), y(0), z(0) {}
    Point(int x, int y, int z) : x(x), y(y), z(z) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
};

namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(const Point& point) const {
            return hash<int>()(point.x) ^ hash<int>()(point.y) ^ hash<int>()(point.z);
        }
    };
}

std::istream& operator>>(std::istream& stream, Point& point);
std::ostream& operator<<(std::ostream& stream, const Point& point);

#endif
