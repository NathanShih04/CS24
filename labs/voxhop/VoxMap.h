#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include "Point.h"
#include "Route.h"

class VoxMap {
private:
    int width, depth, height;
    std::vector<std::vector<std::vector<bool>>> voxels;

    bool isValid(const Point& point) const;
    bool isWalkable(const Point& point) const;
    std::vector<Point> getNeighbors(const Point& point) const;

public:
    VoxMap(std::istream& stream);
    ~VoxMap() = default;

    Route route(Point src, Point dst);
};

#endif
