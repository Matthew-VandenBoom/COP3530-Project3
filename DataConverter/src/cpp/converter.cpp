#include "converter.h"

#include "errno.h"

static unordered_map<string, uint32_t> userHashToIDTable;
static bool baseTimeValid = false;
static time_t baseTime;
static int baseTimeMS;

static bool findBaseTime(time_t *t, int *ms, ifstream &inputFile)
{
    int64_t time;
    tm rowTime{};
    rowTime.tm_isdst = -1;
    time_t timeEpoch;
    int timeMS;
    istringstream timeSS;
    double timediff;

    string timeString;
    string unused;

    bool valid = false;
    while ( !inputFile.eof() )
    {
        std::getline(inputFile, timeString, ',');
        std::getline(inputFile, unused, '\n');
        /*
        this will set the eof bit if at the end of the file, otherwise the loop wont terminate
        when it is at the end and will do one more iteration, which may have unintended results
        this is because the eof bit is only set when trying to read the end of the file, and even
        if the last getline put the seekpoint of the stream on the last line, where there are no
        more characters to read, it hasnt attempted to read yet
        */
        inputFile.peek();

        timeSS.clear();
        timeSS.str(timeString);
        timeSS >> get_time(&rowTime, "%Y-%m-%d %H:%M:%S");
        if ( timeSS.fail() )
        {
            printf("Time conversion Failed\n");
        }
        timeSS.ignore(1);
        timeSS >> timeMS;
        timeEpoch = mktime(&rowTime);
        if ( !valid )
        {
            // printf("Setting base time:  %ld + %d ms\n", timeEpoch, timeMS);

            valid = true;
            (*t) = timeEpoch;
            (*ms) = timeMS;
        }
        timediff = difftime(timeEpoch, (*t));
        time = ((int64_t)timediff * 1000) + (int64_t)timeMS - (int64_t)(*ms);
        if ( time < 0 )
        {
            // printf("New lowest found: %ld + %d ms < %ld + %d ms\n", timeEpoch, timeMS, *t, *ms);
            *t = timeEpoch;
            *ms = timeMS;
        }
    }
    return true;
}

uint32_t userHashToID(string userHash)
{
    auto IDPair = userHashToIDTable.insert(make_pair(userHash, (uint32_t)(userHashToIDTable.size())));
    return IDPair.first->second;
}

/// @brief Converts rows from input file to packed format and then writes them to output file
/// @param inputFile filestream for the input file. This must be aligned to the start of a line of the file. It will convert and append this line and all following lines
/// @param outputFile output filestream to write to. This should be the end of the file, otherwise data will be overwritten
/// @param numEntries used to return the total number of entries written to outputFile
/// @return false on some sort of failure, true otherwise
static bool convertFileInternal(ifstream &inputFile, ofstream &outputFile, uint32_t *numEntries)
{
    PackedEntry *e = (PackedEntry *)malloc(sizeof(PackedEntry));

    int64_t time;
    uint32_t user;
    uint8_t r, g, b;
    uint16_t x, y;

    string timeString;
    string userHash;
    string colorCode;
    string coordString;

    // temporaries for converting the time
    tm rowTime{};
    rowTime.tm_isdst = -1;
    time_t timeEpoch;
    int timeMS;
    istringstream timeSS;
    double timediff;

    // temporaries for converting cords
    size_t coordsComma;
    if ( !baseTimeValid )
    {
        printf("Base time must be set before converting files\n");
        return false;
    }
    while ( !inputFile.eof() )
    {
        std::getline(inputFile, timeString, ',');
        std::getline(inputFile, userHash, ',');
        std::getline(inputFile, colorCode, ',');
        std::getline(inputFile, coordString, '\n');
        /*
        this will set the eof bit if at the end of the file, otherwise the loop wont terminate
        when it is at the end and will do one more iteration, which may have unintended results
        this is because the eof bit is only set when trying to read the end of the file, and even
        if the last getline put the seekpoint of the stream on the last line, where there are no
        more characters to read, it hasnt attempted to read yet
        */
        inputFile.peek();

        timeSS.clear();
        timeSS.str(timeString);
        timeSS >> get_time(&rowTime, "%Y-%m-%d %H:%M:%S");
        if ( timeSS.fail() )
        {
            printf("Time conversion Failed\n");
        }
        timeSS.ignore(1);
        timeSS >> timeMS;
        timeEpoch = mktime(&rowTime);
        timediff = difftime(timeEpoch, baseTime);
        time = ((int64_t)timediff * 1000) + (int64_t)timeMS - (int64_t)baseTimeMS;
        if ( time < 0 )
        {
            printf("Time below base time on line with color: %s and coord: %s\n", colorCode.c_str(), coordString.c_str());
            return false;
        }
        user = userHashToID(userHash);

        r = (uint8_t)stoul(colorCode.substr(1, 2), nullptr, 16);
        g = (uint8_t)stoul(colorCode.substr(3, 2), nullptr, 16);
        b = (uint8_t)stoul(colorCode.substr(5, 2), nullptr, 16);

        coordString = coordString.substr(1, coordString.length() - 2);
        coordsComma = coordString.find(',');
        if ( coordsComma != coordString.rfind(',') )
        {
            printf("Time: %lld ms Time(converted) %u\nUser: %u\nColors: R:%x, G:%x, B:%x\nCoord: %s\n", time, (uint32_t)time, user, r, g, b, coordString.c_str());
            printf("moderator rectangle, skipping\n");
            continue;
        }
        x = stoul(coordString.substr(0, coordsComma), nullptr, 10);
        y = stoul(coordString.substr(coordsComma + 1), nullptr, 10);

        writeEntry((uint32_t)time, user, r, g, b, x, y, e);
        outputFile.write((char *)e, sizeof(PackedEntry));
        (*numEntries)++;
    }

    free(e);
    return true;
}

bool convertFile(string inputFile, string outputFile)
{
    uint32_t numEntries = 0;
    ifstream inputStream;
    ofstream outputStream;

    inputStream.open(inputFile);
    outputStream.open(outputFile, std::ios::binary | std::ios::trunc);
    if ( !inputStream.is_open() )
    {
        printf("failed to open input file: %s", inputFile.c_str());
        return false;
    }
    if ( !outputStream.is_open() )
    {
        printf("failed to open output file: %s", outputFile.c_str());
        return false;
    }

    // ignore the headers of the csv
    inputStream.ignore(200, '\n');
    // leave space for the number of entries before writing to the file
    outputStream.write((char *)(&numEntries), sizeof(numEntries));
    bool success = convertFileInternal(inputStream, outputStream, &numEntries);
    printf("Writing finished, %u entries written", numEntries);
    if ( success )
    {
        outputStream.seekp(0, std::ios::beg);
        outputStream.write((char *)(&numEntries), sizeof(numEntries));
    }
    else
    {
        printf("Something went wrong when writing");
    }
    outputStream.close();
    inputStream.close();
    return success;
}
bool convertFiles(vector<string> inputFiles, string outputFile)
{
    uint32_t totalEntries = 0;
    uint32_t numEntries = 0;
    ifstream inputStream;
    ofstream outputStream;

    string inputFile;
    bool success;

    outputStream.open(outputFile, std::ios::binary | std::ios::trunc);
    if ( !outputStream.is_open() )
    {
        printf("failed to open output file: %s", outputFile.c_str());
        return false;
    }
    outputStream.write((char *)(&totalEntries), sizeof(totalEntries));
    for ( size_t i = 0; i < inputFiles.size(); i++ )
    {
        numEntries = 0;
        inputFile = inputFiles[i];
        inputStream.open(inputFile);
        if ( !inputStream.is_open() )
        {
            printf("failed to open input file: %s", inputFile.c_str());
            return false;
        }
        // ignore the headers of the csv
        inputStream.ignore(200, '\n');

        success = convertFileInternal(inputStream, outputStream, &numEntries);
        printf("Conversion finished for %s, %u entries written\n", inputFile.c_str(), numEntries);
        if ( success )
        {
            totalEntries += numEntries;
            outputStream.seekp(0, std::ios::beg);
            outputStream.write((char *)(&totalEntries), sizeof(totalEntries));
            outputStream.seekp(0, std::ios::end);
        }
        else
        {
            printf("Something went wrong when writing");
            return false;
        }
        inputStream.close();
    }
    outputStream.close();

    return true;
}

void SetBaseTime(vector<string> inputFiles)
{
    time_t lowestTime, fileTime;
    string lowestFile;
    int lowestMS, fileMS;
    ifstream inputStream;
    string inputFile;

    // used for comparing the times
    double dTimeComp;
    int64_t itimeComp;
    for ( size_t i = 0; i < inputFiles.size(); i++ )
    {
        inputFile = inputFiles[i];
        inputStream.open(inputFile);
        if ( !inputStream.is_open() )
        {
            printf("failed to open input file: %s", inputFile.c_str());
            return;
        }
        // ignore the headers of the csv
        inputStream.ignore(200, '\n');
        findBaseTime(&fileTime, &fileMS, inputStream);
        printf("%s finished, best: %lld + %d ms\n", inputFile.c_str(), fileTime, fileMS);
        inputStream.close();
        if ( i == 0 )
        {
            lowestTime = fileTime;
            lowestMS = fileMS;
            lowestFile = inputFile;
        }
        else
        {
            dTimeComp = difftime(fileTime, lowestTime);
            itimeComp = ((int64_t)dTimeComp * 1000) + (int64_t)fileMS - (int64_t)(lowestMS);
            if ( itimeComp < 0 )
            {
                printf("New lowest found: %lld + %d ms < %lld + %d ms\n", fileTime, fileMS, lowestTime, lowestMS);
                lowestTime = fileTime;
                lowestMS = fileMS;
                lowestFile = inputFile;
            }
        }
    }
    printf("Lowest overall: %lld + %d ms in %s\n", lowestTime, lowestMS, lowestFile.c_str());
    SetBaseTime(lowestTime, lowestMS);
}
void SetBaseTime(time_t t, int ms)
{
    baseTime = t;
    baseTimeMS = ms;
    baseTimeValid = true;
}

bool SortFile(string inputFile, string outputFile)
{
    ifstream inStream;
    ofstream outStream;

    inStream.open(inputFile, std::ios::binary);
    if ( !inStream.is_open() )
    {
        printf("Input file could not be opened\n");
        return false;
    }

    uint32_t numEntries;
    inStream.read((char *)(&numEntries), sizeof(numEntries));
    printf("Num read: %lld\n", inStream.gcount());
    struct stat64 fileStats;
    stat64(inputFile.c_str(), &fileStats);
    uint64_t arrayBytes = numEntries * sizeof(PackedEntry);
    uint64_t fileSize = sizeof(numEntries) + arrayBytes;

    if ( fileSize != ((uint64_t)fileStats.st_size) )
    {
        printf("Incorect Filesize for number of entries.\n");
        printf("Entry Count: %u\n", numEntries);
        printf("Calculated filesize: %llu\n", fileSize);
        printf("True filesize: %lld\n", (fileStats.st_size));
        inStream.close();
        return false;
    }

    // the file is of the correct size for the number of entries.

    PackedEntry *array = (PackedEntry *)malloc(arrayBytes);

    if ( array == nullptr )
    {
        printf("Malloc failed to allocate array");

        inStream.close();
        return false;
    }

    // we can only read 2^31 -1 bytes at a time, because the number of bytes
    // to read is passed as a signed 32 bit int
    uint64_t bytesToRead = arrayBytes;
    uint64_t bytesRead = 0;
    while ( bytesRead != arrayBytes )
    {
        if ( bytesToRead > 0x7fffffff )
        {
            inStream.read(((char *)array) + bytesRead, 0x7fffffff);
            bytesRead += 0x7fffffff;
            bytesToRead -= 0x7fffffff;
        }
        else
        {
            inStream.read(((char *)array) + bytesRead, bytesToRead);
            bytesRead += bytesToRead;
            bytesToRead -= bytesToRead;
        }
        if ( !inStream.good() )
        {
            printf("bytestoread: %llu, bytesread: %llu", bytesToRead, bytesRead);
        }
    }
    // for ( uint64_t i = 0; i < numEntries; i += 1000000 )
    // {
    //     printf("\nEntry %llu\n", i);
    //     printPackedEntry(&(array[i]));
    // }
    // printf("\nEntry %lu\n", numEntries - 1);
    // printPackedEntry(&(array[numEntries - 1]));
    printf("sorting\n");
    std::sort(array, array + numEntries, [](PackedEntry a, PackedEntry b)
              { return a.time < b.time; });
    printf("finished sorting\n");
    // for ( uint32_t i = 0; i < numEntries - 1; i++ )
    // {
    //     if ( !(array[i].time <= array[i + 1].time) )
    //     {
    //         printf("%u: %u > %u\n", i, array[i].time, array[i + 1].time);
    //         break;
    //     }
    // }

    outStream.open(outputFile, std::ios::binary | std::ios::trunc);
    if ( !outStream.is_open() )
    {
        printf("Output file could not be opened\n");
        return false;
    }

    outStream.write((char *)(&numEntries), sizeof(numEntries));
    uint64_t bytesToWrite = arrayBytes;
    uint64_t bytesWritten = 0;
    while ( bytesWritten != arrayBytes )
    {
        if ( bytesToWrite > 0x7fffffff )
        {
            outStream.write(((char *)array) + bytesWritten, 0x7fffffff);
            bytesWritten += 0x7fffffff;
            bytesToWrite -= 0x7fffffff;
        }
        else
        {
            outStream.write(((char *)array) + bytesWritten, bytesToWrite);
            bytesWritten += bytesToWrite;
            bytesToWrite -= bytesToWrite;
        }
        if ( !outStream.good() )
        {
            printf("bytestowrite: %llu, byteswritten: %llu", bytesToWrite, bytesWritten);
        }
    }

    free(array);

    inStream.close();
    outStream.close();
    return true;
}