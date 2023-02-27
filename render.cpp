#include "render.h"

/*
* Redraws the screen for the next frame
*/
void updateScreen(SDL_Renderer *renderer, std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> grid) {

    // Update all rectangles on screen
    for (int i = 0; i < gridHeight; i++) {
            for (int j = 0; j < gridWidth; j++) {
                std::array<int, 4> c = std::get<0>(grid[i][j]).cellFill; // Checks Cell Color
                SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
                SDL_RenderFillRect(renderer, &std::get<1>(grid[i][j]));
            }
        }
    // Redraw Lines
    drawGridLines(renderer);
}


// Grid Animations

// Mouse Cursor Animation
// When moving mouse, whatever cell its in should highlight.

///// Need to add highlighting for other cell types,
///// maybe try drawing a translucent rectangle on top instead?
void mouseUpdateCellHighlight(bool highlight, Cell *&cell) {
    if (cell->start || cell->end || cell->wall || cell->visited) {return;}
    if (highlight) {
        cell->cellFill = highlightFill;
    } else { // Unhighlight cell
        cell->cellFill = defaultFill;
    }
}

void mouseUpdateCellHighlight(bool highlight, Cell &cell) {
    if (cell.start || cell.end || cell.wall || cell.visited) {return;}
    if (highlight) {
        cell.cellFill = highlightFill;
    } else { // Unhighlight cell
        cell.cellFill = defaultFill;
    }
}

// Cell click events

// Left Clicking Cell
void wallCellUpdate(bool wall, Cell *&cell) {
    if (cell->start || cell->end) {return;}
    if (wall) {
        cell->cellFill = wallFill;
        cell->wall = true;
    } else { // Unhighlight cell
        cell->cellFill = defaultFill;
        cell->wall = false;
    }
}

void wallCellUpdate(bool wall, Cell &cell) {
    if (cell.start || cell.end) {return;}
    if (wall) {
        cell.cellFill = wallFill;
        cell.wall = true;
    } else { // Unhighlight cell
        cell.cellFill = defaultFill;
        cell.wall = false;
    }
}