#pragma once

#include <vector>
#include "cell.h"

class Grid {
    public:
        Grid();
        int getHeight();
        int getWidth();
        
        Cell* getStart();
        Cell* getEnd();

        std::vector<std::vector<Cell>> grid;
        
        
    private:
        int calculateCellCoords(int coord);
        void intRect(Cell &cell, std::tuple<int,int> coord);

        Cell *start = nullptr;
        Cell *end = nullptr;
        int gridWidth;
        int gridHeight;
};