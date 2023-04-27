#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap(key_t maxKey)
{
    min = nullptr;
    totalNodes = 0;
    this->maxKey = maxKey;
    keyNodeMap = std::vector<Node *>(maxKey, nullptr);
}

FibonacciHeap::~FibonacciHeap()
{
    Node *nextNode;
    while ( min )
    {
        nextNode = RemoveNode(min);
        delete min;
        min = nextNode;
    }
}

FibonacciHeap::Node::~Node()
{
    Node *nextNode;
    while ( children )
    {
        nextNode = RemoveNode(children);
        delete children;
        children = nextNode;
    }
#ifdef FIBHEAP_DEBUG_ALLOC
    printf("Deleting Node %d\n", key);
#endif
}

bool FibonacciHeap::IsEmpty()
{
    return totalNodes == 0;
}
void FibonacciHeap::Insert(key_t key, uint32_t priority)
{
    if ( min == nullptr )
    {
        min = new Node(key, priority);
        totalNodes++;
        keyNodeMap[key] = min;
        return;
    }

    if ( !(keyNodeMap[key] == nullptr) )
    {
#ifdef FIBHEAP_DEBUG
        printf("%u already exists in fib heap\n", key);
#endif
        return;
    }
    totalNodes++;

    Node *newNode = new Node(key, priority);
    keyNodeMap[key] = newNode;

    // insert into root list
    min = MergeTrees(newNode, min);
}

FibonacciHeap::Node *FibonacciHeap::MergeTrees(Node *root1, Node *root2)
{
    /*
    both nodes are the minimum node in a tree, so are part of a valid circularly linked list
    this merge can be thought of as
    root1, root1->next, ... ->root1->prev
    and
    root2, root2->next, ... ->root2->prev
    they are then placed end to end:
    root1, root1->next, ... ->root1->prev, root2, root2->next, ... ->root2->prev
    by updating root1->prev->next and root2->prev
    then root1->prev and the original root2->prev are connected to restore circularity
    */

    // save the previous node in liked list
    Node *root2Last = root2->prev;

    root1->prev->next = root2;
    root2->prev = root1->prev;

    root1->prev = root2Last;
    root2Last->next = root1;

    return (root1->priority <= root2->priority ? root1 : root2);
}

key_t FibonacciHeap::PeekMin()
{
    return min->key;
}

key_t FibonacciHeap::ExtractMin()
{
    key_t retVal = min->key;
    Node *minFirstChild = min->children;
    Node *currChild = minFirstChild;
    Node *minMinChild = minFirstChild;

    if ( !(min->children == nullptr) )
    {
        do
        {
            // the child is now a root
            currChild->parent = nullptr;
            if ( currChild->priority < minMinChild->priority )
            {
                minMinChild = currChild;
            }
            currChild = currChild->next;
        } while ( currChild != minFirstChild );
    }

    Node *tempMin = RemoveNode(min);
    totalNodes--;
    keyNodeMap[min->key] = nullptr;
    delete min;
    min = tempMin;

    MeldRootList();
    return retVal;
}

void FibonacciHeap::PrintRoots()
{
    PrintRootList(min);
}
void FibonacciHeap::PrintRootList(Node *root)
{
    Node *currNode = root;
#ifdef FIBHEAP_DEBUG
    do
    {
        printf("Key: %u, Priority: %u, Degree: %d\n", currNode->key, currNode->priority, currNode->degree);
        currNode = currNode->next;
    } while ( currNode != root );
#endif
}

FibonacciHeap::Node *FibonacciHeap::RemoveNode(Node *root)
{
    if ( root->next == root )
    {
        return nullptr;
    }
    Node *leftNode = root->prev;
    leftNode->next = root->next;
    leftNode->next->prev = leftNode;
    root->prev = root;
    root->next = root;

    return leftNode;
}

void FibonacciHeap::MeldRootList()
{
    std::vector<Node *> newRoots;
    int newRootsSize = newRoots.size(); // cache size of vector to avoid 2 size calls for every iteration

    Node *remainingNodes = min;

    Node *currNode;

    int degree;
    while ( remainingNodes )
    {
        currNode = remainingNodes;

        remainingNodes = RemoveNode(currNode);

        degree = currNode->degree;

        // make sure there is space in the newRoots vector for this loop
        // newRoots must be at least degree
        if ( newRootsSize < degree )
        {
            newRoots.resize(degree, nullptr);
            newRootsSize = degree;
        }

        // if the last element isnt nullptr, it would be possible
        // for the last elemnt to me merged to the next degree, which
        // wouldn't have a space in the vector
        if ( newRootsSize == 0 || newRoots[newRootsSize - 1] != nullptr )
        {
            newRoots.resize(newRootsSize + 1, nullptr);
            newRootsSize++;
        }

        while ( newRoots[degree] != nullptr )
        {
            // the degree is already occupied, so merge the 2 nodes
            currNode = JoinTrees(currNode, newRoots[degree]);

            newRoots[degree] = nullptr;
            degree++;
        }

        // newRoot has the root of a new node and its degree is free

        newRoots[degree] = currNode;
    }

    min = nullptr;
    for ( size_t i = 0; i < newRoots.size(); i++ )
    {
        if ( newRoots[i] != nullptr )
        {
            if ( min == nullptr )
            {
                min = newRoots[i];
            }
            else
            {
                // printf("Merging (%u,%u) and (%u,%u)", min->key, min->priority, newRoots[i]->key, newRoots[i]->priority);
                min = MergeTrees(min, newRoots[i]);
                // printf(". Result: (%u,%u)\n", min->key, min->priority);
            }
        }
    }
}

FibonacciHeap::Node *FibonacciHeap::JoinTrees(Node *root1, Node *root2)
{
    // the code assumes root 1 is the min, so swap if its not
    Node *temp;
    if ( root2->priority < root1->priority )
    {
        temp = root2;
        root2 = root1;
        root1 = temp;
    }

    if ( root1->degree == 0 )
    {
        root1->children = root2;
    }
    else
    {
        MergeTrees(root1->children, root2);
    }
    root2->parent = root1;
    root1->degree++;
    return root1;
}

void FibonacciHeap::PrintHeap()
{
#ifdef FIBHEAP_DEBUG
    printf("Min: (%u,%u)\n", min->key, min->priority);
    Node *currNode = min;
    do
    {
        PrintTree(currNode, 0);
        std::cout << std::endl;
        currNode = currNode->next;
    } while ( currNode != min );
#endif
}

void FibonacciHeap::PrintTree(Node *root, int padding)
{
#ifdef FIBHEAP_DEBUG

    Node *currChild = root->children;
#define PADDING_PER_LEVEL 27
    int parentKey = padding == 0 ? -1 : root->parent->key;
    // "(xxx, xxx, xxx, xxx, x):   " characters added to padding each time
    std::cout << "(" << std::setw(3) << root->key << ", " << std::setw(3) << root->priority << ", " << std::setw(3) << root->degree << ", " << std::setw(3) << parentKey << ", " << std::noboolalpha << root->marked << "):   ";
    if ( root->children != nullptr )
    {
        while ( true )
        {
            PrintTree(currChild, padding + PADDING_PER_LEVEL);
            if ( currChild->next == root->children )
            {
                break;
            }
            else
            {
                // clang-format off
                std::cout << std::endl << std::setw(padding + PADDING_PER_LEVEL) << " ";
                currChild = currChild->next;
                // clang-format on
            }
        }
    }
#endif
}

bool FibonacciHeap::DecreaseKey(key_t key, uint32_t newPriority)
{
    Node *keyNode = keyNodeMap[key];
    if ( keyNode == nullptr )
    {
        Insert(key, newPriority);
        return true;
    }

    if ( newPriority > keyNode->priority )
    {
        return false;
    }

    keyNode->priority = newPriority;

    // keyNode is a root node, no cutting or marking needed, just check if its the new min
    if ( keyNode->parent == nullptr )
    {
        if ( keyNode->priority < min->priority )
        {
            min = keyNode;
        }
        return true;
    }

    // heap property is still intact, just no cutting needed, just return
    if ( keyNode->parent->priority < keyNode->priority )
    {
        return true;
    }

    Cut(keyNode);
    return true;
}

void FibonacciHeap::Cut(Node *cutNode)
{
    // this will never be the first val, only seen on
    // recursive calls, so no need to check for new minimum
    if ( cutNode->parent == nullptr )
    {
        cutNode->marked = false;
        return;
    }

    Node *parent = cutNode->parent;

    // parent not marked, all we need to do is cut it out
    Node *child = RemoveNode(cutNode);
    if ( parent->children == cutNode )
    {
        parent->children = child;
    }
    cutNode->parent = nullptr;
    min = MergeTrees(cutNode, min);
    cutNode->marked = false;
    parent->degree--;
    if ( parent->marked )
    {
        Cut(parent);
    }
    else if ( parent->parent )
    {
        parent->marked = true;
    }
}