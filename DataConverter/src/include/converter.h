#pragma once

#include <entries.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

using namespace std;

bool convertFiles(vector<string> inputFiles, string outputFile);
bool convertFile(string inputFile, string outputFile);

bool SortFile(string inputFile, string outputFile);

void SetBaseTime(vector<string> inputFiles);
void SetBaseTime(time_t t, int ms);