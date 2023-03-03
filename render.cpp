#include "render.h"

RenderWindow::RenderWindow() {
    window = nullptr;
    renderer = nullptr;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255); // Set draw color to use on renderer
    SDL_RenderClear(renderer); // Clear the screen to black (in memory atm)

    font = TTF_OpenFont("font/OpenSans-Regular.ttf", 12);
}

void RenderWindow::destroySDL() {
    // Close font subsystem 
    TTF_CloseFont(font);

    // Remove SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void RenderWindow::drawWindow(Menu &menu, Grid &g) {
    SDL_SetRenderDrawColor(renderer, 0,0,0,255); // Set draw color to use on renderer
    SDL_RenderClear(renderer); // Clear the screen to black (in memory atm)
    // drawGrid(g);
    drawMenu(menu);
    updateDropDown(menu.algorithms);
    SDL_RenderPresent(renderer);
}

void RenderWindow::drawMenu(Menu &menu) {
    drawMenuBar(menu);
    drawButton(menu.algorithms.dropButton);
    // Other buttons
} 

void RenderWindow::drawMenuBar(Menu &menu) {
    SDL_SetRenderDrawColor(renderer, buttonFill[0],buttonFill[1],buttonFill[2],buttonFill[3]);
    SDL_RenderFillRect(renderer, &menu.menuBar);
}

void RenderWindow::drawGrid(Grid &g) {

    // Update all cells on grid
    for (int i = 0; i < g.getHeight(); i++) {
        for (int j = 0; j < g.getWidth(); j++) {
            std::array<int, 4> c = g.grid[i][j].getFill(); // Checks Cell Color
            SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
            SDL_RenderFillRect(renderer, &g.grid[i][j].cellRect);
        }
    }

    // Redraw Lines
    drawGridLines(g);
}


/*
* Draws lines for the grid, windowWidth and windowHeight are calculated to create bounds
* for the loops. The bounds are the size of the window.
*/
void RenderWindow::drawGridLines(Grid &g) {
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


/*
* Redraws the screen for the next frame
*/
void RenderWindow::updateDropDown(DropDown &dropDown) {
    
    // drawMenuBar(menu);
    // drawButton(dropDown.dropButton);

    if (dropDown.dropButton.isClicked()) {
        drawComboBox(dropDown);
    }

}

/*
 * Make a texture for text to be used on button
*/
SDL_Texture* RenderWindow::makeStringTexture(std::string text) {
    
    // Pixels from text
    TTF_SetFontSizeDPI(font, 16, 2048,2048);
    SDL_Surface* surfaceText = TTF_RenderUTF8_Blended(font, text.c_str(), {255, 255, 255, 255});

    // Setup texture
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

    // Free surface since we loaded it to the texture
    SDL_FreeSurface(surfaceText);

    return textureText;
}

/*
 *  Draw the button in render.CPP 
*/
void RenderWindow::drawButton(Button &b) {
    std::array<int, 4> fill = b.returnFill();
    SDL_SetRenderDrawColor(renderer, fill[0], fill[1], fill[2], fill[3]);
  	b.texture = makeStringTexture(b.returnText());
    SDL_RenderFillRect(renderer, &b.button);
    SDL_RenderFillRect(renderer, &b.center);

    SDL_RenderCopy(renderer, b.texture, NULL, &b.center);
}

/*
* If menu button is clicked draw this (Place in render.cpp)
*/
void RenderWindow::drawComboBox(DropDown &d) {
    SDL_SetRenderDrawColor(renderer, d.comboBoxFill[0], d.comboBoxFill[1], d.comboBoxFill[2], d.comboBoxFill[3]);
    SDL_RenderFillRect(renderer, &d.comboBox);

    // Draw Buttons that need to be displayed in the comboBox
    for (int i = 0; i < d.ddButtons.size(); i++) {
        drawButton(d.ddButtons[i]);
    }
}

// Check all menuButtons to see which one needs to be updated
    // for (auto b : menuButtons) {
    //     if (b.isClicked) {
    //         b.draw(renderer);
    //     }
    // }