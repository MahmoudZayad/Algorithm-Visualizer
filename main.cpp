#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "grid.h"
#include "animation.h"
#include <tuple>

using namespace std;

const int WIDTH = 640; // Temporary until I figure out what to do with resizing
const int HEIGHT = 480;

const int numCells = 300; // How many cells we want for now? Maybe ill allow user to adjust

int rectSize = sqrt((WIDTH*HEIGHT)/numCells); 

// Determine size of grid
int gridHeight = HEIGHT/rectSize;
int gridWidth = WIDTH/rectSize;

////// Start working on event handling so I can select start, end nodes and create walls.

int main() {

    // Intialize SDL Modules
    SDL_Init(SDL_INIT_EVERYTHING);

    // Creates window and renderer
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(WIDTH+1, HEIGHT+1, 0, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); // clear screen white
    
    auto grid = intializeGrid(renderer,gridHeight, gridWidth, rectSize);
    SDL_RenderPresent(renderer);

    // Event handling loop
    bool running = true;  // Used to shut off loop
    SDL_Event e; // Event handler
    int mouseX, mouseY; // Holds grid coordinates of mouse

    // Used to point to cells that need to be updated
    Cell *prevHighlightedCell = nullptr;
    Cell *prevStartCell = nullptr;

    // Event Loop
    while (running) {
        while (SDL_PollEvent(&e)) {
            mouseX = (e.motion.x - 1)/rectSize; // -1 is for offset caused by grid lines
            mouseY = (e.motion.y - 1)/rectSize;
            switch (e.type){
                // Close Window
                case SDL_QUIT: 
                    running = false; 
                    break;
                // Mouse Motion
                case SDL_MOUSEMOTION:
                    // Highlight first cell, else if new cell unhighlight previous and highlight new
                    if (!prevHighlightedCell) { 
                        mouseUpdateCellHighlight(true, get<0>(grid[mouseY][mouseX])); // Highlight new cell
                        prevHighlightedCell = &get<0>(grid[mouseY][mouseX]);
                    } 
                    else if (prevHighlightedCell->coord != get<0>(grid[mouseY][mouseX]).coord) {
                        mouseUpdateCellHighlight(false, prevHighlightedCell); // Unhighlight previous cell
                        mouseUpdateCellHighlight(true, get<0>(grid[mouseY][mouseX])); // Highlight new cell
                        prevHighlightedCell = &get<0>(grid[mouseY][mouseX]);
                    } 
                    updateScreen(renderer, gridHeight, gridWidth, rectSize, grid);
                    break;
            }
        }
    }
    return 0;
}