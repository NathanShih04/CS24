#include "VoxMap.h"
#include "Errors.h"
#include <stdexcept>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <cctype>

// Custom hash function for Point
struct PointHash {
    std::size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1) ^ (std::hash<int>()(p.z) << 2);
    }
};

// Custom equality function for Point
struct PointEqual {
    bool operator()(const Point& lhs, const Point& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
};

VoxMap::VoxMap(std::istream& stream) {
    // Read the map dimensions from the input stream
    stream >> width >> depth >> height;
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line

    // Initialize the voxel map with the given dimensions
    voxels.resize(height, std::vector<std::vector<Voxel>>(depth, std::vector<Voxel>(width, {false, false})));

    // Read the voxel data from the input stream
    for (int h = 0; h < height; ++h) {
        std::string line;
        std::getline(stream, line); // Read the empty line
        if (!line.empty()) {
            throw std::invalid_argument("Expected an empty line between tiers");
        }
        for (int d = 0; d < depth; ++d) {
            std::getline(stream, line);
            if (line.size() < static_cast<std::string::size_type>(width / 4)) {
                throw std::invalid_argument("Map line length is less than expected");
            }
            for (int w = 0; w < width / 4; ++w) {
                char hex_char = line[w];
                if (!isxdigit(hex_char)) {
                    std::cerr << "Invalid character in map input: " << hex_char << std::endl;
                    throw std::invalid_argument("Invalid character in map input");
                }
                int value = std::stoi(std::string(1, hex_char), nullptr, 16);
                for (int bit = 0; bit < 4; ++bit) {
                    voxels[h][d][w * 4 + bit].isFilled = (value & (1 << (3 - bit))) != 0;
                }
            }
        }
    }

    // Mark surface voxels
    for (int h = 0; h < height - 1; ++h) {
        for (int d = 0; d < depth; ++d) {
            for (int w = 0; w < width; ++w) {
                if (voxels[h][d][w].isFilled && !voxels[h + 1][d][w].isFilled) {
                    voxels[h][d][w].isSurface = true;
                }
            }
        }
    }
}

VoxMap::~VoxMap() {
    // Destructor
}

bool VoxMap::is_valid_point(const Point& point) const {
    return point.x >= 0 && point.x < width &&
           point.y >= 0 && point.y < depth &&
           point.z >= 0 && point.z < height;
}

bool VoxMap::is_valid_voxel(const Point& point) const {
    if (!is_valid_point(point)) return false;
    if (voxels[point.z][point.y][point.x].isFilled) return false; // Must be empty
    if (point.z == 0 || voxels[point.z - 1][point.y][point.x].isFilled) return true; // Must have a full voxel below
    return false;
}

Route VoxMap::route(Point src, Point dst) {
    if (!is_valid_voxel(src)) {
        throw InvalidPoint(src);
    }
    if (!is_valid_voxel(dst)) {
        throw InvalidPoint(dst);
    }

    struct Node {
        Point pt;
        int cost;
        Route path;
        bool operator>(const Node& other) const {
            return cost > other.cost;
        }
    };

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_set<Point, PointHash, PointEqual> closedSet;

    auto heuristic = [](const Point& a, const Point& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
    };

    openSet.push({src, 0, {}});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.pt == dst) {
            return current.path;
        }

        if (closedSet.find(current.pt) != closedSet.end()) continue;
        closedSet.insert(current.pt);

        static const std::array<std::pair<Point, Move>, 4> directions{
            std::pair<Point, Move>{{0, 1, 0}, Move::NORTH}, 
            std::pair<Point, Move>{{1, 0, 0}, Move::EAST},
            std::pair<Point, Move>{{0, -1, 0}, Move::SOUTH}, 
            std::pair<Point, Move>{{-1, 0, 0}, Move::WEST}};

        for (const auto& [delta, move] : directions) {
            Point next = {current.pt.x + delta.x, current.pt.y + delta.y, current.pt.z};

            // Check if the next point is within bounds
            if (!is_valid_point(next)) continue;

            // If the next point is empty, simulate falling
            if (!voxels[next.z][next.y][next.x].isFilled) {
                while (next.z > 0 && !voxels[next.z - 1][next.y][next.x].isFilled) {
                    next.z--;
                }
                // If it falls out of the map or onto an invalid voxel, continue
                if (!is_valid_voxel(next)) continue;
            }

            int newCost = current.cost + 1 + heuristic(next, dst);
            Route newPath = current.path;
            newPath.push_back(move);

            openSet.push({next, newCost, newPath});
        }
    }

    throw NoRoute(src, dst);
}
