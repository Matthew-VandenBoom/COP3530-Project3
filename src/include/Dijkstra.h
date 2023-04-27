#pragma once
#include "graph.h"
#include <vector>

struct DijkstraRet
{
    int startNode;
    vector<uint32_t> dist;
    vector<int> prev;
};
template <typename HeapType>
DijkstraRet *Dijkstra(const Graph &graph, int startNode);

#include "Dijkstra.tpp"