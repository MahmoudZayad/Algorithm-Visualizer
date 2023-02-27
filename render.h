
#pragma once

#include <SDL2/SDL.h>

#include "grid.h"


/*
* Redraws the screen for the next frame
*/
void updateScreen(SDL_Renderer *renderer, std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> grid);

// Grid Animations

// Mouse Cursor Animation
// When moving mouse, whatever cell its in should highlight.

///// Need to add highlighting for other cell types,
///// maybe try drawing a translucent rectangle on top instead?
void mouseUpdateCellHighlight(bool highlight, Cell *&cell);

void mouseUpdateCellHighlight(bool highlight, Cell &cell);

// Cell click events

// Left Clicking Cell
void wallCellUpdate(bool wall, Cell *&cell);

void wallCellUpdate(bool wall, Cell &cell);