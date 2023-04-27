#include <cstdint>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;
class Graph
{
public:
    Graph();
    Graph(std::string file);
    ~Graph();
    void addEdges(string vertex1, string vertex2);
    void addNode(string vertex, string coordinate, int weight);
    int NumNodes();
    unordered_map<uint32_t, uint32_t> GetAdjacent(uint32_t nodeID);
    int totalNodes;
    vector<tuple<string, string, int>> nodes;
    vector<tuple<string, string, int>> edges;
    set<string> uniqueNodes;
    // string from Node, string to Node, weight of from->to
    unordered_map<string, vector<tuple<string, int>>> graph;

private:
};