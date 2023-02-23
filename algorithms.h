#ifndef ALGORITHM
#define ALGORITHM

#include <SDL2/SDL.h>
#include <queue>
#include <tuple>
#include <map>
#include <stack>

#include "grid.h"
#include "render.h"

/*
*  Takes map and creates stack to be used to render path
*/
stack<Cell> reconstructPath(map<Cell,Cell> prev, Cell end) {
    stack<Cell> path;

    Cell currentCell = end;
    Cell previousCell;

    path.push(currentCell);
    while (!currentCell.start) {
        previousCell = prev[currentCell];
        path.push(previousCell);  
        currentCell = previousCell;
    }
    return path;
}

/*
* Finds Coords of all children given root and grid size.
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
stack<Cell> BFS(vector<vector<tuple<Cell, SDL_Rect>>> &grid, Cell &start) {
    // Declare Queue
    queue<Cell> fringe;
    map<Cell, Cell> previous;
    Cell currentCell;
    Cell nextCell;
    Cell end;

    start.visited = true; // Mark root visited
    fringe.push(start);

    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.front();
        fringe.pop();

        if (currentCell.end) {  // End of Search return path
            end = currentCell;
            end.visited = true;
            reconstructPath(previous, end);
        } 

        // Explore children
        array<tuple<int, int>, 4> children = findChildren(currentCell, grid.size(), grid[0].size());

        for (const auto &coord : children) {
            int y = get<0>(coord);
            int x = get<1>(coord);

            // Add children to queue if they have not been visited and are not a wall
            if (!get<0>(grid[y][x]).visited && !get<0>(grid[y][x]).wall) { 
                get<0>(grid[y][x]).visited = true;
                fringe.push(get<0>(grid[y][x]));
                nextCell = get<0>(grid[y][x]);
                previous[nextCell] = currentCell;
            }          
        }   
    }
}

#endif