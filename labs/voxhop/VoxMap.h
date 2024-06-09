#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "Point.h"
#include "Route.h"

class VoxMap {
    int width, depth, height;
    std::vector<std::vector<std::vector<bool>>> voxels;

    bool isValid(const Point& p) const;
    bool isWalkable(const Point& p) const;

    std::vector<Point> getNeighbors(const Point& p) const;
    int heuristic(const Point& a, const Point& b) const;

public:
    VoxMap(std::istream& stream);
    ~VoxMap() = default;

    Route route(Point src, Point dst);
};

#endif
