#ifndef GRAPH
#define GRAPH

#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

// Dark theme.
    array<int,4> lineColor = {22, 22, 22, 255}; // Background - Barely Black
    array<int,4> defaultFill = {44, 44, 44, 255}; // Grid Lines  - Dark grey

/*
* Cells
* visited - if vertex has been visited before.
* wall - if user made vertex a wall, walls cannot be traversed.
* cost - cost difference to arrive to this vertex (total edge cost = |v2.cost - v1.cost|)
*/
struct Cell {
    bool visited = false;
    bool wall = false; // Can be updated by user
    int cost = 1; // Default cost, can be adjusted by user

    tuple<int, int> coord; // maybe unnecessary

    // SDL Rectangle object - color
    array<int,4> cellFill = defaultFill;
};


/*
* Draws lines for the grid, windowWidth and windowHeight are calculated to create bounds
* for the loops. The bounds are the size of the window.
*/
void drawGridLines(SDL_Renderer *renderer, int gridHeight, int gridWidth, int rectSize) {
    int windowHeight = gridHeight * rectSize; // Determine window size
    int windowWidth = gridWidth * rectSize;

    array<int,4> c = lineColor;
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]); // color off black

    // Draw vertical grid lines
    for (int x = 0; x < 1 + windowWidth; x += rectSize) {
        SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
    }

    // Draw horizontal grid lines.
    for (int y = 0; y < 1 + windowHeight; y += rectSize) {
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }
    SDL_RenderPresent(renderer);
}

/*
* Properly spaces each rectangle for rendering on the grid.
* Uses the rectangle size to determine how many pixels the next rectangle should be shifted.
*/
int calculateCellCoords(int coord, int rectSize) {
    return 1 + (coord*rectSize);
}


/*
* Intialize Rectangle with color, size and coord
* sizexy contains the rectangle size, and the indices of the element in the grid.
*/
void intRect(SDL_Renderer *renderer, SDL_Rect &r, tuple<int,int,int> sizexy) {
    r.h = get<0>(sizexy);
    r.w = get<0>(sizexy);
    r.x = calculateCellCoords(get<2>(sizexy), get<0>(sizexy)); 
    r.y = calculateCellCoords(get<1>(sizexy), get<0>(sizexy));

    array<int,4> c = defaultFill;
    // Draw Rectangles
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]); // default color off black
    SDL_RenderFillRect(renderer, &r);
}

/*
* This creates a 2D-vector with a tuple of Cell and SDL_Rect. These are used to 
* keep track of the abstracted and presented properties of each cell on the grid.
* For loop - updates the vector elements with their coords and rectangle colors.
*/
auto intializeGrid (SDL_Renderer *renderer, int gridHeight, int gridWidth, int rectSize) {
    vector<vector<tuple<Cell, SDL_Rect>>> grid(gridHeight, 
    vector<tuple<Cell, SDL_Rect>>(gridWidth, make_tuple(Cell(), SDL_Rect())));

    // Add Coord data and update colors for the rectangles
    for (int i = 0; i < gridHeight; i++) {
            for (int j = 0; j < gridWidth; j++) {
                get<0>(grid[i][j]).coord = {i,j};
                intRect(renderer, get<1>(grid[i][j]), {rectSize,i,j});
            }
        }

    drawGridLines(renderer, gridHeight, gridWidth, rectSize);
    
    return grid;
}

#endif