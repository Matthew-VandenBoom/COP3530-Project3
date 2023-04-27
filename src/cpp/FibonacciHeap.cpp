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
    min->children = nullptr;
    delete min;
    min = tempMin;
#ifdef FIBHEAP_DEBUG
    printf("min: %p\n", min);
#endif
    MeldRootList();
#ifdef FIBHEAP_DEBUG
    // printf("finished melding\n");
#endif
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
#ifdef FIBHEAP_DEBUG_VERIFY_CIRCULARITY
    printf("verifying circularity\n");
    bool verify = VerifyCircularity(min);
    printf("Circularity: %d\n", verify);
#endif
    std::vector<Node *> newRoots;
    int newRootsSize = newRoots.size(); // cache size of vector to avoid 2 size calls for every iteration

    Node *remainingNodes = min;

    Node *currNode;

    int degree;
    // printf("starting loop, initial remaining nodes: %p\n", remainingNodes);
    while ( remainingNodes )
    {
        currNode = remainingNodes;
        // printf("looping with %p\n", currNode);

        remainingNodes = RemoveNode(currNode);

        degree = currNode->degree;
        // printf("Node removed, remaining nodes %p, degree %d\n", remainingNodes, degree);
        //  make sure there is space in the newRoots vector for this loop
        //  newRoots must be at least degree
        // printf("verifying size 1: %d, %d\n", newRootsSize, (int)newRoots.size());
        if ( newRootsSize < (degree + 1) )
        {
            // printf("resizing 1\n");
            newRoots.resize(degree + 1, nullptr);
            newRootsSize = degree + 1;
        }
        // printf("size 1 verified %d\n", newRootsSize);

        // if the last element isnt nullptr, it would be possible
        // for the last elemnt to me merged to the next degree, which
        // wouldn't have a space in the vector
        // printf("verifying size 2\n");
        if ( newRootsSize == 0 || newRoots[newRootsSize - 1] != nullptr )
        {
            // printf("resizing 2: %d\n", newRootsSize);
            newRoots.resize(newRootsSize + 1, nullptr);
            newRootsSize++;
        }
        // printf("size 2 verified:  %d, %d\n", newRootsSize, (int)newRoots.size());

        // printf("starting merging loop\n");
        //  if ( degree >= newRootsSize )
        //  {
        //      printf("DEGREE %d IS TOO LARGE FOR VECTOR %d, %d\n", degree, (int)newRoots.size(), newRootsSize);
        //  }
        while ( newRoots[degree] != nullptr )
        {
            // if ( degree >= newRootsSize )
            // {
            //     printf("DEGREE %d IS TOO LARGE FOR VECTOR %d, %d\n", degree, (int)newRoots.size(), newRootsSize);
            // }
            // the degree is already occupied, so merge the 2 nodes
            // printf("degree: %d, old degrees %d, %d\n", degree, currNode->degree, newRoots[degree]->degree);
            currNode = JoinTrees(currNode, newRoots[degree]);
            // printf("joined the trees\n");

            newRoots[degree] = nullptr;
            degree++;
            // printf("starting next iteration: degree = %d\n", degree);
        }
        // printf("ended merging loop\n");

        // newRoot has the root of a new node and its degree is free

        // printf("degree == currNode.degree: %d\n", (degree == currNode->degree));
        newRoots[degree] = currNode;
        // printf("updated newRoots\n");
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
#ifdef FIBHEAP_DEBUG
    printf("decreasing %d\n", key);
#endif
    Node *keyNode = keyNodeMap[key];
    if ( keyNode == nullptr )
    {
#ifdef FIBHEAP_DEBUG
        printf("decreasing new key, inserting\n");
#endif
        Insert(key, newPriority);
        return true;
    }

    if ( newPriority > keyNode->priority )
    {
#ifdef FIBHEAP_DEBUG
        printf("already lower, not decreasing\n");
#endif
        return false;
    }

    keyNode->priority = newPriority;

    // keyNode is a root node, no cutting or marking needed, just check if its the new min
    if ( keyNode->parent == nullptr )
    {
#ifdef FIBHEAP_DEBUG
        printf("Decreasing a root node, checking if its new min\n");
#endif
        if ( keyNode->priority < min->priority )
        {
#ifdef FIBHEAP_DEBUG
            printf("    New Min found!\n");
#endif
            min = keyNode;
        }
        return true;
    }

    // heap property is still intact, just no cutting needed, just return
    if ( keyNode->parent->priority < keyNode->priority )
    {
#ifdef FIBHEAP_DEBUG
        printf("Decreasing didn't violate heap property\n");
#endif
        return true;
    }

#ifdef FIBHEAP_DEBUG
    printf("cutting\n");
#endif
    Cut(keyNode);
#ifdef FIBHEAP_DEBUG
    printf("finished cutting\n");
#endif
    return true;
}

void FibonacciHeap::Cut(Node *cutNode)
{
    static bool firstcut = true;
    if ( firstcut )
    {
        printf("FIRST CUT\n");
        firstcut = false;
    }
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

#ifdef FIBHEAP_DEBUG_VERIFY_CIRCULARITY
bool FibonacciHeap::VerifyCircularity(Node *root)
{
    bool r = true;
    int i = 0;
    Node *curr = root;
    if ( curr == nullptr )
    {
        printf("Not circular, null ptr");
        return false;
    }
    while ( curr->next != root )
    {
        if ( curr == nullptr )
        {
            r = false;
            break;
        }
        i++;
        if ( i % 10 == 0 )
        {
            printf("%d, ", i);
        }
        curr = curr->next;
    }
    printf("\nTotal length: %d\n", i);
    return r;
}

#endif