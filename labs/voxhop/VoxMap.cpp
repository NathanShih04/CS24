#include "VoxMap.h"
#include "Errors.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <limits>

VoxMap::VoxMap(std::istream &stream)
{
    stream >> width >> depth >> height;
    map.resize(width * depth * height);

    std::vector<int> hexTable(256, 0);
    for (char c = '0'; c <= '9'; ++c)
        hexTable[c] = c - '0';
    for (char c = 'A'; c <= 'F'; ++c)
        hexTable[c] = c - 'A' + 10;
    for (char c = 'a'; c <= 'f'; ++c)
        hexTable[c] = c - 'a' + 10;

    for (int z = 0; z < height; ++z)
    {
        for (int y = 0; y < depth; ++y)
        {
            std::string line;
            stream >> line;
            const char *linePtr = line.c_str();
            for (int x = 0; x < width / 4; ++x)
            {
                int value = hexTable[static_cast<unsigned char>(linePtr[x])];
                int baseIndex = index(x * 4, y, z);
                map[baseIndex] = (value & 8) != 0;
                if (x * 4 + 1 < width)
                    map[baseIndex + 1] = (value & 4) != 0;
                if (x * 4 + 2 < width)
                    map[baseIndex + 2] = (value & 2) != 0;
                if (x * 4 + 3 < width)
                    map[baseIndex + 3] = (value & 1) != 0;
            }
        }
    }
}

inline int VoxMap::index(int x, int y, int z) const
{
    return z * width * depth + y * width + x;
}

inline bool VoxMap::isValidPoint(const Point &point) const
{
    return point.x >= 0 && point.x < width &&
           point.y >= 0 && point.y < depth &&
           point.z > 0 && point.z < height;
}

inline bool VoxMap::isNavigable(const Point &point) const
{
    return isValidPoint(point) && !map[index(point.x, point.y, point.z)] &&
           map[index(point.x, point.y, point.z - 1)];
}

int heuristic(const Point &a, const Point &b)
{
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst)
{
    if (!isNavigable(src))
        throw InvalidPoint(src);
    if (!isNavigable(dst))
        throw InvalidPoint(dst);

    std::unordered_map<Point, Point, PointHash> cameFrom;
    std::unordered_map<Point, int, PointHash> gScore;
    std::unordered_map<Point, int, PointHash> fScore;
    
    auto cmp = [&fScore](const Point &a, const Point &b) {
        return fScore[a] > fScore[b];
    };
    std::priority_queue<Point, std::vector<Point>, decltype(cmp)> openSet(cmp);

    gScore[src] = 0;
    fScore[src] = heuristic(src, dst);
    openSet.push(src);

    const std::vector<Point> directions = {
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}
    };

    while (!openSet.empty())
    {
        Point current = openSet.top();
        openSet.pop();

        if (current == dst)
        {
            Route route;
            while (current != src)
            {
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

        for (const auto &dir : directions)
        {
            Point next = {current.x + dir.x, current.y + dir.y, current.z};
            int fallHeight = 0;

            // Check for falling
            while (isValidPoint(next) && !map[index(next.x, next.y, next.z)])
            {
                next.z--;
                fallHeight++;
                if (fallHeight > 1) break; // Ensure fall height is not greater than 1
            }
            next.z++;
            if (fallHeight <= 1 && isNavigable(next))
            {
                if (isValidPoint(Point(next.x, next.y, next.z + 1)) &&
                    !map[index(next.x, next.y, next.z + 1)] && // No voxel directly above the current voxel
                    isValidPoint(Point(next.x, next.y, next.z + 2)) &&
                    !map[index(next.x, next.y, next.z + 2)]) { // No voxel directly above the destination voxel
                    next.z++;
                }

                if (!cameFrom.count(next) || gScore[current] + 1 < gScore[next])
                {
                    cameFrom[next] = current;
                    gScore[next] = gScore[current] + 1;
                    fScore[next] = gScore[next] + heuristic(next, dst);
                    openSet.push(next);
                }
            }
        }
    }

    throw NoRoute(src, dst);
}
