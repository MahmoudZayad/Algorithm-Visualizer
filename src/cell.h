#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <tuple>

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
        bool wasVisited(); 

        void setWeight();
        int getWeight();

        bool isStart();
        bool isEnd();

        void setPathFill();

        std::array<int,4> getFill();

        std::tuple<int, int> coord;
        SDL_Rect cellRect;

    private:
        bool start;
        bool end;
        bool visited; // Updated by search
        bool wall; // Can be updated by user
        int weight; // Default cost, can be adjusted by user

        std::array<int,4> cellFill = defaultFill; // SDL Rectangle object - color
};



