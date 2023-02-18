#ifndef GRAPH
#define GRAPH

#include <SDL2/SDL.h>
#include <vector>
#include <tuple>

using namespace std;

// Data Types

/*
* Graph (represented by Adjacency matrix)
* Start - start of path, selected by user.
* End - end of path, selected by user.
*/ 
struct Graph {
    Vertex start;
    Vertex end;
    vector<vector<Vertex>> adjMatrix;
};

/*
* Vertex
* visited - if vertex has been visited before.
* wall - if user made vertex a wall, walls cannot be traversed.
* cost - cost difference to arrive to this vertex (total edge cost = |v2.cost - v1.cost|)
*/
struct Vertex {
    bool visited = false;
    bool wall = false; // Can be updated by user
    int cost = 1; // Default cost, can be adjusted by user
};

#endif