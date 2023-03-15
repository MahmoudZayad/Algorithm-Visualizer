#include "grid.h"
#include <iostream>

/*
* Properly spaces each rectangle for rendering on the grid.
* Uses the rectangle size to determine how many pixels the next rectangle should be shifted.
*/
int Grid::calculateCellCoords(int coord) {return 1 + (coord*(cellSize));}
int Grid::getHeight() {return gridHeight;}
int Grid::getWidth() {return gridWidth;}
// Cell Grid::getStart() {return start;}
// Cell Grid::getEnd() {return end;}
 

/*
* Intialize Rectangle with color, size and coord
* sizexy contains the rectangle size, and the indices of the element in the grid.
*/
void Grid::intRect(Cell &cell, std::pair<int,int> coord) {
    int menuHeight = 4*cellSize;
    cell.cellRect.h = cellSize - 1 ;
    cell.cellRect.w = cellSize - 1;
    cell.cellRect.y = calculateCellCoords(coord.first) + menuHeight;
    cell.cellRect.x = calculateCellCoords(coord.second); 
}

/*
* This creates a 2D-vector with a tuple of Cell and SDL_Rect. These are used to 
* keep track of the abstracted and presented properties of each cell on the grid.
* For loop - updates the vector elements with their coords and rectangle colors.
*/
Grid::Grid () {

    int menuHeight = 4*cellSize;

    // Determine size of grid
    gridHeight = (HEIGHT-menuHeight-1)/cellSize;
    gridWidth = WIDTH/cellSize;

    grid = std::vector<std::vector<Cell>>(gridHeight, std::vector<Cell>(gridWidth, Cell()));

    // For start and end placement - calculate spacing
    int hSpace = gridHeight/2;
    int wSpace = gridWidth/4;

    // Add Coord data and update colors for the rectangles
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            if (i == hSpace && j == wSpace) { // Start coord
                grid[i][j] = Cell(true);
                grid[i][j].coord = {i,j};
                start = grid[i][j];
            } 
            else if (i == hSpace && j == 3*wSpace) { // End Coord
                grid[i][j] = Cell(false);
                grid[i][j].coord = {i,j};
                end = grid[i][j];
            } else {
                grid[i][j].coord = {i,j};
            }
            intRect(grid[i][j], {i,j});
        }
    }
}

void Grid::clearWalls() {
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            if (grid[i][j].isWall()) { 
                grid[i][j].wallCellUpdate(false);
            }
            if (grid[i][j].wasVisited()) {
                grid[i][j].clearVisited();
            }
        }
    }
}

void Grid::clearWeights() {
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            if (grid[i][j].getWeight() != 1) { 
                grid[i][j].weightCellUpdate(false);
            }
            if (grid[i][j].wasVisited()) {
                grid[i][j].clearVisited();
            }
        }
    }
}

void Grid::clearSearch() {
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            if (grid[i][j].wasVisited()) {
                grid[i][j].clearVisited();
            }
        }
    }
}