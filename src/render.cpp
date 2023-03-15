#include "render.h"

RenderWindow::RenderWindow() :window(NULL), renderer(NULL) {
    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Pathfinding Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, window_flags);

    if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
}

SDL_Renderer* RenderWindow::getRenderer() {return renderer;}
SDL_Window* RenderWindow::getWindow() {return window;}

void RenderWindow::render(Grid& g, ImGuiIO& io) {

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    // SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_SetRenderDrawColor(renderer,44, 44, 44, 255);
    SDL_RenderClear(renderer);
    drawGrid(g);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void RenderWindow::destroySDL() {

    // Remove SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

//////////~~~~~~~~~IMGUI~~~~~~~~~~~////////////////////////////////////


void RenderWindow::setupImGuiContext() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io =  ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void RenderWindow::setupRenderPlatform() {
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
}

void RenderWindow::startImGuiFrame() {
    // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
}

void RenderWindow::destroyImGui() {
    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

/////////////////~~~~~~~~~~~~~~~~~~~Grid~~~~~~~~~~~///////////////////////////

void RenderWindow::drawGrid(Grid &g) {

    std::array<int, 4> c = defaultFill;

    // Redraw Lines
    drawGridLines(g);
    
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
    
    // Update all cells on grid
    for (int i = 0; i < g.getHeight(); i++) {
        for (int j = 0; j < g.getWidth(); j++) {
            c = g.grid[i][j].getFill(); // Checks Cell Color
            SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
            SDL_RenderFillRectF(renderer, &g.grid[i][j].cellRect);
        }
    }
    
    c = defaultFill;
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
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
        SDL_RenderDrawLine(renderer, 0, 4*16 + y, windowWidth, 4*16 + y);
    }
}

/////////////////////////////~~~~~~~~~Cell Animations~~~~~~~~~~/////////////////////////////

/*
* Makes Cell small and allows it to grow then shrink to normal size.
* Creates a queue for the sequence of size changes and returns the queue to be used in the AnimationGrid.
*/
std::queue<Cell> RenderWindow::animateCell(Grid g, int i, int j) {

    // Reset coords and size incase other animations are not finished
    // {x, y, w, h}
    float x = (float) g.calculateCellCoords(g.grid[i][j].coord.second);
    float y = (float) g.calculateCellCoords(g.grid[i][j].coord.first) + 4*cellSize;

    float w = cellSize - 1;
    float h = cellSize - 1;

    std::queue<Cell> anim;

    // Grow from 7 px square to 15px
    for (int k = 8; k >= 0; k--) { 
        g.grid[i][j].cellRect = {x + (k/2), y + (k/2), w - k, h - k};
        anim.push(g.grid[i][j]);
    }

    // Draw cell Oversized by 1px

    // for (int k = 1; k <= 1; k++) { 
    g.grid[i][j].cellRect = {x - (1/2), y - (1/2), w + 1, h + 1};
    anim.push(g.grid[i][j]);
    // }
    
    return anim;
}

/*
* Same as the other animation but has a gradient from searchingFill to visitedFill
*/
std::queue<Cell> RenderWindow::animateSearchCell(Grid g, int i, int j) {

    // Reset coords and size incase other animations are not finished
    // {x, y, w, h}
    float x = (float) g.calculateCellCoords(g.grid[i][j].coord.second);
    float y = (float) g.calculateCellCoords(g.grid[i][j].coord.first) + 4*cellSize;

    float w = cellSize - 1;
    float h = cellSize - 1;

    std::queue<Cell> anim;
    
    // RGB multipliers used in loops
    int R = (searchingFill[0] - visitedFill[0])/16;
    int G = (searchingFill[1] - visitedFill[1])/16;
    int B = (searchingFill[2] - visitedFill[2])/16;
 
    std::array<int, 4> currFill = searchingFill;
        
    // Grow from 8px square to 15px
    for (int k = 8; k >= 0; k--) { 
        g.grid[i][j].setSearchFill({currFill[0] - (R*(8-k)), currFill[1] - (G*(8-k)), currFill[2] - (B*(8-k)), 255});
        g.grid[i][j].cellRect = {x + (k/2), y + (k/2), w - k, h - k };
        anim.push(g.grid[i][j]);
    }
    
    currFill = g.grid[i][j].getFill();

    // Draw cell Oversized by 2px
    for (int k = 1; k <= 2; k++) { 
        g.grid[i][j].setSearchFill({currFill[0] - (R*k), currFill[1] - (G*k), currFill[2] - (B*k), 255}); 
        g.grid[i][j].cellRect = {x - (k/2), y - (k/2), w + k, h + k };
        anim.push(g.grid[i][j]);
    }
    
    currFill = g.grid[i][j].getFill();

    // Shrink back to normal size
    for (int k = 1; k <= 2; k++) { 
        g.grid[i][j].setSearchFill({currFill[0] - (R*k), currFill[1] - (G*k), currFill[2] - (B*k), 255});
        g.grid[i][j].cellRect = {x + (k/2), y + (k/2), (w + 2) - k, (h + 2) - k};
        anim.push(g.grid[i][j]);
    }

    g.grid[i][j].setSearchFill(visitedFill);
    g.grid[i][j].cellRect = {x, y, w, h};
    anim.push(g.grid[i][j]);

    return anim;
}


/*
* Same as the other animation but has a gradient from visitedFill to pathFill
*/
std::queue<Cell> RenderWindow::animatePathCell(Grid g, int i, int j) {

    // Reset coords and size incase other animations are not finished
    // {x, y, w, h}
    float x = (float) g.calculateCellCoords(g.grid[i][j].coord.second);
    float y = (float) g.calculateCellCoords(g.grid[i][j].coord.first) + 4*cellSize;

    float w = cellSize - 1;
    float h = cellSize - 1;

    std::queue<Cell> anim;
    
    // RGB multipliers used in loops
    int R = (visitedFill[0] - pathFill[0])/16;
    int G = (visitedFill[1] - pathFill[1])/16;
    int B = (pathFill[2] - visitedFill[2])/16;
 
    std::array<int, 4> currFill = visitedFill;
        
    // Grow from 8px square to 15px
    for (int k = 8; k >= 0; k--) { 
        g.grid[i][j].setSearchFill({currFill[0] - (R*(8-k)), currFill[1] - (G*(8-k)), currFill[2] - (B*(8-k)), 255});
        g.grid[i][j].cellRect = {x + (k/2), y + (k/2), w - k, h - k };
        anim.push(g.grid[i][j]);
    }
    
    currFill = g.grid[i][j].getFill();

    //Draw cell Oversized by 2px
    for (int k = 1; k <= 2; k++) { 
        g.grid[i][j].setSearchFill({currFill[0] - (R*k), currFill[1] - (G*k), currFill[2] - (B*k), 255}); 
        g.grid[i][j].cellRect = {x - (k/2), y - (k/2), w + k, h + k };
        anim.push(g.grid[i][j]);
    }
    
    currFill = g.grid[i][j].getFill();

    // Shrink back to normal size
    for (int k = 1; k <= 2; k++) { 
        g.grid[i][j].setSearchFill({currFill[0] - (R*k), currFill[1] - (G*k), currFill[2] - (B*k), 255});
        g.grid[i][j].cellRect = {x + (k/2), y + (k/2), (w + 2) - k, (h + 2) - k };
        anim.push(g.grid[i][j]);
    }

    g.grid[i][j].setSearchFill(pathFill);
    g.grid[i][j].cellRect = {x, y, w, h};
    anim.push(g.grid[i][j]);

    return anim;
}