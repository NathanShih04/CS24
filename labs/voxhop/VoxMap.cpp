#include "VoxMap.h"
#include "Errors.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
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

    // Read the stream into a string buffer for recursive processing
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string data = buffer.str();
    size_t offset = 0;

    // Helper lambda to recursively process each dimension
    std::function<void(int, int, int)> processDimension;
    processDimension = [&](int z, int y, int x4) {
        if (z >= height) return;
        if (y >= depth) {
            processDimension(z + 1, 0, 0);
            return;
        }
        if (x4 >= width / 4) {
            processDimension(z, y + 1, 0);
            return;
        }

        // Process the current hex digit and bits
        char hexDigit = data[offset++];
        int value = hexTable[static_cast<unsigned char>(hexDigit)];
        for (int bit = 0; bit < 4; ++bit) {
            int actualX = x4 * 4 + bit;
            if (actualX < width) {
                map[index(actualX, y, z)] = (value & (8 >> bit)) != 0;
            }
        }

        // Recursively process the next position in the current dimension
        processDimension(z, y, x4 + 1);
    };

    // Start the recursive processing
    processDimension(0, 0, 0);
}

inline int VoxMap::index(int x, int y, int z) const {
    return z * width * depth + y * width + x;
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
