#include "VoxMap.h"
#include "Errors.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

VoxMap::VoxMap(std::istream& stream) {
    stream >> width >> depth >> height;
    map.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width)));

    for (int z = 0; z < height; ++z) {
        std::string line;
        for (int y = 0; y < depth; ++y) {
            stream >> line;
            for (int x = 0; x < width / 4; ++x) {
                char hexDigit = line[x];
                int value = (hexDigit >= '0' && hexDigit <= '9') ? hexDigit - '0' : hexDigit - 'A' + 10;
                for (int bit = 0; bit < 4; ++bit) {
                    map[z][y][x * 4 + bit] = (value & (8 >> bit)) != 0;
                }
            }
        }
    }
}

bool VoxMap::isValidPoint(const Point& point) const {
    return point.x >= 0 && point.x < width &&
           point.y >= 0 && point.y < depth &&
           point.z >= 0 && point.z < height;
}

bool VoxMap::isNavigable(const Point& point) const {
    return isValidPoint(point) && !map[point.z][point.y][point.x] && 
           (point.z > 0 && map[point.z - 1][point.y][point.x]);
}

Route VoxMap::route(Point src, Point dst) {
    if (!isNavigable(src)) throw InvalidPoint(src);
    if (!isNavigable(dst)) throw InvalidPoint(dst);

    std::queue<Point> toExplore;
    std::unordered_map<Point, Point, PointHash> cameFrom;
    std::unordered_map<Point, Move, PointHash> moveMap;
    std::unordered_set<Point, PointHash> visited;

    toExplore.push(src);
    visited.insert(src);

    std::vector<Move> directions = {Move::NORTH, Move::EAST, Move::SOUTH, Move::WEST};
    std::vector<Point> deltas = {Point(0, -1, 0), Point(1, 0, 0), Point(0, 1, 0), Point(-1, 0, 0)};

    while (!toExplore.empty()) {
        Point current = toExplore.front();
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
            
            while (isValidPoint(next) && !map[next.z][next.y][next.x]) {
                next.z--;
            }
            next.z++;

            if (isNavigable(next) && visited.find(next) == visited.end()) {
                toExplore.push(next);
                visited.insert(next);
                cameFrom[next] = current;
                moveMap[next] = directions[i];
            }
        }
    }
    throw NoRoute(src, dst);
}
