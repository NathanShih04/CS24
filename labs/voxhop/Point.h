#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
  int x;
  int y;
  int z;

  Point() : x(0), y(0), z(0) {}
  Point(int x, int y, int z): x(x), y(y), z(z) {}

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  bool operator!=(const Point& other) const {
    return !(*this == other);
  }

  struct HashFunction {
    std::size_t operator()(const Point& p) const {
      return std::hash<int>()(p.x) ^ std::hash<int>()(p.y) ^ std::hash<int>()(p.z);
    }
  };
};

std::istream& operator >> (std::istream& stream, Point& point);
std::ostream& operator << (std::ostream& stream, const Point& point);

#endif
