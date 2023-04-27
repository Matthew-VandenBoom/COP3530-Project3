#pragma once
#include <cstdint>
#include <vector>

#define FIBHEAP_DEBUG
#ifdef FIBHEAP_DEBUG
#include <iomanip>
#include <iostream>
#endif

typedef int key_t;
class FibonacciHeap
{
public:
    FibonacciHeap(key_t maxKey);
    ~FibonacciHeap();
    void Insert(key_t key, uint32_t priority);
    key_t ExtractMin();
    key_t PeekMin();
    bool IsEmpty();

    // if key isnt in the heap, create it
    // otherwise update the key if newPriority is < old priority
    // returns whether the key's priority was updated
    bool DecreaseKey(key_t key, uint32_t newPriority);

    void PrintRoots();
    void PrintHeap();

private:
    struct Node
    {
        // next and previous form a circularly linked list of nodes at the same level
        // if this node is a root, the list is all other root nodes
        // if this is a child nodes, the list is all sibling nodes
        // when constructing, initialize next and prev to "this", so that it is a valid circularly linked lists
        Node(key_t key, uint32_t priority) : next(this), prev(this), marked(false), parent(nullptr), children(nullptr), degree(0), key(key), priority(priority)
        {
#ifdef FIBHEAP_DEBUG_ALLOC
            printf("Creating  Node %d\n", key);
#endif
        }
        ~Node();
        Node *next;
        Node *prev;
        bool marked;

        Node *parent;
        Node *children;
        int degree;

        key_t key;
        uint32_t priority;
    };
    key_t maxKey;
    Node *min;
    int totalNodes = 0;
    std::vector<Node *> keyNodeMap;

    // merge 2 root lists to form a larger tree
    // this is mostly used to insert elements, or when extract min needs to put children of a node into the root list
    // both Nodes should be the minimum element of their respective circularly linked lists
    // Returns: Node* to the minimum node in the merged list, which is one of the given ones
    static Node *MergeTrees(Node *root1, Node *root2);

    // removes the Node node from the linked list it is a part of
    // returns a Node in the remaining linked list if the list is more than just node
    // if the list is just Node, return nullptr
    static Node *RemoveNode(Node *node);

    void MeldRootList();

    // Joins 2 trees of the same degree to give a
    // new tree with degree + 1
    // Returns pointer to root of the new tree,
    // which is the lower priority of the roots
    static Node *JoinTrees(Node *root1, Node *root2);

    static void PrintRootList(Node *root);
    static void PrintTree(Node *root, int padding);

    void Cut(Node *node);
};