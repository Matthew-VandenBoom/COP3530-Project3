#pragma once

#include <string>

#include <cstdint>
#include <unordered_map>

class Graph
{
public:
    // however you want to create the graph, i was thinking a path to the file with the data
    // that was passed to a DataReader object
    Graph();
    Graph(std::string file);

    // returns the number of nodes in the graph
    int NumNodes();

    // returns the out edges + their weights for the passed node
    // the actual output container can change, this was just for testing
    std::unordered_map<int, uint32_t> GetAdjacent(int nodeID);

private:
};