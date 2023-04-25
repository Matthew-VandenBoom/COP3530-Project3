#include "converter.h"
#include <cstdlib>
#include <iostream>

#define NUM_FILES 78

using namespace std;
int main()
{
    vector<string> files;
    char buf[100];
    for ( int i = 0; i <= NUM_FILES; i++ )
    {
        sprintf(buf, "../csvdata/2022_place_canvas_history-%012d.csv", i);
        files.push_back(string(buf));
    }

    // set base time for the files, this must be the earliest timestamp in all files
    // passing the list of files will do this automatically
    // but for this dataset it has already been run, so it can just be set directly
    // SetBaseTime(files);
    SetBaseTime(1648831450, 315);

    // convert all csv files into custom format
    convertFiles(files, "../data/data_unsorted.pldat");

    // sort all entries in the file by time, to make filtering easier
    SortFile("../data/data_unsorted.pldat", "../data/data_sorted.pldat");
}
