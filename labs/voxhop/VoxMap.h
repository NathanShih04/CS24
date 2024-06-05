#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "Point.h"
#include "Route.h"

class VoxMap {
  struct Voxel {
    bool is_full;
    bool is_visited;
  };

  int width, depth, height;
  std::vector<std::vector<std::vector<Voxel>>> map;

  bool isValidVoxel(const Point& p) const;
  bool isStandable(const Point& p) const;
  std::vector<Point> getNeighbors(const Point& p) const;
  int heuristic(const Point& a, const Point& b) const;

public:
  VoxMap(std::istream& stream);
  ~VoxMap() = default;

  Route route(Point src, Point dst);
};

#endif
