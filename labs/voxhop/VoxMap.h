#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <functional>
#include <cmath>
#include "Point.h"
#include "Route.h"
#include "Errors.h"

class VoxMap {
    std::vector<std::vector<std::vector<bool>>> map;
    int width, depth, height;

    bool isValidPoint(const Point& point) const;
    bool isWalkable(const Point& point) const;
    std::vector<Point> getNeighbors(const Point& point) const;

public:
    VoxMap(std::istream& stream);
    ~VoxMap();

    Route route(Point src, Point dst);
};

#endif
