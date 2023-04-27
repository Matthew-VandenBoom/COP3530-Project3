#pragma once

#include "entries.h"

#include <fstream>
#include <string>
#include <sys/stat.h>

#define READABILITY_UNTESTED -1
#define READABILITY_TRUE 1
#define READABILITY_FALSE 0

#define ENTRY_BUFFER_SIZE 1000

using namespace std;

struct DataInfo
{
    int maxUser;
    bool allUsers; // all users 0 through maxUser are present
    unsigned int numEntries;
    unsigned int maxTime;
};
class DataReader
{
public:
    DataReader(string file);
    ~DataReader();
    Entry NextEntry();
    unsigned int RemainingEntries();
    bool IsEmpty();
    bool IsValid();

    static DataInfo FileInfo(string file);

private:
    static bool CheckReadability();
    unsigned int numEntries;
    unsigned int numRead;

    char *streambuf;
    ifstream inStream;

    bool valid;
    bool empty;

    static const string testFilePath;

    PackedEntry entryBuffer[ENTRY_BUFFER_SIZE];
    int entryBufferPTR;
};