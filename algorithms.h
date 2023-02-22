#ifndef ALGORITHM
#define ALGORITHM

#include <queue>
#include <SDL2/SDL.h>
#include <tuple>

#include "grid.h"
#include "render.h"

/*
* Finds Coords of all children given root and grid size.
* Marks root as visited once complete.
*/
array<tuple<int, int>, 4> findChildren(Cell &root, int gridWidth, int gridHeight) {
    array<tuple<int, int>, 4> children; 

    int rootX = get<0>(root.coord);
    int rootY = get<1>(root.coord);

    // Check Above
    if (rootX - 1 >= 0){
        children[0] = tuple(rootX-1, rootY);
    }  else { 
        children[0] = tuple(-1,-1); 
    }
    // Check Right
    if (rootY + 1 <= gridWidth){
        children[1] = tuple(rootX, rootY + 1);
    }  else { 
        children[1] = tuple(-1,-1); 
    }
    // Check Below
    if (rootX + 1 <= gridHeight){
        children[2] = tuple(rootX + 1, rootY);
    }  else { 
        children[2] = tuple(-1,-1); 
    }
    // Check Behind
    if (rootY - 1 >= 0){
        children[3] = tuple(rootX, rootY - 1);
    }  else { 
        children[3] = tuple(-1,-1); 
    }

    return children;
}

// Search Algorithms 
void BFS(vector<vector<tuple<Cell, SDL_Rect>>> &grid, Cell &start) {
    // Declare Queue
    queue<Cell> q;
    start.visited = true; // Mark root visited
    q.push(start);

    while (!q.empty()) { // Run till queue is empty
        Cell cell = q.front();
        q.pop();
        if (cell.end) {break;} // End of Search

        // Explore children
        array<tuple<int, int>, 4> children = findChildren(cell, grid.size(), grid[0].size());
        for (const auto &coord : children) {
            int y = get<0>(coord);
            int x = get<1>(coord);

            // Add children to queue if they have not been visited and are not a wall
            if (!get<0>(grid[y][x]).visited && !get<0>(grid[y][x]).wall) { 
                get<0>(grid[y][x]).visited = true;
                q.push(get<0>(grid[y][x]));
            }          
        } 
    }
}

#endif