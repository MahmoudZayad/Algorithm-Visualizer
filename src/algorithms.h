#pragma once

#include <queue>

#include <map>
#include <stack>


#include "render.h"
#include "fills.h"


/*
* Print path taken to end
*/
void printPath(std::stack<Cell*> path);

/*
*  Takes map and creates stack to be used to render path
*/
std::stack<Cell*> reconstructPath(RenderWindow &renderWindow, Grid &grid, std::map<Cell*,Cell*> prev, Cell *&end);

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::tuple<int, int>, 4> findChildren(Grid &g, Cell *&root);

// Search Algorithms 
void BFS(RenderWindow &renderWindow, Grid &grid);