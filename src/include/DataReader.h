#pragma once

#include "entries.h"

#include <fstream>
#include <string>
#include <sys/stat.h>

#define READABILITY_UNTESTED -1
#define READABILITY_TRUE 1
#define READABILITY_FALSE 0

using namespace std;
class DataReader
{
public:
    DataReader(string file);
    ~DataReader();
    Entry NextEntry();
    unsigned int RemainingEntries();
    bool IsEmpty();
    bool IsValid();

private:
    static bool CheckReadability();
    unsigned int numEntries;
    unsigned int numRead;

    ifstream inStream;

    bool valid;

    static const string testFilePath;
};