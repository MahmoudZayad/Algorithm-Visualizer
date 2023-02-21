#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "grid.h"
#include "animation.h"

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

    bool running = true; // Event loop 
    SDL_Event e; // Event handler
    int newCellX, newCellY; // mouse coords
    int prevCellX = -1;
    int prevCellY = -1;
    cout << grid.size() << " " << grid[0].size();
    while (running) {
        while (SDL_PollEvent(&e)) {
                switch (e.type){
                    // Close Window
                    case SDL_QUIT: 
                        running = false; 
                        break;
                    // Mouse Motion
                    case SDL_MOUSEMOTION:
                        cout <<e.motion.x << "," << e.motion.y << endl;
                        int mouseX = (e.motion.x - 1)/rectSize; // -1 is for offset caused by grid lines
                        int mouseY = (e.motion.y - 1)/rectSize;
                        if (prevCellX == -1 && prevCellY == -1) { // First time on screen
                            // Highlight new cell
                            mouseUpdateCellHighlight(renderer, true, grid[mouseY][mouseX]);
                            prevCellX = mouseX;
                            prevCellY = mouseY;
                            cout << "First Cell: " << mouseY << "," << mouseX << endl;
                        } // If the cell location is different update previous cell and new cell
                        else if (prevCellX  != mouseX || prevCellY != mouseY) {
                            // Unhighlight previous cell
                            mouseUpdateCellHighlight(renderer, false, grid[prevCellY][prevCellX]);
                            cout << "previous: " << mouseY << "," << mouseX << endl;
                            
                            // Highlight new cell
                            mouseUpdateCellHighlight(renderer, true, grid[mouseY][mouseX]);
                            cout << "NEW: " << mouseY << "," << mouseX << endl;
                            prevCellX = mouseX;
                            prevCellY = mouseY;
                        } 
                        updateScreen(renderer, gridHeight, gridWidth, rectSize, grid);
                        // Do nothing if in the same cell.
                        break;
                }
        }
    }
    return 0;
}