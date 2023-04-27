#include <ctime>

#include "Random.h"

// static variables must be redeclared in global space
mt19937 Random::random(time(0));

int Random::Int(int min, int max) {
    uniform_int_distribution<int> dist(min,max);

    return dist(random);
}