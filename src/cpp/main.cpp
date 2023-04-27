#include <iostream>
#include <string>

#include "DataReader.h"
#include "Dijkstra.h"
#include "graph.h"

#include "BinaryHeap.h"
#include "FibonacciHeap.h"

#define DATA_PATH "../data/data_sorted.pldat"

int fibtest(int argc, char *argv[])
{
    FibonacciHeap heap(100);
    heap.Insert(0, 10);
    heap.Insert(1, 5);
    heap.Insert(2, 3);
    heap.Insert(3, 8);
    heap.Insert(4, 9);
    heap.Insert(5, 7);

    heap.Insert(6, 1);
    heap.Insert(7, 2);
    heap.Insert(8, 3);
    heap.Insert(9, 11);
    heap.Insert(10, 13);
    heap.Insert(11, 6);
    heap.PrintRoots();

    printf("\n\n");
    heap.PrintHeap();

    heap.ExtractMin();
    printf("\n\n");
    heap.PrintHeap();

    heap.DecreaseKey(11, 1);
    heap.PrintHeap();

    heap.DecreaseKey(9, 0);
    heap.PrintHeap();
    return 0;
}

int main()
{
    Graph test;
    for ( int i = 0; i < test.NumNodes(); i++ )
    {
        std::cout << i << ": ";
        for ( auto iter = test.GetAdjacent(i).begin(); iter != test.GetAdjacent(i).end(); iter++ )
        {
            cout << "(" << (*iter).first << ", " << (*iter).second << "), ";
        }
        std::cout << std::endl;
    }
    DijkstraRet *res = Dijkstra<FibonacciHeap>(test, 0);
    return 0;
}