#pragma once

#include <cstdint>

#include <vector>

typedef int key_t;

using HeapEntry = std::pair<key_t, uint32_t>;
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
    std::vector<HeapEntry> heap;

    // contains map from keys to their entry in the heap
    // for O(1) access to allow for decrease key
    // a value of -1 means the item isnt in the heap
    // a non negative value means the item is at that index in the heap array
    std::vector<int> keyNodeMap;

    void Swap(int E1, int E2);

    void HeapifyUp(int index);
    void HeapifyDown(int index);
};