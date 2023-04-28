#include "BinaryHeap.h"

// macros for parent and chld access to
// avoid retyping them without the overhead of function calls every time
#define PARENT(x) (x / 2)
#define LCHILD(x) (2 * x)
#define RCHILD(x) ((2 * x) + 1)

#include <iostream>

#define BINHEAP_DEBUG
#ifdef BINHEAP_DEBUG
#endif

BinaryHeap::BinaryHeap(key_t maxKey)
{
    heap = std::vector<HeapEntry>();
    heap.push_back(HeapEntry(-1, 0xFFFFFFFF)); // dummy entry becase this heap is 1 based
    keyNodeMap = std::vector<int>(maxKey, -1);
}
BinaryHeap::~BinaryHeap() {}
void BinaryHeap::Insert(key_t key, uint32_t priority)
{
#ifdef BINHEAP_DEBUG
    if ( keyNodeMap[key] != -1 )
    {
        printf("%d is already in the heap\n", key);
    }
#endif

    int insertIndex = heap.size();
    heap.push_back(HeapEntry(key, priority));
    keyNodeMap[key] = insertIndex;
    HeapifyUp(insertIndex);
}
key_t BinaryHeap::ExtractMin()
{
#ifdef BINHEAP_DEBUG
    if ( heap.size() == 1 )
    {
        printf("Heap is empty\n");
        return -1;
    }
#endif
    Swap(1, heap.size() - 1);

    HeapEntry min = heap.back();
    heap.pop_back();            // remove the actual element from the heap
    keyNodeMap[min.first] = -1; // remove it from the map

    HeapifyDown(1);
    return min.first;
}
key_t BinaryHeap::PeekMin()
{
    return heap[1].first;
}
bool BinaryHeap::IsEmpty()
{
    // 1 is empty bc of the extra entry at 0
    return heap.size() == 1;
}

bool BinaryHeap::DecreaseKey(key_t key, uint32_t newPriority)
{
    int keyIndex = keyNodeMap[key];
    if ( keyIndex == -1 )
    {
        // this key isn't in the heap, add it
        Insert(key, newPriority);
        return true;
    }

    if ( heap[keyIndex].second < newPriority )
    {
        // existing priority is lower
        return false;
    }

    heap[keyIndex].second = newPriority;
    HeapifyUp(keyIndex);
    return true;
}

void BinaryHeap::Swap(int E1, int E2)
{
    // swap the elements themselvs
    HeapEntry tempEntry;
    tempEntry = heap[E1];
    heap[E1] = heap[E2];
    heap[E2] = tempEntry;

    keyNodeMap[heap[E1].first] = E1;
    keyNodeMap[heap[E2].first] = E2;
}

void BinaryHeap::HeapifyUp(int index)
{
    int parent = PARENT(index);
    while ( (parent > 0) && heap[parent].second > heap[index].second )
    {
        Swap(parent, index);
        index = parent;
        parent = PARENT(index);
    }
}
void BinaryHeap::HeapifyDown(int index)
{
    int size = heap.size();
    int lChild = LCHILD(index);
    int rChild = RCHILD(index);
    while ( rChild < size )
    {
        // both children are valid if the right child is valid
        if ( heap[lChild].second < heap[rChild].second )
        {
            if ( heap[lChild].second < heap[index].second )
            {
                Swap(lChild, index);
                index = lChild;
            }
            else
            {
                break;
            }
        }
        else
        {
            if ( heap[rChild].second < heap[index].second )
            {
                Swap(rChild, index);
                index = rChild;
            }
            else
            {
                break;
            }
        }
        lChild = LCHILD(index);
        rChild = RCHILD(index);
    }

    // left child may get skipped on the last rotation if it doesnt have a right sibling
    if ( lChild < size && heap[lChild].second < heap[index].second )
    {
        Swap(lChild, index);
    }
}