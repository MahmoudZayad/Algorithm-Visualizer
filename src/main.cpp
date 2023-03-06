
#include "render.h"
#include "algorithms.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#include <iostream>

// Fix wall click to remove wall not working

int main(int, char**) {
    // Declare Grid
    Grid grid = Grid();


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

    // Creates window and renderWindow.renderer
    RenderWindow rind = RenderWindow();

    // Setup Dear ImGui context
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io =  ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle();
    ImVec2 padding = ImVec2(0,0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);

    // Setup Platform/Renderer backends
    rind.setupRenderPlatform();
   
    int mouseX, mouseY; // Holds grid coordinates of mouse
    SDL_Point mouse;

    // Used to point to cells that need to be updated
    Cell *prevHighlightedCell = nullptr;
    Cell *prevWallCell = nullptr;
    bool leftClick = false; 

    // Main Loop
    bool done = false;
    while (!done) {
        static bool bfs = false;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(rind.getWindow()))
                done = true;
        
            mouseX = (event.motion.x - 1)/cellSize; // -1 is for offset caused by grid lines
            mouseY = (event.motion.y - 4*16 - 1)/cellSize;

            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            switch (event.type){
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
        rind.startImGuiFrame();

        {
            // static float f = 0.0f;
            static bool show_window = true;
            static int counter = 0; 
            static ImVec2 pos = ImVec2(0,0);
            static ImVec2 windowSize = ImVec2(640,4*16);
            static ImVec2 buttonSize = ImVec2((642/6),4*16);
            
            ImGui::SetNextWindowPos(pos);
            // ImGui::SetNextWindowBgAlpha(0.0f);  // Make Transparent
            ImGui::SetNextWindowSize(windowSize);
            ImGui::Begin("Buttons", &show_window, ImGuiWindowFlags_NoDecoration |  ImGuiWindowFlags_NoMove); // Remove Bar

            // const char* items[] = {"BFS", "DFS"};
            // static int item_current_idx = 0; // Here we store our selection data as an index.
            // const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

            // ImGui::SetNextWindowSizeConstraints(buttonSize, buttonSize);
            // if (ImGui::BeginCombo("Algorithms", "", ImGuiComboFlags_NoPreview))
            // {
            //     for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            //     {
            //         const bool is_selected = (item_current_idx == n);
            //         if (ImGui::Selectable(items[n], is_selected))
            //             item_current_idx = n;

            //         // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            //         if (is_selected)
            //             ImGui::SetItemDefaultFocus();
            //     }
            //     ImGui::EndCombo();
            // }

            if (ImGui::Button("Algorithms", buttonSize)) {
                bfs = true;
            }                
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("Speed", buttonSize))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("Visualize", buttonSize))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("Clear", buttonSize)) {
                grid = Grid();
                prevHighlightedCell = nullptr;
                prevWallCell = nullptr;
                leftClick = false;
            }                       
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("SCoobieofs", buttonSize))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("BEE", buttonSize))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            
            // ImGui::Text("counter = %d", counter);

            // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // // 3. Show another simple window.
        // if (show_another_window)
        // {
        //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //     ImGui::Text("Hello from another window!");
        //     if (ImGui::Button("Close Me"))
        //         show_another_window = false;
        //     ImGui::End();
        // }

        if (bfs) {
            BFS(rind, grid, io);
            bfs = false;
        }

        // Rendering
        rind.render(grid, io);
    }


    // Cleanup
    rind.destroyImGui();

    rind.destroySDL();
    SDL_Quit();
    return 0;
}