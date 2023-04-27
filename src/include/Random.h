#pragma once

#include <random>

using namespace std;

class Random {
    static mt19937 random;
public:
    static int Int(int min, int max);
};