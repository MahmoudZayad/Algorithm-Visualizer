#pragma once

#include <queue>
#include <tuple>
#include <map>
#include <stack>
#include <vector>
#include <iostream>

#include "grid.h"
#include "render.h"
#include "fills.h"

/*
* Print path taken to end
*/
void printPath(std::stack<Cell*> path);

/*
*  Takes map and creates stack to be used to render path
*/
std::stack<Cell*> reconstructPath(SDL_Renderer *renderer,std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> &grid, 
                            std::map<Cell*,Cell*> prev, Cell *&end);

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::tuple<int, int>, 4> findChildren(Cell *&root);

// Search Algorithms 
void BFS(std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> &grid, Cell &start,
                SDL_Renderer *renderer);