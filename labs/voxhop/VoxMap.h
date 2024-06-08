#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_map>
#include "Point.h"
#include "Route.h"

class VoxMap {
    int width;
    int depth;
    int height;
    std::vector<bool> map;

    bool isValidPoint(const Point& point) const;
    bool isNavigable(const Point& point) const;
    int index(int x, int y, int z) const;

public:
    VoxMap(std::istream& stream);
    ~VoxMap() = default;

    Route route(Point src, Point dst);
};

#endif
