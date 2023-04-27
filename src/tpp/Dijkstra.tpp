#include "Dijkstra.h"

#define DIJKSTRA_DEBUG
#ifdef DIJKSTRA_DEBUG
#include <iostream>
#endif
#ifdef DIJKSTRA_DEBUG
#endif

template <typename HeapType>
DijkstraRet *Dijkstra(Graph &graph, int startNode)
{
    HeapType pq(graph.NumNodes());
    std::vector<int> prev(graph.NumNodes(), -1);
    std::vector<uint32_t> dist(graph.NumNodes(), std::numeric_limits<uint32_t>::max());
#ifdef DIJKSTRA_DEBUG
    printf(" i | dist | prev\n");
    for ( int i = 0; i < graph.NumNodes(); i++ )
    {
        printf(" %1d | %4u | %4d\n", i, dist[i], prev[i]);
    }
#endif

    pq.Insert(startNode, 0);
    dist[startNode] = 0;
    int workingNode;
    std::unordered_map<int, uint32_t> outNodes;
    while ( !pq.IsEmpty() )
    {
        workingNode = pq.ExtractMin();
#ifdef DIJKSTRA_DEBUG
        printf("Relaxing Node %d\n", workingNode);
#endif
        outNodes = graph.GetAdjacent(workingNode);
        for ( auto iter = outNodes.begin(); iter != outNodes.end(); iter++ )
        {
            if ( dist[iter->first] > (dist[workingNode] + iter->second) )
            {
#ifdef DIJKSTRA_DEBUG
                printf("    Decreasing node %d\n", iter->first);
#endif
                dist[iter->first] = dist[workingNode] + iter->second;
                pq.DecreaseKey(iter->first, dist[workingNode] + iter->second);
                prev[iter->first] = workingNode;
            }
        }
    }
#ifdef DIJKSTRA_DEBUG
    printf("\n\n i | dist | prev\n");
    for ( int i = 0; i < graph.NumNodes(); i++ )
    {
        printf(" %1d | %4u | %4d\n", i, dist[i], prev[i]);
    }
#endif

    DijkstraRet *ret = new DijkstraRet();
    ret->dist = dist;
    ret->prev = prev;
    ret->startNode = startNode;

    return ret;
}