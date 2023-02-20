#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "grid.h"

using namespace std;

const int WIDTH = 640; // Temporary until I figure out what to do with resizing
const int HEIGHT = 480;

const int numVertices = 300; // How many cells we want for now? Maybe ill allow user to adjust

int rectSize = sqrt((WIDTH*HEIGHT)/numVertices); 

// Determine size of grid - used in for loops and intializing gridArray
int gridHeight = HEIGHT/rectSize;
int gridWidth = WIDTH/rectSize;

int main() {

    // Intialize SDL Modules
    SDL_Init(SDL_INIT_EVERYTHING);

    // Creates window and renderer
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, bG.r, bG.g, bG.b, bG.a);
    SDL_RenderClear(renderer); // clear screen black

    intializeGrid(renderer,gridHeight, gridWidth, rectSize);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000); // So it does not close immediately


    return 0;
}