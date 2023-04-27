#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

struct EdgeInfo
{
    EdgeInfo() : EdgeInfo(0xFFFFFFFF, 0, 0, 0, 0, 0) {}
    EdgeInfo(uint32_t weight, uint8_t red, uint8_t green, uint8_t blue, int x, int y) : weight(weight), r(red), g(green), b(blue), x(x), y(y)
    {
        if ( weight > 518400000 && (x || y) )
        {
            printf("weight is unusually high %u\n", weight);
        }
    }
    uint32_t weight;
    uint8_t r, g, b;
    int x, y;
};

class Graph
{
public:
    Graph(std::string file, int xMin, int yMin, int xMax, int yMax);
    Graph(std::string file) : Graph(file, 0, 0, 399, 399){};
    ~Graph();

    int NumNodes() const;
    std::unordered_map<int, EdgeInfo> GetAdjacent(int nodeID) const;

private:
    std::vector<std::unordered_map<int, EdgeInfo>> adjacency;

    int totalNodes;
};