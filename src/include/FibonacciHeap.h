

class FibonacciHeap
{
public:
    FibonacciHeap();
    void Insert();
    int ExtractMin();
    int PeekMin();
    void DecreaseKey();

private:
    struct Node
    {
        Node *next;
        Node *prev;
        bool marked;
    };
    Node *root;
};