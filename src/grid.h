#pragma once

#include <vector>
#include "cell.h"

class Grid {
    public:
        Grid();
        void clearWalls();
        void clearSearch();
        int getHeight();
        int getWidth();

        Cell start;
        Cell end;

        std::vector<std::vector<Cell>> grid;
        
        
    private:
        int calculateCellCoords(int coord);
        void intRect(Cell &cell, std::tuple<int,int> coord);
        int gridWidth;
        int gridHeight;
};