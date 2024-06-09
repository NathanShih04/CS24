#include "VoxMap.h"
#include "Errors.h"
#include <sstream>
#include <algorithm>

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

bool VoxMap::isValid(const Point& p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < depth && p.z >= 0 && p.z < height;
}

bool VoxMap::isWalkable(const Point& p) const {
    return isValid(p) && !voxels[p.z][p.y][p.x] && (p.z == 0 || voxels[p.z - 1][p.y][p.x]);
}

std::vector<Point> VoxMap::getNeighbors(const Point& p) const {
    std::vector<Point> neighbors;
    std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (auto& [dx, dy] : directions) {
        Point neighbor(p.x + dx, p.y + dy, p.z);
        if (isValid(neighbor)) {
            if (!voxels[neighbor.z][neighbor.y][neighbor.x]) {
                while (neighbor.z > 0 && !voxels[neighbor.z - 1][neighbor.y][neighbor.x]) {
                    --neighbor.z;
                }
                neighbors.push_back(neighbor);
            } else if (isValid({p.x + dx, p.y + dy, p.z + 1}) && !voxels[p.z + 1][p.y + dy][p.x + dx]) {
                neighbors.push_back({p.x + dx, p.y + dy, p.z + 1});
            }
        }
    }
    return neighbors;
}

int VoxMap::heuristic(const Point& a, const Point& b) const {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst) {
    if (!isWalkable(src)) {
        throw InvalidPoint(src);
    }
    if (!isWalkable(dst)) {
        throw InvalidPoint(dst);
    }

    std::unordered_map<Point, Point, PointHash> cameFrom;
    std::unordered_map<Point, int, PointHash> costSoFar;
    std::priority_queue<std::pair<int, Point>, std::vector<std::pair<int, Point>>, std::greater<>> frontier;

    frontier.emplace(0, src);
    cameFrom[src] = src;
    costSoFar[src] = 0;

    while (!frontier.empty()) {
        Point current = frontier.top().second;
        frontier.pop();

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
            int newCost = costSoFar[current] + 1;
            if (!costSoFar.count(neighbor) || newCost < costSoFar[neighbor]) {
                costSoFar[neighbor] = newCost;
                int priority = newCost + heuristic(neighbor, dst);
                frontier.emplace(priority, neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }

    throw NoRoute(src, dst);
}
