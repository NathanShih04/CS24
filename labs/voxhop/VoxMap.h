#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

#include "Point.h"
#include "Route.h"
#include "Errors.h"

class VoxMap {
  // Member Variables
  int width, depth, height;
  std::vector<std::vector<std::vector<bool>>> voxels;

  // Helper Functions
  bool isValidPoint(const Point& p) const;
  bool isValidVoxel(const Point& p) const;
  std::vector<Point> getNeighbors(const Point& p) const;
  int heuristic(const Point& a, const Point& b) const;
  int index(int x, int y, int z) const;

public:
  VoxMap(std::istream& stream);
  ~VoxMap();

  Route route(Point src, Point dst);
};

#endif
