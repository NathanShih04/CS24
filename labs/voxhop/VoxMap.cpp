#include "VoxMap.h"
#include "Errors.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>

VoxMap::VoxMap(std::istream &stream)
{
    stream >> width >> depth >> height;
    map.resize(width * depth * height);

    for (int z = 0; z < height; ++ z) {
        std::string line;
        for (int y = 0; y < depth; ++ y) {
            stream >> line;
            for (int x = 0; x < width / 4; ++ x) {
                char hexDigit = line[x];
                int value = (hexDigit >= '0' && hexDigit <= '9') ? hexDigit - '0' : hexDigit - 'A' + 10;
                for (int bit = 0; bit < 4; ++ bit) {
                    map[index(x * 4 + bit, y, z)] = (value & (8 >> bit)) != 0;
                }
            }
        }
    }
}

inline int VoxMap::index(int x, int y, int z) const {
    return z * width * depth + y * width + x;
}

inline bool VoxMap::isValidPoint(const Point &point) const {
    return point.x >= 0 && point.x < width &&
           point.y >= 0 && point.y < depth &&
           point.z >= 0 && point.z < height;
}

inline bool VoxMap::isNavigable(const Point &point) const {
    return isValidPoint(point) && !map[index(point.x, point.y, point.z)] &&
           (point.z > 0 && map[index(point.x, point.y, point.z - 1)]);
}

inline double heuristic(const Point &a, const Point &b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst) {
    if (!isNavigable(src))
        throw InvalidPoint(src);
    if (!isNavigable(dst))
        throw InvalidPoint(dst);

    using PQElement = std::pair<double, Point>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> toExplore;
    std::unordered_map<Point, Point, PointHash> cameFrom;
    std::unordered_map<Point, double, PointHash> costSoFar;
    std::unordered_map<Point, Move, PointHash> moveMap;

    toExplore.emplace(0, src);
    cameFrom[src] = src;
    costSoFar[src] = 0;

    const std::vector<Move> directions = {Move::NORTH, Move::EAST, Move::SOUTH, Move::WEST};
    const std::vector<Point> deltas = {Point(0, -1, 0), Point(1, 0, 0), Point(0, 1, 0), Point(-1, 0, 0)};

    while (!toExplore.empty()) {
        Point current = toExplore.top().second;
        toExplore.pop();

        if (current == dst) {
            Route route;
            while (current != src) {
                route.push_back(moveMap[current]);
                current = cameFrom[current];
            }
            std::reverse(route.begin(), route.end());
            return route;
        }

        for (size_t i = 0; i < directions.size(); ++ i) {
            Point next = current;
            next.x += deltas[i].x;
            next.y += deltas[i].y;

            // Check for a block above the current position before moving horizontally
            if (isValidPoint(Point(current.x, current.y, current.z + 1)) && map[index(current.x, current.y, current.z + 1)]) {
                continue; // Skip this direction if there is a block above the current position
            }

            // Simulate falling down
            while (isValidPoint(next) && !map[index(next.x, next.y, next.z)]) {
                next.z --;
            }
            next.z ++;

            // Check for block above the head in the next position
            if (isValidPoint(Point(next.x, next.y, next.z + 1)) && map[index(next.x, next.y, next.z + 1)]) {
                continue; // Skip this direction if there is a block above the head in the next position
            }

            if (isNavigable(next)) {
                double newCost = costSoFar[current] + 1; // Each move costs 1
                if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    double priority = newCost + heuristic(next, dst);
                    toExplore.emplace(priority, next);
                    cameFrom[next] = current;
                    moveMap[next] = directions[i];
                }
            }
        }
    }
    throw NoRoute(src, dst);
}
