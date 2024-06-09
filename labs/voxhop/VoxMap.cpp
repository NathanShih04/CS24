#include "VoxMap.h"
#include "Errors.h"
#include <algorithm>

VoxMap::VoxMap(std::istream& stream) {
  stream >> width >> depth >> height;
  map.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width)));

  std::string line;
  for (int h = 0; h < height; ++h) {
    std::getline(stream, line); // Skip empty line
    for (int d = 0; d < depth; ++d) {
      std::getline(stream, line);
      for (int w = 0; w < width / 4; ++w) {
        char hex = line[w];
        int val = std::stoi(std::string(1, hex), nullptr, 16);
        for (int b = 0; b < 4; ++b) {
          map[h][d][4 * w + b] = val & (1 << (3 - b));
        }
      }
    }
  }
}

VoxMap::~VoxMap() {}

bool VoxMap::isValidVoxel(const Point& p) const {
  return p.x >= 0 && p.x < width && p.y >= 0 && p.y < depth && p.z >= 0 && p.z < height;
}

bool VoxMap::isEmpty(const Point& p) const {
  return isValidVoxel(p) && !map[p.z][p.y][p.x];
}

bool VoxMap::hasVoxelBelow(const Point& p) const {
  return p.z > 0 && map[p.z - 1][p.y][p.x];
}

Route VoxMap::route(Point src, Point dst) {
  if (!isValidVoxel(src) || !isEmpty(src) || !hasVoxelBelow(src)) {
    throw InvalidPoint(src);
  }
  if (!isValidVoxel(dst) || !isEmpty(dst) || !hasVoxelBelow(dst)) {
    throw InvalidPoint(dst);
  }

  std::unordered_set<Point, PointHash> closedSet;
  std::unordered_map<Point, Point, PointHash> cameFrom;
  std::unordered_map<Point, int, PointHash> gScore;
  std::unordered_map<Point, int, PointHash> fScore;

  auto heuristic = [](const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
  };

  auto cmp = [&fScore](const Point& a, const Point& b) {
    return fScore[a] > fScore[b];
  };

  std::priority_queue<Point, std::vector<Point>, decltype(cmp)> openSet(cmp);
  openSet.push(src);
  gScore[src] = 0;
  fScore[src] = heuristic(src, dst);

  const std::vector<Point> directions = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}};

  while (!openSet.empty()) {
    Point current = openSet.top();
    openSet.pop();

    if (current == dst) {
      Route path;
      while (current != src) {
        Point prev = cameFrom[current];
        if (current.x > prev.x) path.push_back(Move::EAST);
        else if (current.x < prev.x) path.push_back(Move::WEST);
        else if (current.y > prev.y) path.push_back(Move::SOUTH);
        else if (current.y < prev.y) path.push_back(Move::NORTH);
        current = prev;
      }
      std::reverse(path.begin(), path.end());
      return path;
    }

    closedSet.insert(current);

    for (const Point& dir : directions) {
      Point neighbor = {current.x + dir.x, current.y + dir.y, current.z};

      while (isValidVoxel(neighbor) && isEmpty(neighbor) && neighbor.z > 0 && !map[neighbor.z - 1][neighbor.y][neighbor.x]) {
        neighbor.z--;
      }

      if (!isValidVoxel(neighbor) || !isEmpty(neighbor) || !hasVoxelBelow(neighbor)) {
        continue;
      }

      if (closedSet.find(neighbor) != closedSet.end()) {
        continue;
      }

      int tentative_gScore = gScore[current] + 1;

      if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
        cameFrom[neighbor] = current;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, dst);
        openSet.push(neighbor);
      }
    }
  }

  throw NoRoute(src, dst);
}
