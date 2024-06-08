#include "VoxMap.h"
#include "Errors.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <algorithm>

VoxMap::VoxMap(std::istream& stream) {
    stream >> width >> depth >> height;
    map.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width, false)));

    for (int h = 0; h < height; ++h) {
        std::string line;
        for (int d = 0; d < depth; ++d) {
            stream >> line;
            for (int w = 0; w < width / 4; ++w) {
                char hexChar = line[w];
                int hexVal = (hexChar >= '0' && hexChar <= '9') ? hexChar - '0' : hexChar - 'a' + 10;
                for (int b = 0; b < 4; ++b) {
                    map[h][d][4 * w + b] = (hexVal & (1 << (3 - b))) != 0;
                }
            }
        }
    }
}

VoxMap::~VoxMap() {
}

bool VoxMap::isValidPoint(const Point& point) const {
    if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= depth || point.z < 0 || point.z >= height) {
        return false;
    }
    return !map[point.z][point.y][point.x] && point.z > 0 && map[point.z - 1][point.y][point.x];
}

bool VoxMap::isWalkable(const Point& point) const {
    return isValidPoint(point) && point.z >= 0 && (!map[point.z + 1][point.y][point.x]);
}

std::vector<Point> VoxMap::getNeighbors(const Point& point) const {
    std::vector<Point> neighbors;
    static const std::vector<Point> directions{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}};
    
    for (const auto& dir : directions) {
        Point newPoint = {point.x + dir.x, point.y + dir.y, point.z};
        if (isValidPoint(newPoint)) {
            neighbors.push_back(newPoint);
        }
    }
    
    return neighbors;
}

Route VoxMap::route(Point src, Point dst) {
    if (!isValidPoint(src)) {
        throw InvalidPoint(src);
    }
    if (!isValidPoint(dst)) {
        throw InvalidPoint(dst);
    }
    if (src == dst) {
        return Route();
    }

    std::unordered_set<Point, PointHash> closedSet;
    std::priority_queue<std::pair<int, Point>, std::vector<std::pair<int, Point>>, ComparePoints> openSet;
    std::unordered_map<Point, Point, PointHash> cameFrom;
    std::unordered_map<Point, int, PointHash> gScore, fScore;

    auto heuristic = [](const Point& a, const Point& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
    };

    openSet.emplace(0, src);
    gScore[src] = 0;
    fScore[src] = heuristic(src, dst);

    while (!openSet.empty()) {
        Point current = openSet.top().second;
        openSet.pop();

        if (current == dst) {
            Route path;
            while (cameFrom.find(current) != cameFrom.end()) {
                Point parent = cameFrom[current];
                if (parent.x < current.x) path.push_back(Move::EAST);
                if (parent.x > current.x) path.push_back(Move::WEST);
                if (parent.y < current.y) path.push_back(Move::SOUTH);
                if (parent.y > current.y) path.push_back(Move::NORTH);
                current = parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(current);

        for (const auto& neighbor : getNeighbors(current)) {
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            int tentative_gScore = gScore[current] + 1;

            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, dst);
                openSet.emplace(fScore[neighbor], neighbor);
            }
        }
    }

    throw NoRoute(src, dst);
}
