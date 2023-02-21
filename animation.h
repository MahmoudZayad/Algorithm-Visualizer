#ifndef ANIMATION
#define ANIMATION

#include <SDL2/SDL.h>
#include <tuple>
#include "grid.h"

using namespace std;

array<int, 4> cellHighlight = {88, 88, 88, 255}; // lighter grey

/*
* This creates a 2D-vector with a tuple of Cell and SDL_Rect. These are used to 
* keep track of the abstracted and presented properties of each cell on the grid.
* For loop - updates the vector elements with their coords and rectangle colors.
*/
void updateScreen(SDL_Renderer *renderer, int gridHeight, int gridWidth, int rectSize,
 vector<vector<tuple<Cell, SDL_Rect>>> grid) {

    // Update all rectangles on screen
    for (int i = 0; i < gridHeight; i++) {
            for (int j = 0; j < gridWidth; j++) {
                array<int, 4> c = get<0>(grid[i][j]).cellFill; // Checks Cell Color
                SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
                SDL_RenderFillRect(renderer, &get<1>(grid[i][j]));
            }
        }
    // Redraw Lines
    drawGridLines(renderer, gridHeight, gridWidth, rectSize);
}


// Grid Animations

// Mouse Cursor Animation
// When moving mouse, whatever cell its in should highlight.

void mouseUpdateCellHighlight(SDL_Renderer *renderer, bool highlight, tuple<Cell,SDL_Rect> &cell) {
    if (highlight) {
        get<0>(cell).cellFill = cellHighlight;
    } else { // Unhighlight cell
        get<0>(cell).cellFill = defaultFill;
    }
}


// Clicking Node



#endif