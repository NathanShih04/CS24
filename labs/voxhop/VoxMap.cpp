#include "VoxMap.h"
#include "Errors.h"
#include <vector>
#include <queue>
#include <bitset>
#include <algorithm>

const int MAX_WIDTH = 1024; // Adjust based on your specific requirements
const int MAX_DEPTH = 1024; // Adjust based on your specific requirements
const int MAX_HEIGHT = 128; // Adjust based on your specific requirements
const int MAX_SIZE = MAX_WIDTH * MAX_DEPTH * MAX_HEIGHT;

VoxMap::VoxMap(std::istream &stream) {
    stream >> width >> depth >> height;
    map.resize(width * depth * height);

    std::vector<int> hexTable(256, 0);
    for (char c = '0'; c <= '9'; ++c) hexTable[c] = c - '0';
    for (char c = 'A'; c <= 'F'; ++c) hexTable[c] = c - 'A' + 10;
    for (char c = 'a'; c <= 'f'; ++c) hexTable[c] = c - 'a' + 10;

    for (int z = 0; z < height; ++z) {
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

Route VoxMap::route(Point src, Point dst) {
    if (!isNavigable(src))
        throw InvalidPoint(src);
    if (!isNavigable(dst))
        throw InvalidPoint(dst);

    std::queue<Point> toExplore;
    std::bitset<MAX_SIZE> visited;
    std::vector<Point> cameFrom(MAX_SIZE);
    std::vector<Move> moveMap(MAX_SIZE);

    int srcIndex = index(src.x, src.y, src.z);

    toExplore.push(src);
    visited.set(srcIndex);
    cameFrom[srcIndex] = src;

    const std::vector<Move> directions = {Move::NORTH, Move::EAST, Move::SOUTH, Move::WEST};
    const std::vector<Point> deltas = {Point(0, -1, 0), Point(1, 0, 0), Point(0, 1, 0), Point(-1, 0, 0)};

    while (!toExplore.empty()) {
        Point current = toExplore.front();
        toExplore.pop();

        if (current == dst) {
            Route route;
            while (current != src) {
                route.push_back(moveMap[index(current.x, current.y, current.z)]);
                current = cameFrom[index(current.x, current.y, current.z)];
            }
            std::reverse(route.begin(), route.end());
            return route;
        }

        for (size_t i = 0; i < directions.size(); ++i) {
            Point next = current;
            next.x += deltas[i].x;
            next.y += deltas[i].y;

            if (!isValidPoint(next))
                continue;

            // Check if we can move horizontally without an overhead block
            if (current.z + 1 < height && map[index(current.x, current.y, current.z + 1)]) {
                continue;
            }

            int nextZ = next.z;
            while (nextZ >= 0 && !map[index(next.x, next.y, nextZ)]) {
                nextZ--;
            }
            next.z = nextZ + 1;

            if (next.z < height - 1 && map[index(next.x, next.y, next.z + 1)]) {
                continue;
            }

            int nextIndex = index(next.x, next.y, next.z);
            if (isNavigable(next) && !visited.test(nextIndex)) {
                toExplore.push(next);
                visited.set(nextIndex);
                cameFrom[nextIndex] = current;
                moveMap[nextIndex] = directions[i];
            }
        }
    }
    throw NoRoute(src, dst);
}
