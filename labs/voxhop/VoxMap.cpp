
#include "VoxMap.h"
#include <stdexcept>
#include <sstream>

VoxMap::VoxMap(std::istream& stream) {
  // Read the map dimensions from the input stream
  stream >> width >> height >> depth;

  // Initialize the voxel map with the given dimensions
  voxels.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width, false)));

  // Read the voxel data from the input stream
  for (int h = 0; h < height; ++h) {
    std::string line;
    for (int d = 0; d < depth; ++d) {
      stream >> line;
      for (int w = 0; w < width / 4; ++w) {
        char hex_char = line[w];
        int value = std::stoi(std::string(1, hex_char), nullptr, 16);
        for (int bit = 0; bit < 4; ++bit) {
          voxels[h][d][w * 4 + bit] = (value & (1 << (3 - bit))) != 0;
        }
      }
    }
  }
}

VoxMap::~VoxMap() {
  // Destructor
}

bool VoxMap::is_valid_point(const Point& point) const {
  return point.x >= 0 && point.x < width &&
         point.y >= 0 && point.y < height &&
         point.z >= 0 && point.z < depth;
}

bool VoxMap::is_valid_voxel(const Point& point) const {
  if (!is_valid_point(point)) return false;
  if (!voxels[point.y][point.z][point.x]) return false; // Must be empty
  if (point.y == 0 || !voxels[point.y - 1][point.z][point.x]) return false; // Must have a full voxel below
  return true;
}

Route VoxMap::route(Point src, Point dst) {
  if (!is_valid_voxel(src)) {
    throw InvalidPoint(src);
  }
  if (!is_valid_voxel(dst)) {
    throw InvalidPoint(dst);
  }

  // Placeholder for the pathfinding algorithm
  // Here we just simulate that no route is found
  throw NoRoute(src, dst);

  // In a complete implementation, the algorithm would return a valid Route
  // if one exists between the source and destination points.
}

