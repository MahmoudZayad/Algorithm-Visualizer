#include "cell.h"
#include <iostream>

Cell::Cell() {
    start = false;
    end = false;
    visited = false;
    wall = false; // Can be updated by user
    weight = 1; // Default cost, can be adjusted by user
    
    coord = {0,0};
    cellFill = defaultFill;
}

Cell::Cell(bool isStart) {
    if (isStart) {
        start = true;
        end = false;
        cellFill = startFill;
    } else {
        start = false;
        end = true;
        cellFill = endFill;
    }
    visited = false;
    wall = false; // Can be updated by user
    weight = 1; // Default cost, can be adjusted by user

    cellRect = SDL_FRect();
    coord = {0,0};
}


void Cell::mouseHighlightUpdate(bool highlight) {
    if (start || end || wall  || weight == 15|| visited) {return;}
    if (highlight) {
        cellFill = highlightFill;
    } else { // Unhighlight cell
        cellFill = defaultFill;
    }
}

/////////////////////~~~~~Cell click events~~~~~~~\\\\\\\\\\\\\\\\\\\\\/

// Make a wall cell or reset
void Cell::wallCellUpdate(bool isWall) {
    if (start || end || visited) {return;}
    if (isWall) {
        cellFill = wallFill;
        wall = true;
    } else { // Unhighlight cell
        cellFill = defaultFill;
        wall = false;
        float x = cellRect.x;
        float y = cellRect.y;

        float w = cellRect.w;
        float h = cellRect.h;
        cellRect = {x + 1, y + 1, w - 2, h - 2};    
    }
}

// Make a weight cell or reset
void Cell::weightCellUpdate(bool isWeight) {
    if (start || end || visited) {return;}
    if (isWeight) {
        cellFill = weightFill;
        setWeight();
    } else { // Unhighlight cell
        cellFill = defaultFill;
        resetWeight();

        float x = cellRect.x;
        float y = cellRect.y;

        float w = cellRect.w;
        float h = cellRect.h;
        cellRect = {x + 1/2, y + 1/2, w - 1, h - 1};  
    }
}

///////////////////////~~~~~~~~~Getters and Setters~~~~~~~~~~~/////////////////////////

// Visit cell
void Cell::visit() {
    visited = true;
    // So it updates after search fill changes it
    if (end && visited) {
        cellFill = endFill;
        return;
    }
    if (start && visited) {
        cellFill = startFill;
        return;
    }
    cellFill = visitedFill;
}

// For resetting grid
void Cell::clearVisited() {
    visited = false; 
    if (start || end) {return;}
    cellFill = defaultFill;
}

// Get visited
bool Cell::wasVisited() {return visited;}

// Set and get Weight
void Cell::setWeight() {weight = 15;}
void Cell::resetWeight() {weight = 1;}
int Cell::getWeight() {return weight;}


// get Wall
bool Cell::isWall() {return wall;}

// Check if start/end
bool Cell::isStart() {
    if (start) {cellFill = startFill;} 
    return start;
}
bool Cell::isEnd() {return end;}

void Cell::setPathFill() {if (!start) cellFill = pathFill;}
void Cell::setSearchFill(std::array<int,4> fill) {
    if (end || start) {return;}
    cellFill = fill;
}
std::array<int,4> Cell::getFill() {return cellFill;}
