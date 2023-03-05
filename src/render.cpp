#include "render.h"

RenderWindow::RenderWindow() {}

// SDL_Renderer* RenderWindow::getRenderer() {return renderer;}
// SDL_Window* RenderWindow::getWindow() {return window;}

// void RenderWindow::setupRenderPlatform() {
//     // Setup Platform/Renderer backends
//     ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
//     ImGui_ImplSDLRenderer_Init(renderer);
// }

void RenderWindow::destroySDL(SDL_Renderer* renderer, SDL_Window* window) {

    // Remove SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// void RenderWindow::drawWindow(Grid &g) {
//     SDL_SetRenderDrawColor(renderer, 0,0,0,255); // Set draw color to use on renderer
//     SDL_RenderClear(renderer); // Clear the screen to black (in memory atm)
//     drawGrid(g);
//     SDL_RenderPresent(renderer);
// }


void RenderWindow::drawGrid(SDL_Renderer* renderer, Grid &g) {

    // Update all cells on grid
    for (int i = 0; i < g.getHeight(); i++) {
        for (int j = 0; j < g.getWidth(); j++) {
            std::array<int, 4> c = g.grid[i][j].getFill(); // Checks Cell Color
            SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
            SDL_RenderFillRect(renderer, &g.grid[i][j].cellRect);
        }
    }

    // Redraw Lines
    drawGridLines(renderer, g);
}


/*
* Draws lines for the grid, windowWidth and windowHeight are calculated to create bounds
* for the loops. The bounds are the size of the window.
*/
void RenderWindow::drawGridLines(SDL_Renderer* renderer, Grid &g) {
    int windowHeight = g.getHeight() * cellSize; // Determine window size
    int windowWidth = g.getWidth() * cellSize;

    std::array<int,4> c = lineColor;
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]); // color off black

    // Draw vertical grid lines
    for (int x = 0; x < 1 + windowWidth; x += cellSize) {
        SDL_RenderDrawLine(renderer, x, 4*16, x, windowHeight + 4*16);
    }

    // Draw horizontal grid lines.
    for (int y = 0; y < 1 + windowHeight; y += cellSize) {
        SDL_RenderDrawLine(renderer, 0, 4*16 + y, windowWidth, 4*16+ y);
    }
}

