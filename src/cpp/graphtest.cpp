#include "graph.h"

Graph::Graph()
{
}
Graph::Graph(std::string file)
{
}

const static std::unordered_map<int, std::unordered_map<int, uint32_t>> testgraph =
    {
        {0, {{1, 5}, {4, 1}}},
        {1, {{2, 1}, {3, 4}, {4, 10}}},
        {2, {{4, 3}}},
        {3, {{1, 2}, {2, 4}, {4, 6}}},
        {4, {{3, 1}}},
};

int Graph::NumNodes()
{
    return 5;
}

std::unordered_map<int, uint32_t> Graph::GetAdjacent(int nodeID)
{
    return testgraph.at(nodeID);
}