#include "VoxMap.h"
#include "Errors.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <future>
#include <algorithm>

VoxMap::VoxMap(std::istream &stream)
{
    stream >> width >> depth >> height;
    map.resize(width * depth * height);

    // Precompute hex value lookup table
    std::vector<int> hexTable(256, 0);
    for (char c = '0'; c <= '9'; ++c) hexTable[c] = c - '0';
    for (char c = 'A'; c <= 'F'; ++c) hexTable[c] = c - 'A' + 10;
    for (char c = 'a'; c <= 'f'; ++c) hexTable[c] = c - 'a' + 10;

    auto processLayer = [&](int z) {
        for (int y = 0; y < depth; ++y) {
            std::string line;
            stream >> line;
            const char *linePtr = line.c_str();
            for (int x = 0; x < width / 4; ++x) {
                int value = hexTable[static_cast<unsigned char>(linePtr[x])];
                int baseIndex = index(x * 4, y, z);
                map[baseIndex] = (value & 8) != 0;
                if (x * 4 + 1 < width) map[baseIndex + 1] = (value & 4) != 0;
                if (x * 4 + 2 < width) map[baseIndex + 2] = (value & 2) != 0;
                if (x * 4 + 3 < width) map[baseIndex + 3] = (value & 1) != 0;
            }
        }
    };

    // Process each layer in parallel
    std::vector<std::future<void>> futures;
    for (int z = 0; z < height; ++z) {
        futures.push_back(std::async(std::launch::async, processLayer, z));
    }
    for (auto &fut : futures) {
        fut.get();
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

        for (size_t i = 0; i < directions.size(); ++i) {
            Point next = current;
            next.x += deltas[i].x;
            next.y += deltas[i].y;

            if (isValidPoint(Point(current.x, current.y, current.z + 1)) && map[index(current.x, current.y, current.z + 1)]) {
                continue;
            }

            while (isValidPoint(next) && !map[index(next.x, next.y, next.z)]) {
                next.z--;
            }
            next.z++;

            if (isValidPoint(Point(next.x, next.y, next.z + 1)) && map[index(next.x, next.y, next.z + 1)]) {
                continue;
            }

            if (isNavigable(next)) {
                double newCost = costSoFar[current] + 1;
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
