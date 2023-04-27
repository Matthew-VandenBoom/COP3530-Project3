#include "graph.h"

#include "DataReader.h"

// clang-format off
#define I_2D(x, y) x + y * xWidth
// clang-format on

// maps each old userID to a newOne
// returns a pair with the new user Num
// and whether an insertion was preformed, which signals if this was a new user
static std::pair<int, bool> remapUserNum(int oldUserNum, std::unordered_map<int, int> *newUserNums)
{
    // insert will only add the value to the map if the key doesnt already
    // exist in the map. Regardless of if a value was inserted, it returns
    // the key and actual value in the map.
    auto IDPair = newUserNums->insert(make_pair(oldUserNum, (int)newUserNums->size()));

    return make_pair(IDPair.first->second, IDPair.second);
}

Graph::Graph(std::string file, int xMin, int yMin, int xMax, int yMax)
{
    totalNodes = 0;
    adjacency = std::vector<std::unordered_map<int, EdgeInfo>>();
    if ( xMin < 0 )
    {
        xMin = 0;
        printf("xMin out of bounds, correcting\n");
    }
    if ( yMin < 0 )
    {
        yMin = 0;
        printf("yMin out of bounds, correcting\n");
    }
    if ( xMax > 1999 )
    {
        xMax = 1999;
        printf("xMax out of bounds, correcting\n");
    }
    if ( yMax > 1999 )
    {
        yMax = 1999;
        printf("yMax out of bounds, correcting\n");
    }
    DataReader data(file);
    if ( !data.IsValid() )
    {
        printf("Graph couldn't read data\n");
        return;
    }
    int xWidth = xMax - xMin + 1;
    int yHeight = yMax - yMin + 1;

    int numPixels = xWidth * yHeight;

    Entry *prevAccesses = new Entry[numPixels];
    Entry empty = Entry();

    Entry currEntry;
    // for ( int y = 0; y < yHeight; y++ )
    // {
    //     for ( int x = 0; x < xWidth; x++ )
    //     {
    //         if ( prevAccesses[I_2D(x, y)] != empty )
    //         {
    //             printf("(%d, %d) Absolute (%d) not empty\n", x, y, I_2D(x, y));
    //         }
    //     }
    // }
    // printf("All entries checked\n");

    // Remap user numbers so that all user numbers are contiguous
    // the map used for this is stored here so that its memory will be freed when the constructor ends
    // instead of holding memory for the lifetime of the program, as it will take decent amount of memory
    // This also allows multiple graphs to be created without interfering with each other
    std::unordered_map<int, int> newUserNums;

    int adjustedX;
    int adjustedY;
    std::pair<int, bool> newUID;

    std::unordered_map<int, EdgeInfo>::iterator existingEdge;
    Entry *lastAccess;
    while ( !data.IsEmpty() )
    {
        currEntry = data.NextEntry();
        if ( (currEntry.x >= xMin) && (currEntry.x <= xMax) && (currEntry.y >= yMin) && (currEntry.y <= yMax) )
        {
            adjustedX = currEntry.x - xMin;
            adjustedY = currEntry.y - yMin;
            newUID = remapUserNum(currEntry.user, &newUserNums);
            if ( newUID.second )
            {
                // printf("New user found: %d -> %d\n", currEntry.user, newUID.first);
                totalNodes++;
                adjacency.push_back(std::unordered_map<int, EdgeInfo>());
            }
            currEntry.user = newUID.first;
            lastAccess = prevAccesses + I_2D(adjustedX, adjustedY);
            if ( (*lastAccess) != empty )
            {
                existingEdge = adjacency[lastAccess->user].find(currEntry.user);
                // original, becuase it gives a better understanding of what is happening, but is slower
                /*
                if ( existingEdge == adjacency[lastAccess->user].end() )
                {
                    // there isn't an edge between these two users, add one
                    // printf("Adding Edge\n");
                    adjacency[lastAccess->user][currEntry.user] = EdgeInfo(currEntry.time - lastAccess->time, currEntry.r, currEntry.g, currEntry.b, adjustedX, adjustedY);
                }
                else
                {
                    if ( existingEdge->second.weight > currEntry.time - prevAccesses[I_2D(adjustedX, adjustedY)].time )
                    {
                        adjacency[lastAccess->user][currEntry.user] = EdgeInfo(currEntry.time - lastAccess->time, currEntry.r, currEntry.g, currEntry.b, currEntry.x, currEntry.y);
                        // printf("Replacing Edge\n");
                    }
                    else
                    {
                        // printf("Keeping Edge\n");
                    }
                }
                */

                if ( (existingEdge == adjacency[lastAccess->user].end()) || (existingEdge->second.weight > currEntry.time - lastAccess->time) )
                {
                    // there isn't an edge between these two users, add one
                    // printf("Adding Edge\n");
                    adjacency[lastAccess->user][currEntry.user] = EdgeInfo(currEntry.time - lastAccess->time, currEntry.r, currEntry.g, currEntry.b, adjustedX, adjustedY);
                }
            }
            prevAccesses[I_2D(adjustedX, adjustedY)] = currEntry;
        }
        if ( (data.RemainingEntries() % 1048576) == 0 )
        {
            printf("%u remaining\n", data.RemainingEntries());
        }
    }

    free(prevAccesses);
}

Graph::~Graph()
{
}

int Graph::NumNodes() const
{
    if ( (long long unsigned int)totalNodes != adjacency.size() )
    {
        printf("Adjacency list size mismatch\n");
    }
    return totalNodes;
}

std::unordered_map<int, EdgeInfo> Graph::GetAdjacent(int nodeID) const
{
    return adjacency[nodeID];
}
