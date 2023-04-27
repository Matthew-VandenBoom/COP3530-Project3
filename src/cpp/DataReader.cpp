#include "DataReader.h"

#include <unordered_set>

const string DataReader::testFilePath = "../data/testsorted.pldat";

DataReader::DataReader(string file)
{
    if ( !CheckReadability() )
    {
        printf("Ability to Read Data not verified, Not Creating data reader\n");
        valid = false;
        return;
    }
    inStream.open(file, std::ios::binary);
    if ( !inStream.is_open() )
    {
        printf("Opening file failed");
        valid = false;
        return;
    }

    inStream.read((char *)(&numEntries), sizeof(numEntries));

    struct stat64 fileStats;
    stat64(file.c_str(), &fileStats);
    uint64_t arrayBytes = numEntries * sizeof(PackedEntry);
    uint64_t fileSize = sizeof(numEntries) + arrayBytes;

    if ( fileSize != ((uint64_t)fileStats.st_size) )
    {
        printf("Incorect Filesize for number of entries.\n");
        printf("Entry Count: %u\n", numEntries);
        printf("Calculated filesize: %llu\n", fileSize);
        printf("True filesize: %lld\n", (fileStats.st_size));
        inStream.close();
        valid = false;
        return;
    }

    valid = true;
    numRead = 0;
}

DataReader::~DataReader()
{
    if ( inStream.is_open() )
    {
        inStream.close();
    }
}

bool DataReader::IsEmpty()
{
    if ( !valid )
    {
        return true;
    }
    return RemainingEntries() == 0;
}

bool DataReader::IsValid()
{
    return valid;
}

unsigned int DataReader::RemainingEntries()
{
    if ( !valid )
    {
        return 0;
    }
    return numEntries - numRead;
}

Entry DataReader::NextEntry()
{
    if ( !valid || IsEmpty() )
    {
        printf("Returning Blank Entry");
        return Entry();
    }
    PackedEntry e;
    inStream.read((char *)(&e), sizeof(e));
    numRead++;
    return Entry(&e);
}

static const uint32_t testNumEntries = 4;
static const uint32_t testTimes[] = {0, 1, 2, 3};
static const uint32_t testUsers[] = {1, 3, 0, 2};
static const uint8_t testReds[] = {0x11, 0x77, 0xAA, 0x44};
static const uint8_t testGreens[] = {0x22, 0x88, 0xFF, 0x55};
static const uint8_t testBlues[] = {0x33, 0x99, 0xBB, 0x66};
static const uint16_t testXs[] = {0, 1900, 0, 18};
static const uint16_t testYs[] = {199, 498, 1999, 22};
bool DataReader::CheckReadability()
{
    static int readability = READABILITY_UNTESTED;
    if ( readability == READABILITY_TRUE )
    {
        return true;
    }
    if ( readability == READABILITY_FALSE )
    {
        return false;
    }

    ifstream testFile;
    testFile.open(testFilePath);
    if ( !testFile.is_open() )
    {
        readability = READABILITY_FALSE;
        printf("Reader self test error:\nfile \"%s\" couldn't be opened\n", testFilePath.c_str());
        return false;
    }

    struct stat64 fileStats;
    stat64(testFilePath.c_str(), &fileStats);
    uint64_t arrayBytes = testNumEntries * sizeof(PackedEntry);
    uint64_t fileSize = sizeof(testNumEntries) + arrayBytes;

    if ( fileSize != ((uint64_t)fileStats.st_size) )
    {
        printf("Reader self test error:\n");
        printf("Incorect Filesize for number of entries.\n");
        printf("Expected Entry Count: %u\n", testNumEntries);
        printf("Calculated filesize: %llu\n", fileSize);
        printf("True filesize: %lld\n", (fileStats.st_size));
        printf("Possible cause: PackedEntry has a different size on this computer\n");

        testFile.close();
        readability = READABILITY_FALSE;
        return false;
    }

    readability = READABILITY_TRUE;
    uint32_t numEntries = 0;
    PackedEntry testPackedEntry;
    Entry testEntry;

    testFile.read((char *)(&numEntries), sizeof(numEntries));
    if ( numEntries != testNumEntries )
    {
        printf("Reader self test error:\n");
        printf("Actual entry count (%u) != Expected entry count (%u)\n", numEntries, testNumEntries);
        printf("Possible cause: test file was created on a computer with different endianness\n");
        readability = READABILITY_FALSE;
        return false;
    }

    readability = READABILITY_TRUE;

    for ( uint32_t i = 0; i < testNumEntries; i++ )
    {
        testFile.read((char *)(&testPackedEntry), sizeof(testPackedEntry));
        testEntry = Entry(&testPackedEntry);
        if ( testEntry.time != testTimes[i] )
        {
            printf("Entry %u time doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
        if ( testEntry.user != testUsers[i] )
        {
            printf("Entry %u user doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
        if ( testEntry.r != testReds[i] )
        {
            printf("Entry %u red doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
        if ( testEntry.g != testGreens[i] )
        {
            printf("Entry %u green doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
        if ( testEntry.b != testBlues[i] )
        {
            printf("Entry %u blue doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
        if ( testEntry.x != testXs[i] )
        {
            printf("Entry %u X doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
        if ( testEntry.y != testYs[i] )
        {
            printf("Entry %u Y doesnt match test value\n", i);
            readability = READABILITY_FALSE;
        }
    }

    return CheckReadability();
}

DataInfo DataReader::FileInfo(string file)
{
    DataInfo info;
    DataReader data(file);
    Entry e;
    std::unordered_set<uint32_t> seenUsers;
    uint32_t maxUser = 0;
    uint32_t maxTime = 0;
    if ( !data.IsValid() )
    {
        printf("File %s isnt valid\n", file.c_str());
    }
    info.numEntries = data.RemainingEntries();

    while ( !data.IsEmpty() )
    {
        e = data.NextEntry();
        seenUsers.insert(e.user);
        if ( e.user > maxUser )
        {
            maxUser = e.user;
        }
        if ( e.time > maxTime )
        {
            maxTime = e.time;
        }
    }
    info.maxTime = maxTime;
    info.maxUser = maxUser;
    info.allUsers = true;
    printf("Checking for all users\n");
    for ( uint32_t user = 0; user <= maxUser; user++ )
    {
        if ( seenUsers.find(user) == seenUsers.end() )
        {
            printf("User %u missing\n", user);
            info.allUsers = false;
        }
    }
    return info;
}