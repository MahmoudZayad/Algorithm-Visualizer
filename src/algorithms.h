#pragma once

#include <queue>
#include <functional>

#include <map>
#include <stack>


#include "render.h"
#include "fills.h"


void algorithmsMenu(bool& show_algorithms, int& algorithm);
void speedMenu(bool& show_speed);
void weightMenu(bool &show_weights, int& weight);


/*
* Print path taken to end
*/
void printPath(std::stack<Cell*> path);

/*
*  Takes map and creates stack to be used to render path
*/
std::stack<Cell*> reconstructPath(RenderWindow &renderWindow, Grid &grid, ImGuiIO& io, std::map<Cell*,Cell*> prev, Cell *&end);

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::tuple<int, int>, 4> findChildren(Grid &g, Cell *&root);

// Search Algorithms 

void generalSearch(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::function<bool(Cell*,Cell*)> func);
void BFS(RenderWindow &renderWindow, Grid &grid, ImGuiIO& io);
void DFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io);
void IDS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io);
void depthLimitedSearch(RenderWindow& renderWindow, Grid& g, ImGuiIO& io, int l);

enum Algorithm_ {
    Algorithm_None,
    Algorithm_BFS, // Breadth-first Search
    Algorithm_DFS, // Depth-first Search
    Algorithm_UCS, // Uniform-cost Search
    Algorithm_IDS, // Iterative Deepening
    Algorithm_Greedy, // Greedy Best-first Search
    Algorithm_AStar, // A* Search
    Algorithm_Dijkstra, // Dijkstra's Algorithm
    Algorithm_Swarm // Swarm Algorithm
};

extern int speed;

enum Speed_ { // Used to set speed of search
    Speed_Fast  = 5,
    Speed_Normal = 30,
    Speed_Slow = 2000
};

enum Draw_ { // Used to set speed of search
    Draw_Wall,
    Draw_Weights,
    Draw_Bombs
};