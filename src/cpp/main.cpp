#include <iostream>
#include <string>

#include "DataReader.h"
#include "Dijkstra.h"
#include "graph.h"

#include "BinaryHeap.h"
#include "FibonacciHeap.h"

#define DATA_PATH "../data/data_sorted.pldat"

#include "chrono"

using std::cout, std::cin;
int main()
{
    auto starttime = std::chrono::system_clock::now();
    auto endtime = std::chrono::system_clock::now();

    cout << "Creating Graph\n";
    Graph g(DATA_PATH, 0, 0, 400, 400);
    cout << "There are " << g.NumNodes() << " nodes in the graph.\n";
    int startNode, endNode, alg;
    DijkstraRet *ret;
    cout << "Enter the Starting Node (0 - " << g.NumNodes() << "): ";
    cin >> startNode;
    cout << "Enter the Target Node (0 - " << g.NumNodes() << "): ";
    cin >> endNode;
    while ( true )
    {
        cout << "Enter which type of heap use for Dijkstra's Priority Queue (1: Binary Heap, 2: Fibonacci Heap): ";
        cin >> alg;
        if ( alg == 1 )
        {
            cout << "Searching For path from " << startNode << " to " << endNode << ".\n";
            starttime = std::chrono::system_clock::now();
            ret = Dijkstra<BinaryHeap>(g, startNode);
            endtime = std::chrono::system_clock::now();
            break;
        }
        else if ( alg == 2 )
        {
            cout << "Searching For path from " << startNode << " to " << endNode << ".\n";
            starttime = std::chrono::system_clock::now();
            ret = Dijkstra<FibonacciHeap>(g, startNode);
            endtime = std::chrono::system_clock::now();
            break;
        }
        else
        {
            cout << "That is not a valid identifier. Please try again." << endl;
            cin >> alg;
        }
    }
    if ( ret->prev[endNode] != -1 )
    {
        cout << "Path found with a total separation of " << ret->dist[endNode] << std::endl;
    }
    else
    {
        cout << "No path exists between the selected users.";
    }
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(endtime.time_since_epoch()).count() - std::chrono::duration_cast<std::chrono::seconds>(starttime.time_since_epoch()).count();

    std::cout << "Time to run Dijkstra's: " << diff << " seconds\n";
    delete ret;
    return 0;
}
int main2()
{
    // DataReader test(DATA_PATH);
    auto starttime = std::chrono::system_clock::now();

    // clang-format off
    std::cout << "startime seconds since epoch: " << std::chrono::duration_cast<std::chrono::seconds>(starttime.time_since_epoch()).count() << '\n';
    // clang-format on

    Graph test(DATA_PATH, 0, 0, 99, 99);
    printf("Total Nodes: %d", test.NumNodes());
    DijkstraRet *vals;
    int testpoints[] = {1000}; // , 4, 32424, 788888, 7, 5674};
    for ( int j = 0; j < 1; j++ )
    {
        printf("Starting at: %d\n", testpoints[j]);
        printf("Outdegree: %d\n", (int)(test.GetAdjacent(j).size()));
        for ( auto iter = test.GetAdjacent(j).begin(); iter != test.GetAdjacent(j).end(); iter++ )
        {
            printf("(%d, %u), ", (*iter).first, iter->second.weight);
        }
        vals = Dijkstra<BinaryHeap>(test, testpoints[j]);
        printf("\n\n i | dist | prev\n");
        for ( int i = 0; i < test.NumNodes(); i++ )
        {
            if ( vals->dist.at(i) != 0xFFFFFFFF )
            {
                printf(" %d | %u | %d\n", i, vals->dist.at(i), vals->prev.at(i));
            }
        }
        delete vals;
    }

    auto endtime = std::chrono::system_clock::now();
    // clang-format off
    std::cout << "end seconds since epoch: " << std::chrono::duration_cast<std::chrono::seconds>(starttime.time_since_epoch()).count() << '\n';
    auto diff = endtime - starttime;
    std::cout << "diff (seconds): " << std::chrono::duration_cast<std::chrono::seconds>(diff).count() << '\n';
    // clang-format on

    printf("Total Nodes: %d\n", test.NumNodes());
}
