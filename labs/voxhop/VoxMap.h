#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_set>
#include "Point.h"
#include "Route.h"

class VoxMap {
    int width;
    int depth;
    int height;
    std::vector<std::vector<std::vector<bool>>> map;

    bool isValidPoint(const Point& point) const;
    bool isNavigable(const Point& point) const;
    bool canJump(const Point& current, const Point& next) const;

public:
    VoxMap(std::istream& stream);
    ~VoxMap() = default;

    Route route(Point src, Point dst);
};

#endif
