#include "VoxMap.h"
#include "Errors.h"
#include <queue>
#include <unordered_set>
#include <sstream>
#include <bitset>
#include <cmath>
#include <array>
#include <stdexcept>
#include <iostream>

VoxelMap::VoxelMap(int width, int depth, int height)
    : width(width), depth(depth), height(height),
      voxels(width * depth * height, Voxel{false, false}) {}

Voxel& VoxelMap::getVoxel(int x, int y, int z) {
    return voxels[(z * depth + y) * width + x];
}

const Voxel& VoxelMap::getVoxel(int x, int y, int z) const {
    return voxels[(z * depth + y) * width + x];
}

bool VoxelMap::isValid(int x, int y, int z) const {
    return x >= 0 && x < width && y >= 0 && y < depth && z >= 0 && z < height;
}

void VoxelMap::markSurface() {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < depth; ++y) {
            for (int z = 1; z < height; ++z) {
                if (getVoxel(x, y, z).isFilled && !getVoxel(x, y, z - 1).isFilled) {
                    getVoxel(x, y, z - 1).isSurface = true;
                }
            }
        }
    }
}

VoxMap::VoxMap(std::istream& stream) {
    int width, depth, height;
    stream >> width >> depth >> height;

    map = std::make_unique<VoxelMap>(width, depth, height);

    std::string line;
    for (int z = 0; z < height; ++z) {
        std::getline(stream, line); // Read empty line
        if (line.empty()) {
            std::getline(stream, line); // Read next line if current is empty
        }

        for (int y = 0; y < depth; ++y) {
            std::getline(stream, line);
            if (line.empty()) continue; // Skip empty lines

            for (int x = 0; x < width / 4; ++x) {
                try {
                    std::bitset<4> bits(std::stoi(std::string(1, line[x]), nullptr, 16));
                    for (int b = 0; b < 4; ++b) {
                        map->getVoxel(x * 4 + b, y, z).isFilled = bits[3 - b];
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid character in map input: " << line[x] << std::endl;
                    throw;
                }
            }
        }
    }

    map->markSurface();
}

VoxMap::~VoxMap() {}

bool VoxMap::isValidPoint(const Point& p) const {
    if (!map->isValid(p.x, p.y, p.z)) {
        return false;
    }
    if (map->getVoxel(p.x, p.y, p.z).isFilled) {
        return false;
    }
    if (p.z > 0 && !map->getVoxel(p.x, p.y, p.z - 1).isFilled) {
        return false;
    }
    return true;
}

Route VoxMap::route(Point src, Point dst) {
    if (!isValidPoint(src)) throw InvalidPoint(src);
    if (!isValidPoint(dst)) throw InvalidPoint(dst);

    struct Node {
        Point pt;
        int cost;
        Route path;
        bool operator>(const Node& other) const {
            return cost > other.cost;
        }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_set<int> closedSet;

    auto hashPoint = [this](const Point& p) {
        return (p.z * map->depth + p.y) * map->width + p.x;
    };

    auto heuristic = [](const Point& a, const Point& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
    };

    openSet.push({src, 0, {}});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.pt.x == dst.x && current.pt.y == dst.y && current.pt.z == dst.z) {
            return current.path;
        }

        int currentHash = hashPoint(current.pt);
        if (closedSet.find(currentHash) != closedSet.end()) continue;
        closedSet.insert(currentHash);

        static const std::array<std::pair<Point, Move>, 4> directions{
            std::pair<Point, Move>{{0, 1, 0}, Move::NORTH}, 
            std::pair<Point, Move>{{1, 0, 0}, Move::EAST},
            std::pair<Point, Move>{{0, -1, 0}, Move::SOUTH}, 
            std::pair<Point, Move>{{-1, 0, 0}, Move::WEST}};

        for (const auto& [delta, move] : directions) {
            Point next = {current.pt.x + delta.x, current.pt.y + delta.y, current.pt.z};
            if (!isValidPoint(next)) continue;

            int newCost = current.cost + 1 + heuristic(next, dst);
            Route newPath = current.path;
            newPath.push_back(move);

            openSet.push({next, newCost, newPath});
        }
    }

    throw NoRoute(src, dst);
}


