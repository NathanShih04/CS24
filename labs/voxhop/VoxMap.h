#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <memory>
#include "Point.h"
#include "Route.h"
#include <unordered_map>
#include <queue>

struct Voxel {
    bool isFilled;
    bool isSurface;
};

class VoxelMap {
public:
    VoxelMap(int width, int depth, int height);
    
    Voxel& getVoxel(int x, int y, int z);
    const Voxel& getVoxel(int x, int y, int z) const;
    
    bool isValid(int x, int y, int z) const;
    void markSurface();

    int width, depth, height;  // Make these public for access

private:
    std::vector<Voxel> voxels;
};

class VoxMap {
public:
    VoxMap(std::istream& stream);
    ~VoxMap();

    Route route(Point src, Point dst);

private:
    std::unique_ptr<VoxelMap> map;
    bool isValidPoint(const Point& p) const;
};

#endif
