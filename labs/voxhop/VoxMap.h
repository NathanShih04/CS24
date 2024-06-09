#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <functional>

#include "Point.h"
#include "Route.h"

class VoxMap {
  std::vector<std::vector<std::vector<bool>>> map;
  int width, depth, height;

  bool isValidVoxel(const Point& p) const;
  bool isEmpty(const Point& p) const;
  bool hasVoxelBelow(const Point& p) const;

public:
  VoxMap(std::istream& stream);
  ~VoxMap();

  Route route(Point src, Point dst);
};

#endif
