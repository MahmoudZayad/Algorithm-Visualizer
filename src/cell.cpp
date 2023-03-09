#include "cell.h"
#include <iostream>

Cell::Cell() {
    start = false;
    end = false;
    visited = false;
    wall = false; // Can be updated by user
    weight = 1; // Default cost, can be adjusted by user
    index = 0;

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
    index = 0;
}


void Cell::mouseHighlightUpdate(bool highlight) {
    if (start || end || wall  || weight == 15|| visited) {return;}
    if (highlight) {
        cellFill = highlightFill;
    } else { // Unhighlight cell
        cellFill = defaultFill;
    }
}

// Cell click events

// Left Clicking Cell
bool Cell::isWall() {return wall;}
void Cell::wallCellUpdate(bool isWall) {
    if (start || end) {return;}
    if (isWall) {
        cellFill = wallFill;
        wall = true;
    } else { // Unhighlight cell
        cellFill = defaultFill;
        wall = false;
    }
}

void Cell::visit() {
    visited = true;
    if (end || start) {return;}
    cellFill = visitedFill;
}

void Cell::clearVisited() {
    visited = false; 
    if (start || end) {return;}
    cellFill = defaultFill;
}

bool Cell::wasVisited() {return visited;}

// Set and get Weight
void Cell::setWeight() {weight = 15;}
void Cell::resetWeight() {weight = 1;}
int Cell::getWeight() {return weight;}

void Cell::weightCellUpdate(bool isWeight) {
    if (start || end) {return;}
    if (isWeight) {
        cellFill = weightFill;
        setWeight();
    } else { // Unhighlight cell
        cellFill = defaultFill;
        resetWeight();
    }
}


// Check if start/end
bool Cell::isStart() {
    if (start) {cellFill = startFill;} 
    return start;
}
bool Cell::isEnd() {return end;}

void Cell::setPathFill() {if (!start) cellFill = pathFill;}
void Cell::setSearchFill() {
    if (end && visited) {
        cellFill = endFill;
        return;
    }
    if (start && visited) {
        cellFill = startFill;
        return;
    }
    cellFill = searchingFill;
}
std::array<int,4> Cell::getFill() {return cellFill;}
