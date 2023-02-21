#ifndef ANIMATION
#define ANIMATION

#include <SDL2/SDL.h>
#include <tuple>
#include "grid.h"

using namespace std;

array<int, 4> highlightFill = {88, 88, 88, 255}; // lighter grey
array<int, 4> startFill = {171, 237, 198, 255};  // Celadon - bluish

/*
* Redraws the screen for the next frame
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
void mouseUpdateCellHighlight(bool highlight, Cell *&cell) {
    if (cell->start) {return;}
    if (highlight) {
        cell->cellFill = highlightFill;
    } else { // Unhighlight cell
        cell->cellFill = defaultFill;
    }
}

void mouseUpdateCellHighlight(bool highlight, Cell &cell) {
    if (cell.start) {return;}
    if (highlight) {
        cell.cellFill = highlightFill;
    } else { // Unhighlight cell
        cell.cellFill = defaultFill;
    }
}


// // Clicking Node
void startCellUpdate(bool start, Cell *&cell) {
    if (start) {
        cell->cellFill = startFill;
        cell->start = true;
    } else { // Unhighlight cell
        cell->cellFill = defaultFill;
        cell->start = false;
    }
}

void startCellUpdate(bool start, Cell &cell) {
    if (start) {
        cell.cellFill = startFill;
        cell.start = true;
    } else { // Unhighlight cell
        cell.cellFill = defaultFill;
        cell.start = false;
    }
}




#endif