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
    Cell *prevEndCell = nullptr;

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
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT: // Select Start cell
                            if (!prevStartCell) { 
                            startCellUpdate(true, get<0>(grid[mouseY][mouseX])); // Make start Cell
                            prevStartCell = &get<0>(grid[mouseY][mouseX]);
                            } 
                            else if (prevStartCell->coord != get<0>(grid[mouseY][mouseX]).coord) {
                                startCellUpdate(false, prevStartCell); // Unhighlight previous cell
                                startCellUpdate(true, get<0>(grid[mouseY][mouseX])); // Select new start cell
                                prevStartCell = &get<0>(grid[mouseY][mouseX]);
                            } else {
                                startCellUpdate(false, prevStartCell); // Make a start cell a normal cell
                                prevStartCell = nullptr;
                            }
                            updateScreen(renderer, gridHeight, gridWidth, rectSize, grid);
                            break;
                        case SDL_BUTTON_RIGHT: // Select End cell
                            if (!prevEndCell) { 
                                endCellUpdate(true, get<0>(grid[mouseY][mouseX])); // Make end Cell
                                prevEndCell = &get<0>(grid[mouseY][mouseX]);
                            } 
                            else if (prevEndCell->coord != get<0>(grid[mouseY][mouseX]).coord) {
                                endCellUpdate(false, prevEndCell); // Unhighlight previous cell
                                endCellUpdate(true, get<0>(grid[mouseY][mouseX])); // Select new end cell
                                prevEndCell = &get<0>(grid[mouseY][mouseX]);
                            } else {
                                endCellUpdate(false, prevEndCell); // Make a end cell a normal cell
                                prevEndCell = nullptr;
                            }
                            updateScreen(renderer, gridHeight, gridWidth, rectSize, grid);
                            break;
                        }
                case SDL_MOUSEMOTION: // Mouse hovering - highlights cells
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
    // Remove SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}