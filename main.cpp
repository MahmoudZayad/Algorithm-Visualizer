#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <iostream>

#include "grid.h"
#include "render.h"
#include "algorithms.h"

// Fix wall click to remove wall not working

// Fix render not updating after clearing


Cell start;
Cell end;

int main() {

    // Intialize SDL Modules
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Creates window and renderer
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(WIDTH+1, HEIGHT+1, 0, &window, &renderer);

    // Load font
    TTF_Font* font32 = TTF_OpenFont("/font/OpenSans-Regular.ttf", 32);

    // Pixels from text
    SDL_Surface* surfaceText = TTF_RenderText_Solid(font32, "Breadth-first Search", {255, 255, 255});

    // Setup texture
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

    // Free surface since we loaded it to the texture
    SDL_FreeSurface(surfaceText);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); // clear screen white
    
    auto grid = intializeGrid(renderer, end, start);

    SDL_RenderPresent(renderer);

    // Event handling loop
    bool running = true;  // Used to shut off loop
    SDL_Event e; // Event handler
    int mouseX, mouseY; // Holds grid coordinates of mouse

    // Used to point to cells that need to be updated
    Cell *prevHighlightedCell = nullptr;
    Cell *prevWallCell = nullptr;
    bool leftClick = false; 
    bool searching = false;

    // Event Loop
    while (running) {
        while (SDL_PollEvent(&e)) {
            mouseX = (e.motion.x - 1)/cellSize; // -1 is for offset caused by grid lines
            mouseY = (e.motion.y - 1)/cellSize;
            switch (e.type){
                // Close Window
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_c: // Clear screen
                            grid = intializeGrid(renderer, end, start);
                            prevHighlightedCell = nullptr;
                            prevWallCell = nullptr;
                            leftClick = false;
                            break; 
                        case SDLK_SPACE:  // Run BFS 
                            BFS(grid, start, renderer);
                            break;
                        default:
                            continue;
                    }
                    break;
                case SDL_QUIT: 
                    running = false; 
                    break;
                case SDL_MOUSEBUTTONDOWN: // WORK ON DRAG EVENT
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT: // Select Wall cell   
                            leftClick = true;                         
                            // If wall make it not a wall
                            wallCellUpdate(false, std::get<0>(grid[mouseY][mouseX]));
                            if (std::get<0>(grid[mouseY][mouseX]).wall) {
                                wallCellUpdate(false, std::get<0>(grid[mouseY][mouseX]));
                            } else { // Make it a wall
                                wallCellUpdate(true,  std::get<0>(grid[mouseY][mouseX]));
                            }
                            updateScreen(renderer, grid);
                            break;
                        }
                case SDL_MOUSEMOTION:
                    // Drag events
                    if (leftClick) {
                        // If Wall make default 
                        if (!prevWallCell) {                                    // Update ptr with first wall cell
                            wallCellUpdate(true, std::get<0>(grid[mouseY][mouseX]));
                            prevWallCell = &std::get<0>(grid[mouseY][mouseX]);
                        } else if (prevHighlightedCell->coord == std::get<0>(grid[mouseY][mouseX]).coord) { // Do not in same cell
                            break;
                        } else if (!std::get<0>(grid[mouseY][mouseX]).wall) {        // Make wall Cell
                            wallCellUpdate(true, std::get<0>(grid[mouseY][mouseX])); 
                            prevWallCell = &std::get<0>(grid[mouseY][mouseX]);
                        } else {                                                // Remove Wall Cell
                            wallCellUpdate(false, std::get<0>(grid[mouseY][mouseX]));
                            prevWallCell = &std::get<0>(grid[mouseY][mouseX]);
                        }
                    }
                    // Highlight first cell, else if new cell unhighlight previous and highlight new
                    if (!prevHighlightedCell) { 
                        mouseUpdateCellHighlight(true, std::get<0>(grid[mouseY][mouseX])); // Highlight new cell
                        prevHighlightedCell = &std::get<0>(grid[mouseY][mouseX]);
                    } 
                    else if (prevHighlightedCell->coord != std::get<0>(grid[mouseY][mouseX]).coord) {
                        mouseUpdateCellHighlight(false, prevHighlightedCell); // Unhighlight previous cell
                        mouseUpdateCellHighlight(true, std::get<0>(grid[mouseY][mouseX])); // Highlight new cell
                        prevHighlightedCell = &std::get<0>(grid[mouseY][mouseX]);
                    } 
                    updateScreen(renderer, grid);
                    break;
                case SDL_MOUSEBUTTONUP:
                    switch(e.button.button) {
                        case SDL_BUTTON_LEFT:
                            leftClick = false;
                    }
                    break;
            }
        }
    }

    // SDL Destroy texture
    SDL_DestroyTexture(textureText);

    // Close font subsystem 
    TTF_CloseFont(font32);

    // Remove SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}