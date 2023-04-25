#include "DataReader.h"
#include "graph.h"
#include <iostream>
#include <string>

int main(int argc, char *argv)
{
    DataReader data("../data/data_sorted.pldat");
    printf("Entry Count: %u\n", data.RemainingEntries());
}