#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>


#include "render.h"
#include "algorithms.h"

// Fix wall click to remove wall not working

Grid grid = Grid();
Menu menu = Menu();

int main() {

    // Intialize SDL Modules
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Creates window and renderWindow.renderer
    RenderWindow renderWindow = RenderWindow();

    // Render Menu

    // Render Buttons

    // Render Grid
    
    renderWindow.drawWindow(menu, grid);


    // Event handling loop
    bool running = true;  // Used to shut off loop
    SDL_Event e; // Event handler
    int mouseX, mouseY; // Holds grid coordinates of mouse
    SDL_Point mouse;

    // Used to point to cells that need to be updated
    Cell *prevHighlightedCell = nullptr;
    Cell *prevWallCell = nullptr;
    bool leftClick = false; 
    bool searching = false;

    // Event Loop
    while (running) {
        while (SDL_PollEvent(&e)) {
            mouseX = (e.motion.x - 1)/cellSize; // -1 is for offset caused by grid lines
            mouseY = (e.motion.y - 4*16 - 1)/cellSize;

            mouse.x = e.motion.x;
            mouse.y = e.motion.y;
            switch (e.type){
                // Close Window
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_c: // Clear screen
                            grid = Grid();
                            prevHighlightedCell = nullptr;
                            prevWallCell = nullptr;
                            leftClick = false;
                            break; 
                        case SDLK_SPACE:  // Run BFS 
                            BFS(renderWindow, grid);
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
                            menu.algorithms.dropButton.clicked();
                            for (auto &b : menu.algorithms.ddButtons) {
                                b.clicked();
                            }
                                           
                            // If wall make it not a wall
                            if (mouseY >= 0) {
                                grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                if (grid.grid[mouseY][mouseX].isWall()) {
                                    grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                } else { // Make it a wall
                                    grid.grid[mouseY][mouseX].wallCellUpdate(true);
                                }
                            }
                            
                            renderWindow.drawWindow(menu, grid);
                            break;
                        }
                case SDL_MOUSEMOTION:
                    // Drag events
                    menu.algorithms.dropButton.update(mouse);
                    for (auto &b : menu.algorithms.ddButtons) {
                         b.update(mouse);
                    }
                
                    if (mouseY >= 0) { 
                        if (leftClick) {
                            // If Wall make default 
                            if (!prevWallCell) {                                 // Update ptr with first wall cell
                                grid.grid[mouseY][mouseX].wallCellUpdate(true);
                                prevWallCell = &grid.grid[mouseY][mouseX];
                            } else if (prevHighlightedCell->coord == grid.grid[mouseY][mouseX].coord) { // Do not in same cell
                                break;
                            } else if (!grid.grid[mouseY][mouseX].isWall()) {        // Make wall Cell
                                grid.grid[mouseY][mouseX].wallCellUpdate(true); 
                                prevWallCell = &grid.grid[mouseY][mouseX];
                            } else {                                                // Remove Wall Cell
                                grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                prevWallCell = &grid.grid[mouseY][mouseX];
                            }
                        }
                    }
                    // Highlight first cell, else if new cell unhighlight previous and highlight new
                    if (mouseY < 0 || mouse.x == 0 || mouse.y == HEIGHT-1 || mouse.x == WIDTH-1) {
                        if (prevHighlightedCell) {
                            prevHighlightedCell->mouseHighlightUpdate(false);// Unhighlight previous cell
                        }
                    }
                    else if (!prevHighlightedCell) { 
                        grid.grid[mouseY][mouseX].mouseHighlightUpdate(true); // Highlight new cell
                        prevHighlightedCell = &grid.grid[mouseY][mouseX];
                    } 
                    else if (prevHighlightedCell->coord != grid.grid[mouseY][mouseX].coord) {
                        prevHighlightedCell->mouseHighlightUpdate(false);// Unhighlight previous cell
                        grid.grid[mouseY][mouseX].mouseHighlightUpdate(true); // Highlight new cell
                        prevHighlightedCell = &grid.grid[mouseY][mouseX];
                    } 
                    renderWindow.drawWindow(menu, grid);
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

    // // SDL Destroy texture
    // SDL_DestroyTexture(textureText);

    renderWindow.destroySDL();
    SDL_Quit();
    return 0;
}