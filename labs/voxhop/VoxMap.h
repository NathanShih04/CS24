#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "Point.h"
#include "Route.h"

class VoxMap {
  int width, depth, height;
  std::vector<std::vector<std::vector<bool>>> voxels;

  bool isValidPoint(const Point& point) const;
  bool isWalkable(const Point& point, bool& invalid) const;
  int heuristic(const Point& a, const Point& b) const;

public:
  VoxMap(std::istream& stream);
  ~VoxMap() = default;

  Route route(Point src, Point dst);
};

#endif
