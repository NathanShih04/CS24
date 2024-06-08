#include "VoxMap.h"
#include "Errors.h"
#include <bitset>
#include <sstream>
#include <algorithm>

VoxMap::VoxMap(std::istream& stream) {
    stream >> width >> depth >> height;
    voxels.resize(height, std::vector<std::vector<bool>>(depth, std::vector<bool>(width, false)));

    std::string line;
    for (int h = 0; h < height; ++h) {
        std::getline(stream, line);  // Skip empty line
        for (int d = 0; d < depth; ++d) {
            std::getline(stream, line);
            std::stringstream ss(line);
            char hexChar;
            int w = 0;
            while (ss >> hexChar) {
                std::bitset<4> bits(std::stoi(std::string(1, hexChar), nullptr, 16));
                for (int i = 0; i < 4; ++i) {
                    if (w * 4 + i < width) {
                        voxels[h][d][w * 4 + i] = bits[3 - i];
                    }
                }
                ++w;
            }
        }
    }
}

bool VoxMap::isValidPoint(const Point& point) const {
    return point.x >= 0 && point.x < width &&
         point.y >= 0 && point.y < height &&
         point.z >= 0 && point.z < depth;
}

bool VoxMap::isWalkable(const Point& point) const {
    // Check if the point is within the map bounds
    if (!isValidPoint(point)) {
        return false;
    }

    // Check if the voxel at the point is empty
    if (voxels[point.z][point.y][point.x]) {
        return false;
    }

    // Check if the point is at ground level (z == 0)
    if (point.z == 0) {
        return true;
    }

    // Check if there is a full voxel directly below the point
    if (!voxels[point.z - 1][point.y][point.x]) {
        return false;
    }

    // If all checks passed, the point is walkable
    return true;
}

int VoxMap::heuristic(const Point& a, const Point& b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

struct Compare {
    bool operator()(const std::pair<int, Point>& a, const std::pair<int, Point>& b) const {
        return a.first > b.first;
    }
};

Route VoxMap::route(Point src, Point dst) {
    bool invalidSrc = false, invalidDst = false;

    // Validate the source point
    if (!isWalkable(src)) {
        throw InvalidPoint(src);
    }

    // Validate the destination point
    if (!isWalkable(dst)) {
        throw InvalidPoint(dst);
    }

    using PQElement = std::pair<int, Point>;
    std::priority_queue<PQElement, std::vector<PQElement>, Compare> frontier;
    std::unordered_map<Point, int> costSoFar;
    std::unordered_map<Point, std::pair<Point, Move>> cameFrom;

    frontier.push({0, src});
    costSoFar[src] = 0;
    cameFrom[src] = {src, Move::NORTH};  // Dummy initial value

    const std::vector<std::pair<Point, Move>> directions = {
        {{0, -1, 0}, Move::NORTH}, {{1, 0, 0}, Move::EAST}, {{0, 1, 0}, Move::SOUTH}, {{-1, 0, 0}, Move::WEST}
    };

    while (!frontier.empty()) {
        Point current = frontier.top().second;
        frontier.pop();

        if (current == dst) {
            Route route;
            Point currentPoint = current;
            while (cameFrom[currentPoint].first != currentPoint) {
                route.push_back(cameFrom[currentPoint].second);
                currentPoint = cameFrom[currentPoint].first;
            }
            std::reverse(route.begin(), route.end());
            return route;
        }

        for (const auto& [dir, move] : directions) {
            Point next(current.x + dir.x, current.y + dir.y, current.z);
            while (isValidPoint(next) && !voxels[next.z][next.y][next.x]) {
                --next.z;
            }
            ++next.z;

            bool invalidNext = false;
            if (isWalkable(next)) {
                int newCost = costSoFar[current] + 1;
                if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    int priority = newCost + heuristic(next, dst);
                    frontier.push({priority, next});
                    cameFrom[next] = {current, move};
                }
            }
        }
    }

    throw NoRoute(src, dst);
}
