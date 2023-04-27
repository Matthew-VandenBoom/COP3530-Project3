#include "graph.h"
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
using namespace std;
// constructor
Graph::Graph()
{
}
// adds node/vertex
Graph::Graph(string file)
{
    ifstream infile(file);
    string line;
    while ( getline(infile, line) )
    {
        stringstream ss(line);
        string userHash;
        string coordString;
        int timeString;
        ss >> userHash >> coordString >> timeString;
        addNode(userHash, coordString, timeString);
    }
}
void Graph::addNode(string vertex, string coordinate, int weight)
{
    if ( uniqueNodes.insert(vertex).second )
    {
        nodes.push_back(make_tuple(vertex, coordinate, weight));
    }
}
void Graph::addEdges(string vertex1, string vertex2)
{
    for ( int i = 0; i < nodes.size(); ++i )
    {
        for ( int j = 0; j < nodes.size(); ++j )
        {
            if ( vertex1 == get<0>(nodes[i]) && vertex2 == get<0>(nodes[j]) )
            {
                string coordinate1 = get<1>(nodes[i]);
                string coordinate2 = get<1>(nodes[j]);
                int weight1 = get<2>(nodes[i]);
                int weight2 = get<2>(nodes[j]);
                if ( coordinate1 == coordinate2 )
                {
                    int newWeight = weight2 - weight1;
                    edges.push_back(make_tuple(vertex1, vertex2, newWeight));
                    graph[vertex1].push_back(make_tuple(vertex2, newWeight));
                }
            }
        }
    }
}
int Graph::NumNodes()
{
    return nodes.size();
}