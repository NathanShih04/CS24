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
                    voxels[h][d][w * 4 + i] = bits[3 - i];
                }
                ++w;
            }
        }
    }
}

bool VoxMap::isValidPoint(const Point& point) const {
    return point.x >= 0 && point.x < width && point.y >= 0 && point.y < depth && point.z >= 0 && point.z < height;
}

bool VoxMap::isWalkable(const Point& point) const {
    if (!isValidPoint(point) || voxels[point.z][point.y][point.x]) return false;
    if (point.z == 0) return true;
    return voxels[point.z - 1][point.y][point.x];
}

int VoxMap::heuristic(const Point& a, const Point& b) const {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst) {
    if (!isWalkable(src)) throw InvalidPoint(src);
    if (!isWalkable(dst)) throw InvalidPoint(dst);

    using PQElement = std::pair<int, Point>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> frontier;
    std::unordered_map<int, int> costSoFar;
    std::unordered_map<int, std::pair<int, Move>> cameFrom;

    auto hash = [this](const Point& p) { return p.z * width * depth + p.y * width + p.x; };

    frontier.push({0, src});
    costSoFar[hash(src)] = 0;
    cameFrom[hash(src)] = {hash(src), Move::NORTH};  // Dummy initial value

    const std::vector<std::pair<Point, Move>> directions = {
        {{0, -1, 0}, Move::NORTH}, {{1, 0, 0}, Move::EAST}, {{0, 1, 0}, Move::SOUTH}, {{-1, 0, 0}, Move::WEST}
    };

    while (!frontier.empty()) {
        Point current = frontier.top().second;
        frontier.pop();

        if (current.x == dst.x && current.y == dst.y && current.z == dst.z) {
            Route route;
            int currentHash = hash(current);
            while (cameFrom.find(currentHash) != cameFrom.end() && currentHash != hash(src)) {
                auto [prevHash, move] = cameFrom[currentHash];
                route.push_back(move);
                currentHash = prevHash;
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

            if (isWalkable(next)) {
                int nextHash = hash(next);
                int newCost = costSoFar[hash(current)] + 1;
                if (costSoFar.find(nextHash) == costSoFar.end() || newCost < costSoFar[nextHash]) {
                    costSoFar[nextHash] = newCost;
                    int priority = newCost + heuristic(next, dst);
                    frontier.push({priority, next});
                    cameFrom[nextHash] = {hash(current), move};
                }
            }
        }
    }

    throw NoRoute(src, dst);
}
