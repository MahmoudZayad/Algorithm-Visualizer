
#include "render.h"
#include "algorithms.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#include <iostream>

// Fix wall click to remove wall not working

Grid grid = Grid();
RenderWindow renderWindow = RenderWindow();
// Menu menu = Menu();

int main(int, char**) {

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0,0,0,255); // Set draw color to use on renderer
    SDL_RenderClear(renderer); // Clear the screen to black (in memory atm)

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

   
    int mouseX, mouseY; // Holds grid coordinates of mouse
    SDL_Point mouse;

    // Used to point to cells that need to be updated
    Cell *prevHighlightedCell = nullptr;
    Cell *prevWallCell = nullptr;
    bool leftClick = false; 

    // Main Loop
    bool done = false;
    while (!done) {
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        
            mouseX = (event.motion.x - 1)/cellSize; // -1 is for offset caused by grid lines
            mouseY = (event.motion.y - 4*16 - 1)/cellSize;

            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            switch (event.type){
                // Close Window
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_c: // Clear screen
                            grid = Grid();
                            prevHighlightedCell = nullptr;
                            prevWallCell = nullptr;
                            leftClick = false;
                            break; 
                        case SDLK_SPACE:  // Run BFS 
                            // BFS(, grid);
                            break;
                        default:
                            continue;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN: // WORK ON DRAG EVENT
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT: // Select Wall cell 
                            leftClick = true;            
                            // If wall make it not a wall
                            if (mouseY >= 0) {
                                grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                if (grid.grid[mouseY][mouseX].isWall()) {
                                    grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                } else { // Make it a wall
                                    grid.grid[mouseY][mouseX].wallCellUpdate(true);
                                }
                            }
                            break;
                        }
                case SDL_MOUSEMOTION:
                    // Drag events
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
                    break;
                case SDL_MOUSEBUTTONUP:
                    switch(event.button.button) {
                        case SDL_BUTTON_LEFT:
                            leftClick = false;
                    }
                    break;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();


        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        renderWindow.drawGrid(renderer, grid);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }


    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    renderWindow.destroySDL(renderer, window);
    SDL_Quit();
    return 0;
}