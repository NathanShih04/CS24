#ifndef VOXMAP_H
#define VOXMAP_H

#include <istream>
#include <vector>
#include "Point.h"
#include "Route.h"
#include "Errors.h"

class VoxMap {
  std::vector<std::vector<std::vector<bool>>> voxels;
  int width;
  int height;
  int depth;

  bool is_valid_point(const Point& point) const;
  bool is_valid_voxel(const Point& point) const;

public:
  VoxMap(std::istream& stream);
  ~VoxMap();

  Route route(Point src, Point dst);
};

#endif
