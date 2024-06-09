#include "VoxMap.h"
#include "Errors.h"

int VoxMap::index(int x, int y, int z) const {
  return (z * depth * width) + (y * width) + x;
}

VoxMap::VoxMap(std::istream& stream) {
    stream >> width >> depth >> height;

    voxels.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width)));

    for (int z = 0; z < height; ++z) {
        std::string line;
        for (int y = 0; y < depth; ++y) {
            stream >> line;
            for (int x = 0; x < width / 4; ++x) {
                char hex = line[x];
                int value = (hex >= '0' && hex <= '9') ? hex - '0' : hex - 'A' + 10;
                for (int i = 0; i < 4; ++i) {
                    voxels[z][y][x * 4 + 3 - i] = (value & (1 << i)) != 0;
                }
            }
        }
    }
}

VoxMap::~VoxMap() {}

bool VoxMap::isValidPoint(const Point& p) const {
  return p.x >= 0 && p.x < width && p.y >= 0 && p.y < depth && p.z >= 1 && p.z < height;
}

bool VoxMap::isValidVoxel(const Point& p) const {
  if (!isValidPoint(p) || !isValidPoint(Point(p.x, p.y, p.z - 1))) return false;
  return !voxels[p.z][p.y][p.x] && voxels[p.z - 1][p.y][p.x];
}

std::vector<Point> VoxMap::getNeighbors(const Point& p) const {
  static const std::vector<Point> directions{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}};
  std::vector<Point> neighbors;

  for (const auto& dir : directions) {
    Point next = {p.x + dir.x, p.y + dir.y, p.z};
    int fallHeight = 0;

    // Check for falling
    while (isValidPoint(next) && !voxels[next.z][next.y][next.x]) {
      next.z--;
      fallHeight++;
      if (fallHeight > 1) break; // Ensure fall height is not greater than 1
    }
    next.z++;
    if (fallHeight <= 1 && isValidVoxel(next)) {
      neighbors.push_back(next);
    }

    // Check for jumping
    next = {p.x + dir.x, p.y + dir.y, p.z};
    if (isValidPoint(Point(next.x, next.y, next.z + 1)) &&
        !voxels[next.z + 1][next.y][next.x] && // No voxel directly above the current voxel
        isValidPoint(Point(next.x, next.y, next.z + 2)) &&
        !voxels[next.z + 2][next.y][next.x]) { // No voxel directly above the destination voxel
      next.z++;
      if (isValidVoxel(next)) {
        neighbors.push_back(next);
      }
    }
  }
  return neighbors;
}

int VoxMap::heuristic(const Point& a, const Point& b) const {
  return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst) {
  if (!isValidVoxel(src)) throw InvalidPoint(src);
  if (!isValidVoxel(dst)) throw InvalidPoint(dst);

  std::unordered_map<Point, Point, PointHash> cameFrom;
  std::unordered_map<Point, int, PointHash> gScore;
  std::unordered_map<Point, int, PointHash> fScore;

  auto cmp = [&fScore](const Point& a, const Point& b) {
    return fScore[a] > fScore[b];
  };
  std::priority_queue<Point, std::vector<Point>, decltype(cmp)> openSet(cmp);

  gScore[src] = 0;
  fScore[src] = heuristic(src, dst);
  openSet.push(src);

  while (!openSet.empty()) {
    Point current = openSet.top();
    openSet.pop();

    if (current == dst) {
      Route route;
      while (current != src) {
        Point prev = cameFrom[current];
        if (current.x > prev.x) route.push_back(Move::EAST);
        else if (current.x < prev.x) route.push_back(Move::WEST);
        else if (current.y > prev.y) route.push_back(Move::SOUTH);
        else if (current.y < prev.y) route.push_back(Move::NORTH);
        current = prev;
      }
      std::reverse(route.begin(), route.end());
      return route;
    }

    for (const Point& neighbor : getNeighbors(current)) {
      int tentative_gScore = gScore[current] + 1;
      if (tentative_gScore < gScore[neighbor] || gScore.find(neighbor) == gScore.end()) {
        cameFrom[neighbor] = current;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, dst);
        openSet.push(neighbor);
      }
    }
  }

  throw NoRoute(src, dst);
}
