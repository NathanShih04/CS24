#include "VoxMap.h"
#include "Errors.h"
#include <sstream>
#include <bitset>
#include <algorithm>

VoxMap::VoxMap(std::istream& stream) {
    stream >> width >> depth >> height;
    voxels.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width)));

    std::string line;
    for (int h = 0; h < height; ++h) {
        std::getline(stream, line); // skip empty line
        for (int d = 0; d < depth; ++d) {
            std::getline(stream, line);
            for (size_t w = 0; w < line.size(); ++w) {
                std::bitset<4> bits(std::stoi(std::string(1, line[w]), nullptr, 16));
                for (int b = 0; b < 4; ++b) {
                    voxels[h][d][w * 4 + b] = bits[3 - b];
                }
            }
        }
    }
}

bool VoxMap::isValid(const Point& point) const {
    return point.x >= 0 && point.x < width &&
           point.y >= 0 && point.y < depth &&
           point.z >= 0 && point.z < height;
}

bool VoxMap::isWalkable(const Point& point) const {
    if (!isValid(point) || voxels[point.z][point.y][point.x])
        return false;

    if (point.z == 0 || voxels[point.z - 1][point.y][point.x])
        return true;

    return false;
}

std::vector<Point> VoxMap::getNeighbors(const Point& point) const {
    static const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::vector<Point> neighbors;

    for (const auto& [dx, dy] : directions) {
        Point neighbor(point.x + dx, point.y + dy, point.z);

        if (isValid(neighbor) && isWalkable(neighbor)) {
            while (neighbor.z > 0 && !voxels[neighbor.z - 1][neighbor.y][neighbor.x]) {
                neighbor.z--;
            }
            if (isWalkable(neighbor)) {
                neighbors.push_back(neighbor);
            }
        }
        
        if (neighbor.z < height - 1 && !voxels[neighbor.z + 1][neighbor.y][neighbor.x]) {
            neighbor.z++;
            if (isWalkable(neighbor)) {
                neighbors.push_back(neighbor);
            }
        }
    }

    return neighbors;
}

Route VoxMap::route(Point src, Point dst) {
    if (!isWalkable(src)) throw InvalidPoint(src);
    if (!isWalkable(dst)) throw InvalidPoint(dst);

    std::unordered_map<Point, Point> cameFrom;
    std::unordered_map<Point, int> costSoFar;
    std::priority_queue<std::pair<int, Point>, std::vector<std::pair<int, Point>>, std::greater<>> frontier;

    auto heuristic = [](const Point& a, const Point& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
    };

    frontier.push({0, src});
    cameFrom[src] = src;
    costSoFar[src] = 0;

    while (!frontier.empty()) {
        Point current = frontier.top().second;
        frontier.pop();

        if (current == dst) {
            Route route;
            Point trace = dst;

            while (trace != src) {
                Point parent = cameFrom[trace];
                if (trace.x == parent.x + 1) route.push_back(Move::EAST);
                else if (trace.x == parent.x - 1) route.push_back(Move::WEST);
                else if (trace.y == parent.y + 1) route.push_back(Move::SOUTH);
                else if (trace.y == parent.y - 1) route.push_back(Move::NORTH);
                trace = parent;
            }

            std::reverse(route.begin(), route.end());
            return route;
        }

        for (const Point& next : getNeighbors(current)) {
            int newCost = costSoFar[current] + 1;
            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                int priority = newCost + heuristic(next, dst);
                frontier.push({priority, next});
                cameFrom[next] = current;
            }
        }
    }

    throw NoRoute(src, dst);
}
