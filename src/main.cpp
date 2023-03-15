
#include "render.h"
#include <utility>
#include "algorithms.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#include <iostream>


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

    std::vector<std::vector<std::queue<Cell>>> animGrid(grid.getHeight(), std::vector<std::queue<Cell>>(grid.getWidth(), std::queue<Cell>()));


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io =  ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle();
    ImVec2 padding = ImVec2(0,0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, padding);

    // Setup Platform/Renderer backends
    rind.setupRenderPlatform();
   
    int mouseX, mouseY; // Holds grid coordinates of mouse
    SDL_Point mouse;

    // Used to point to cells that need to be updated
    Cell* prevHighlightedCell = nullptr;
    Cell* prevWallCell = nullptr;
    Cell* prevWeightCell = nullptr;
    bool leftClick = false; 

    // Main Loop
    bool done = false;

    // Menu bools
    static bool show_window = true;
    bool show_algorithms = false;
    bool show_weights = false;
    bool show_speed = false;
    bool menu_clicked = false;
    
    // Visualize button
    bool visualize = false;

    // Animation check
    bool anim = false;

    // Algorithms and Grid interactions
    const char* viz = "Visualize";
    int algorithm = Algorithm_None;
    int wei_num = Draw_Wall;

    while (!done) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(rind.getWindow()))
                done = true;
        
            mouseX = (event.motion.x - 1)/cellSize; // -1 is for offset caused by grid lines
            mouseY = (event.motion.y - 4*16)/cellSize;

            mouse.x = event.motion.x;
            mouse.y = event.motion.y;


            if (!menu_clicked) {
                switch (event.type) {
                    case SDL_MOUSEBUTTONDOWN: // WORK ON DRAG EVENT
                        switch (event.button.button) {
                            case SDL_BUTTON_LEFT: // Select Wall cell 
                                leftClick = true;            
                                // If wall make it not a wall
                                if (mouseY >= 0 && !grid.grid[mouseY][mouseX].isStart() && !grid.grid[mouseY][mouseX].isEnd() && !grid.grid[mouseY][mouseX].wasVisited()) {
                                    switch(wei_num) {
                                        case Draw_Wall:
                                            if (grid.grid[mouseY][mouseX].isWall()) {
                                                grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                            } else { // Make it a wall
                                                grid.grid[mouseY][mouseX].wallCellUpdate(true);
                                                animGrid[mouseY][mouseX] = rind.animateCell(grid, mouseY, mouseX);
                                            }
                                            break;
                                        case Draw_Weights:
                                            if (grid.grid[mouseY][mouseX].getWeight() == 15) {
                                                grid.grid[mouseY][mouseX].weightCellUpdate(false);
                                            } else { // Make it a wall
                                                grid.grid[mouseY][mouseX].weightCellUpdate(true);
                                                animGrid[mouseY][mouseX] = rind.animateCell(grid, mouseY, mouseX);
                                            }
                                            break;
                                    }
                                }
                                break;
                            }
                    case SDL_MOUSEMOTION:
                        // Drag events
                        if (mouseY >= 0 && mouseY < grid.getHeight() && 
                            !grid.grid[mouseY][mouseX].isStart() && 
                            !grid.grid[mouseY][mouseX].isEnd() && 
                            !grid.grid[mouseY][mouseX].wasVisited()) { 
                            if (leftClick) {
                                // If Wall make default 
                                switch(wei_num) {
                                    case Draw_Wall:
                                        if (!prevWallCell) {                                 // Update ptr with first wall cell
                                            grid.grid[mouseY][mouseX].wallCellUpdate(true);
                                            prevWallCell = &grid.grid[mouseY][mouseX];
                                            animGrid[mouseY][mouseX] = rind.animateCell(grid, mouseY, mouseX);
                                        } else if (prevHighlightedCell->coord == grid.grid[mouseY][mouseX].coord) { // Do not in same cell
                                            continue;
                                        } else if (!grid.grid[mouseY][mouseX].isWall()) {        // Make wall Cell
                                            grid.grid[mouseY][mouseX].wallCellUpdate(true); 
                                            prevWallCell = &grid.grid[mouseY][mouseX];
                                            animGrid[mouseY][mouseX] = rind.animateCell(grid, mouseY, mouseX);
                                        } else {                                                // Remove Wall Cell
                                            grid.grid[mouseY][mouseX].wallCellUpdate(false);
                                            prevWallCell = &grid.grid[mouseY][mouseX];
                                        }
                                        break;
                                    case Draw_Weights:
                                         if (!prevWeightCell) {                                 // Update ptr with first weight cell
                                            grid.grid[mouseY][mouseX].weightCellUpdate(true);
                                            prevWeightCell = &grid.grid[mouseY][mouseX];
                                            animGrid[mouseY][mouseX] = rind.animateCell(grid, mouseY, mouseX);
                                        } else if (prevHighlightedCell->coord == grid.grid[mouseY][mouseX].coord) { // Do not in same cell
                                            continue;
                                        } else if (grid.grid[mouseY][mouseX].getWeight() == 1) {        // Make Weight Cell
                                            grid.grid[mouseY][mouseX].weightCellUpdate(true); 
                                            prevWeightCell = &grid.grid[mouseY][mouseX];
                                            animGrid[mouseY][mouseX] = rind.animateCell(grid, mouseY, mouseX);
                                        } else {                                                // Remove Weight Cell
                                            grid.grid[mouseY][mouseX].weightCellUpdate(false);
                                            prevWeightCell = &grid.grid[mouseY][mouseX];
                                        }
                                        break;
                                }

                            }
                            // unhighlight cell if mouse goes off screen
                            if (mouse.y < 4*16 + 1 || mouse.x == 0 || mouse.y == HEIGHT-1 || mouse.x == WIDTH-1) {
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
        }

        // Start the Dear ImGui frame
        rind.startImGuiFrame(); 
        
        {
            static ImVec2 pos = ImVec2(0,0);
            static ImVec2 windowSize = ImVec2(640,4*16);
            static ImVec2 buttonSize = ImVec2((642/6),4*16);
            
            ImGui::SetNextWindowPos(pos);
            // ImGui::SetNextWindowBgAlpha(0.0f);  // Make Transparent
            ImGui::SetNextWindowSize(windowSize);
            ImGui::Begin("Buttons", &show_window, ImGuiWindowFlags_NoDecoration |  ImGuiWindowFlags_NoMove); // Remove Bar

            if (ImGui::Button("Algorithms", buttonSize)) {
                if (show_algorithms) {
                    show_algorithms = false;
                    menu_clicked = false;
                } else {
                    show_algorithms = true;
                    menu_clicked = true;
                    show_speed = false;
                    show_weights = false;
                }
            }                
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("Speed", buttonSize)){
                if (show_speed) {
                    show_speed = false;
                    menu_clicked = false;
                } else {
                    show_speed = true;
                    menu_clicked = true;
                    show_algorithms = false;
                    show_weights = false;
                }
            }
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button(viz, buttonSize)) {
                visualize = true;
                show_speed = false;
                menu_clicked = false;
                show_algorithms = false;
                show_weights = false;
            }
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("Clear Walls", buttonSize)) {
                grid.clearWalls();
                grid.clearWeights();
                prevHighlightedCell = nullptr;
                prevWallCell = nullptr;
                leftClick = false;
            }                       
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button("Clear Search", buttonSize)) {
                grid.clearSearch();
                prevHighlightedCell = nullptr;
                prevWallCell = nullptr;
                leftClick = false;
            }
            ImGui::SameLine(0.0f, 0.0f);
            if (ImGui::Button(" Walls n\'\nWeights", buttonSize)) {
                if (show_weights) {
                    show_weights = false;
                    menu_clicked = false;
                } else {
                    show_weights = true;
                    menu_clicked = true;
                    show_speed = false;
                    show_algorithms = false;
                }
            }
            ImGui::End();
        }

        // Menus
        if (show_algorithms) {
            show_speed = false;
            show_weights = false;
            algorithmsMenu(show_algorithms, algorithm);
        } else if (show_speed) {
            show_algorithms = false;
            show_weights = false;
            speedMenu(show_speed);
        } else if (show_weights) {
            show_algorithms = false;
            show_speed = false;
            weightMenu(show_weights, wei_num);
        } else {
            menu_clicked = false;
        }

        if (visualize) {
            visualize = false;
            switch(algorithm) {
                case Algorithm_BFS:
                    grid.clearWeights();
                    BFS(rind, grid, io, animGrid);
                    break;
                case Algorithm_DFS:
                    grid.clearWeights();
                    DFS(rind, grid, io, animGrid);
                    break;
                case Algorithm_UCS:
                    UCS(rind, grid, io, animGrid);
                    break;
            }
        }

        // Rendering
        for (int i = 0; i < grid.getHeight(); i++) {
            for (int j = 0; j < grid.getWidth(); j++) {
                if (!animGrid[i][j].empty()) {
                    grid.grid[i][j] = animGrid[i][j].front();
                    anim = true;
                    animGrid[i][j].pop();
                }
            }
        }  

        if (algorithm == Algorithm_None) {
            viz = " Pick an \nAlgorithm";
        } else {
            viz = "Visualize!";
        }

        rind.render(grid, io);

        if (anim) { // Catch rendering issues
            SDL_Delay(15);
            anim = false;
        }

    }
    // Cleanup
    rind.destroyImGui();
    rind.destroySDL();
    SDL_Quit();

    return 0;
}