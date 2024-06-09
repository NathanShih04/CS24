#include "VoxMap.h"
#include "Errors.h"
#include <sstream>
#include <algorithm>
#include <limits> // Include this for std::numeric_limits
#include <stdexcept> // Include this for std::runtime_error

VoxMap::VoxMap(std::istream &stream) {
    if (!(stream >> width >> depth >> height)) {
        throw std::runtime_error("Failed to read map dimensions.");
    }
    map.resize(width * depth * height);

    std::vector<int> hexTable(256, 0);
    for (char c = '0'; c <= '9'; ++c)
        hexTable[c] = c - '0';
    for (char c = 'A'; c <= 'F'; ++c)
        hexTable[c] = c - 'A' + 10;
    for (char c = 'a'; c <= 'f'; ++c)
        hexTable[c] = c - 'a' + 10;

    // Skip to the next line after dimensions
    if (!stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n')) {
        throw std::runtime_error("Failed to skip to the next line after dimensions.");
    }

    for (int z = 0; z < height; ++z) {
        for (int y = 0; y < depth; ++y) {
            std::string line;
            if (!std::getline(stream, line) || line.size() < static_cast<size_t>(width / 4)) {
                throw std::runtime_error("Line too short or unexpected end of input while reading voxel data.");
            }
            const char *linePtr = line.c_str();
            int baseIndex = z * width * depth + y * width;

            for (int x = 0; x < width / 4; ++x) {
                int value = hexTable[static_cast<unsigned char>(linePtr[x])];

                map[baseIndex + x * 4] = (value & 8) != 0;
                if (x * 4 + 1 < width)
                    map[baseIndex + x * 4 + 1] = (value & 4) != 0;
                if (x * 4 + 2 < width)
                    map[baseIndex + x * 4 + 2] = (value & 2) != 0;
                if (x * 4 + 3 < width)
                    map[baseIndex + x * 4 + 3] = (value & 1) != 0;
            }
        }
        // Skip empty line between tiers if it exists
        if (stream.peek() == '\n') {
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

bool VoxMap::isValid(const Point& p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < depth && p.z >= 0 && p.z < height;
}

bool VoxMap::isWalkable(const Point& p) const {
    return isValid(p) && !map[index(p.x, p.y, p.z)] && (p.z == 0 || map[index(p.x, p.y, p.z - 1)]);
}

std::vector<Point> VoxMap::getNeighbors(const Point& p) const {
    std::vector<Point> neighbors;
    std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (auto& [dx, dy] : directions) {
        Point neighbor(p.x + dx, p.y + dy, p.z);
        if (isValid(neighbor)) {
            // Check for falling off a ledge
            if (!map[index(neighbor.x, neighbor.y, neighbor.z)]) {
                while (neighbor.z > 0 && !map[index(neighbor.x, neighbor.y, neighbor.z - 1)]) {
                    --neighbor.z;
                }
                if (neighbor.z > 0 && isValid(neighbor)) {
                    neighbors.push_back(neighbor);
                }
            } else if (isValid({p.x + dx, p.y + dy, p.z + 1}) && !map[index(p.x + dx, p.y + dy, p.z + 1)] && !map[index(p.x, p.y, p.z + 1)]) {
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
    if (!isWalkable(src) || src.z == 0) {
        throw InvalidPoint(src);
    }
    if (!isWalkable(dst) || dst.z == 0) {
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
