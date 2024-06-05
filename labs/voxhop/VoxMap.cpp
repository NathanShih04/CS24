#include "VoxMap.h"
#include "Errors.h"
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>

VoxMap::VoxMap(std::istream& stream) {
  if (!(stream >> width >> depth >> height)) {
    throw std::runtime_error("Failed to read map dimensions.");
  }

  map.resize(height, std::vector<std::vector<Voxel>>(depth, std::vector<Voxel>(width)));

  std::string line;
  for (int z = 0; z < height; ++z) {
    if (!std::getline(stream, line)) {
      throw std::runtime_error("Unexpected end of file while reading map data.");
    }

    for (int y = 0; y < depth; ++y) {
      if (!std::getline(stream, line) || line.size() != static_cast<size_t>(width / 4)) {
        throw std::runtime_error("Invalid map data format at depth " + std::to_string(y) + ", height " + std::to_string(z) + ".");
      }

      for (int x = 0; x < width / 4; ++x) {
        char hex_char = line[x];
        int value;
        try {
          value = std::stoi(std::string(1, hex_char), nullptr, 16);
        } catch (const std::invalid_argument& e) {
          throw std::runtime_error("Invalid hex character '" + std::string(1, hex_char) + "' in map data at depth " + std::to_string(y) + ", height " + std::to_string(z) + ".");
        }

        for (int i = 0; i < 4; ++i) {
          map[z][y][4 * x + (3 - i)].is_full = (value & (1 << i)) != 0;
          map[z][y][4 * x + (3 - i)].is_visited = false;
        }
      }
    }
  }
}

bool VoxMap::isValidVoxel(const Point& p) const {
  return p.x >= 0 && p.x < width && p.y >= 0 && p.y < depth && p.z >= 0 && p.z < height;
}

bool VoxMap::isStandable(const Point& p) const {
  return isValidVoxel(p) && !map[p.z][p.y][p.x].is_full && p.z > 0 && map[p.z - 1][p.y][p.x].is_full;
}

std::vector<Point> VoxMap::getNeighbors(const Point& p) const {
  std::vector<Point> neighbors;
  const std::vector<Point> directions = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}};

  for (const auto& dir : directions) {
    Point next = {p.x + dir.x, p.y + dir.y, p.z};
    if (isValidVoxel(next) && !map[next.z][next.y][next.x].is_full) {
      // handle falling down
      while (next.z > 0 && !map[next.z - 1][next.y][next.x].is_full) {
        next.z--;
      }
      // handle jumping up
      if (next.z + 1 < height && !map[next.z + 1][next.y][next.x].is_full && map[next.z][next.y][next.x].is_full) {
        next.z++;
      }
      neighbors.push_back(next);
    }
  }
  return neighbors;
}

int VoxMap::heuristic(const Point& a, const Point& b) const {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst) {
  if (!isStandable(src)) throw InvalidPoint(src);
  if (!isStandable(dst)) throw InvalidPoint(dst);

  using PQElement = std::pair<int, Point>;
  std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> open_set;
  std::unordered_map<Point, Point, Point::HashFunction> came_from;
  std::unordered_map<Point, int, Point::HashFunction> cost_so_far;

  open_set.emplace(0, src);
  cost_so_far[src] = 0;

  while (!open_set.empty()) {
    Point current = open_set.top().second;
    open_set.pop();

    if (current == dst) {
      Route path;
      while (current != src) {
        Point from = came_from[current];
        if (from.x < current.x) path.push_back(Move::EAST);
        else if (from.x > current.x) path.push_back(Move::WEST);
        else if (from.y < current.y) path.push_back(Move::SOUTH);
        else if (from.y > current.y) path.push_back(Move::NORTH);
        current = from;
      }
      std::reverse(path.begin(), path.end());
      return path;
    }

    for (const auto& next : getNeighbors(current)) {
      int new_cost = cost_so_far[current] + 1;
      if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
        cost_so_far[next] = new_cost;
        int priority = new_cost + heuristic(next, dst);
        open_set.emplace(priority, next);
        came_from[next] = current;
      }
    }
  }
  throw NoRoute(src, dst);
}
