#include "VoxMap.h"
#include "Errors.h"
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <climits>

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
           point.z >= 0 && point.z < height;
}

inline bool VoxMap::isNavigable(const Point &point) const
{
    return isValidPoint(point) && !map[index(point.x, point.y, point.z)] &&
           (point.z > 0 && map[index(point.x, point.y, point.z - 1)]);
}

struct AStarNode
{
    Point point;
    int cost;
    int heuristic;

    AStarNode(Point p, int c, int h) : point(p), cost(c), heuristic(h) {}

    bool operator>(const AStarNode &other) const
    {
        return (cost + heuristic) > (other.cost + heuristic);
    }
};

int heuristic(const Point &a, const Point &b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

Route VoxMap::route(Point src, Point dst)
{
    if (!isNavigable(src))
        throw InvalidPoint(src);
    if (!isNavigable(dst))
        throw InvalidPoint(dst);

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> toExplore;
    std::vector<bool> visited(width * depth * height, false);
    std::vector<Point> cameFrom(width * depth * height);
    std::vector<Move> moveMap(width * depth * height);

    toExplore.emplace(src, 0, heuristic(src, dst));
    visited[index(src.x, src.y, src.z)] = true;
    cameFrom[index(src.x, src.y, src.z)] = src;

    const std::vector<Move> directions = {Move::NORTH, Move::EAST, Move::SOUTH, Move::WEST};
    const std::vector<Point> deltas = {Point(0, -1, 0), Point(1, 0, 0), Point(0, 1, 0), Point(-1, 0, 0)};

    while (!toExplore.empty())
    {
        Point current = toExplore.top().point;
        toExplore.pop();

        if (current == dst)
        {
            Route route;
            while (current != src)
            {
                route.push_back(moveMap[index(current.x, current.y, current.z)]);
                current = cameFrom[index(current.x, current.y, current.z)];
            }
            std::reverse(route.begin(), route.end());
            return route;
        }

        for (size_t i = 0; i < directions.size(); ++i)
        {
            Point next = current;
            next.x += deltas[i].x;
            next.y += deltas[i].y;

            if (!isValidPoint(next))
                continue;

            // Check for flat surface movement first
            if (next.z == current.z && isNavigable(next) && !visited[index(next.x, next.y, next.z)])
            {
                int nextIndex = index(next.x, next.y, next.z);
                int priority = heuristic(next, dst);
                toExplore.emplace(next, 0, priority);
                visited[nextIndex] = true;
                cameFrom[nextIndex] = current;
                moveMap[nextIndex] = directions[i];
                continue;
            }

            // Check for overhead blocks and jumps
            if (current.z + 1 < height && !map[index(current.x, current.y, current.z + 1)])
            {
                next.z = current.z + 1;
                if (isNavigable(next) && !visited[index(next.x, next.y, next.z)])
                {
                    int nextIndex = index(next.x, next.y, next.z);
                    int priority = heuristic(next, dst);
                    toExplore.emplace(next, 0, priority);
                    visited[nextIndex] = true;
                    cameFrom[nextIndex] = current;
                    moveMap[nextIndex] = directions[i];
                    continue;
                }
            }

            // Check for drops
            int nextZ = next.z;
            while (nextZ > 0 && !map[index(next.x, next.y, nextZ - 1)])
            {
                nextZ--;
            }
            next.z = nextZ;
            if (isNavigable(next) && !visited[index(next.x, next.y, next.z)])
            {
                int nextIndex = index(next.x, next.y, next.z);
                int priority = heuristic(next, dst);
                toExplore.emplace(next, 0, priority);
                visited[nextIndex] = true;
                cameFrom[nextIndex] = current;
                moveMap[nextIndex] = directions[i];
            }
        }
    }
    throw NoRoute(src, dst);
}
