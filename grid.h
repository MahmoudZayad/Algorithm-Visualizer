#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <array>
#include <iostream>

#include "fills.h"

// Size of Cell
extern int cellSize;

// Grid Size
extern int gridWidth;
extern int gridHeight;

// Grid pixel size
extern int WIDTH;
extern int HEIGHT;

/*
* Cells
* visited - if vertex has been visited before.
* wall - if user made vertex a wall, walls cannot be traversed.
* cost - cost difference to arrive to this vertex (total edge cost = |v2.cost - v1.cost|)
*/
struct Cell {
    bool start;
    bool end;
    bool visited;
    bool wall; // Can be updated by user
    int cost; // Default cost, can be adjusted by user

    std::tuple<int, int> coord; // maybe unnecessary

    // SDL Rectangle object - color
    std::array<int,4> cellFill;
};

/*
* Draws lines for the grid, windowWidth and windowHeight are calculated to create bounds
* for the loops. The bounds are the size of the window.
*/
void drawGridLines(SDL_Renderer *renderer);

/*
* Properly spaces each rectangle for rendering on the grid.
* Uses the rectangle size to determine how many pixels the next rectangle should be shifted.
*/
int calculateCellCoords(int coord);
 

/*
* Intialize Rectangle with color, size and coord
* sizexy contains the rectangle size, and the indices of the element in the grid.
*/
void intRect(SDL_Renderer *renderer, SDL_Rect &r, std::tuple<int,int> coord);

/*
* This creates a 2D-vector with a tuple of Cell and SDL_Rect. These are used to 
* keep track of the abstracted and presented properties of each cell on the grid.
* For loop - updates the vector elements with their coords and rectangle colors.
*/
std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> intializeGrid 
                (SDL_Renderer *renderer, Cell &end, Cell &start);