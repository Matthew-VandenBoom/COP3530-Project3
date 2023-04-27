#include "Dijkstra.h"

// #define DIJKSTRA_DEBUG_FULL

#define DIJKSTRA_DEBUG
#ifdef DIJKSTRA_DEBUG
#include <iostream>
#endif
#ifdef DIJKSTRA_DEBUG
#endif

template <typename HeapType>
DijkstraRet *Dijkstra(const Graph &graph, int startNode)
{
    HeapType pq(graph.NumNodes());
    std::vector<int> prev(graph.NumNodes(), -1);
    std::vector<uint32_t> dist(graph.NumNodes(), std::numeric_limits<uint32_t>::max());
#ifdef DIJKSTRA_DEBUG_FULL
    printf(" i | dist | prev\n");
    for ( int i = 0; i < graph.NumNodes(); i++ )
    {
        printf(" %1d | %4u | %4d\n", i, dist[i], prev[i]);
    }
#endif

#ifdef DIJKSTRA_DEBUG
    printf("\ninserting start node\n");
#endif
    pq.Insert(startNode, 0);
    dist[startNode] = 0;
    int workingNode;
    uint32_t weight;
    std::unordered_map<int, EdgeInfo> outNodes;
#ifdef DIJKSTRA_DEBUG
    printf("starting loop\n");
#endif
    while ( !pq.IsEmpty() )
    {
#ifdef DIJKSTRA_DEBUG
        printf("Extracting min\n");
#endif
        workingNode = pq.ExtractMin();
#ifdef DIJKSTRA_DEBUG
        printf("Relaxing Node %d. ", workingNode);
#endif
        outNodes = graph.GetAdjacent(workingNode);
#ifdef DIJKSTRA_DEBUG
        printf("Outdegree: %d\n", (int)outNodes.size());
#endif
        for ( auto iter = outNodes.begin(); iter != outNodes.end(); iter++ )
        {
            weight = iter->second.weight;
            if ( dist[iter->first] > (dist[workingNode] + weight) )
            {
#ifdef DIJKSTRA_DEBUG
                printf("    Decreasing node %d, weight: %u\n", iter->first, dist[workingNode] + weight);
#endif
#ifdef DIJKSTRA_DEBUG
                printf("        Updating weights array\n");
#endif
                dist[iter->first] = dist[workingNode] + weight;
#ifdef DIJKSTRA_DEBUG
                printf("        Calling Decrease Key\n");
#endif
                pq.DecreaseKey(iter->first, dist[workingNode] + weight);
#ifdef DIJKSTRA_DEBUG
                printf("        Updating previous\n");
#endif
                prev[iter->first] = workingNode;
#ifdef DIJKSTRA_DEBUG
                printf("        Finished Decreasing\n");
#endif
            }
        }
    }
#ifdef DIJKSTRA_DEBUG_FULL
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