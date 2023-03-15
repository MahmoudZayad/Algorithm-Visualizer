#pragma once

#include <vector>
#include "cell.h"

class Grid {
    public:
        Grid();
        void clearWalls();
        void clearWeights();
        void clearSearch();
        int getHeight();
        int getWidth();

        int calculateCellCoords(int coord);

        Cell start;
        Cell end;

        std::vector<std::vector<Cell>> grid;
        
        
    private:
        void intRect(Cell &cell, std::pair<int,int> coord);
        int gridWidth;
        int gridHeight;
};