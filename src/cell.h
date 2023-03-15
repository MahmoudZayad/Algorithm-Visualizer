#pragma once

#include <SDL2/SDL.h>
#include <array>

#include "fills.h"

/*
* Cells
* visited - if vertex has been visited before.
* wall - if user made vertex a wall, walls cannot be traversed.
* cost - cost difference to arrive to this vertex (total edge cost = |v2.cost - v1.cost|)
*/
class Cell {
    public:
        Cell();
        Cell(bool start);
        
        // Highlights cell if mouse moves over it
        // void mouseUpdateCellHighlight(bool highlight, Cell *&cell);
        void mouseHighlightUpdate(bool highlight);

        // Set walls on click events
        void wallCellUpdate(bool wall);
        bool isWall();


        // Cell is visited and fill
        void visit();
        void clearVisited();
        bool wasVisited(); 

        void setWeight();
        void resetWeight();
        int getWeight();
        void weightCellUpdate(bool isWeight);

        bool isStart();
        bool isEnd();

        void setPathFill();
        void setSearchFill(std::array<int,4> fill);


        std::array<int,4> getFill();

        std::pair<int, int> coord;
        SDL_FRect cellRect;
        int index;

    private:
        bool start;
        bool end;
        bool visited; // Updated by search
        bool wall; // Can be updated by user
        int weight; // Default cost, can be adjusted by user

        std::array<int,4> cellFill = defaultFill; // SDL Rectangle object - color
};



