#pragma once

#include <cstdint>

typedef int key_t;

class BinaryHeap
{
public:
    BinaryHeap(key_t maxKey);
    ~BinaryHeap();
    void Insert(key_t key, uint32_t priority);
    key_t ExtractMin();
    key_t PeekMin();
    bool IsEmpty();

    // if key isnt in the heap, create it
    // otherwise update the key if newPriority is < old priority
    // returns whether the key's priority was updated
    bool DecreaseKey(key_t key, uint32_t newPriority);

private:
};