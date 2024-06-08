#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include "Point.h"
#include "Route.h"

struct Voxel {
    bool isFilled;
    bool isSurface;
};

class VoxMap {
    std::vector<std::vector<std::vector<Voxel>>> voxels;
    int width;
    int depth;
    int height;

    bool is_valid_point(const Point& point) const;
    bool is_valid_voxel(const Point& point) const;

public:
    VoxMap(std::istream& stream);
    ~VoxMap();

    Route route(Point src, Point dst);
};

#endif
