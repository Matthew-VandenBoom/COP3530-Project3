#include <SFML/Graphics.hpp>

#include "Textbox.h"
#include "WindowUI.h"

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

int dijkstrastest()
{
    Graph test(DATA_PATH);
    for ( int i = 0; i < test.NumNodes(); i++ )
    {
        std::cout << i << ": ";
        for ( auto iter = test.GetAdjacent(i).begin(); iter != test.GetAdjacent(i).end(); iter++ )
        {
            cout << "(" << (*iter).first << ", " << (*iter).second.weight << "), ";
        }
        std::cout << std::endl;
    }
    DijkstraRet *res = Dijkstra<FibonacciHeap>(test, 0);
    return res->startNode;
}

int realMain()
{
    sf::RenderWindow renderWindow(sf::VideoMode(1200, 600), "COP3530 Project 3 - r/Place Shortest Path");

    WindowUI ui;

    while ( renderWindow.isOpen() )
    {
        sf::Event event;
        sf::Cursor cursor;

        while ( renderWindow.pollEvent(event) )
        {
            if ( event.type == sf::Event::Closed )
            {
                renderWindow.close();
            }
        }

        renderWindow.clear();
        ui.Update(renderWindow, cursor);
        renderWindow.display();
    }
    return 0;
}

#include "chrono"

int main()
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
