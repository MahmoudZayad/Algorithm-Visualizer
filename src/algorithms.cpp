#include "algorithms.h"
#include <iostream>


int speed = Speed_Normal;

/*
* Algorithm Menu
*/
void algorithmsMenu(bool &show_algorithms, int& algorithm) {

    static ImVec2 menuPos = ImVec2(4, (4*16) + 4);
    static ImVec2 menuSize = ImVec2((642/6) + 5*16 ,14*16);
    static ImVec2 dropSize = ImVec2((642/6) + 5*16,2*16);
    
    ImGui::SetNextWindowPos(menuPos);
    // ImGui::SetNextWindowBgAlpha(0.0f);  // Make Transparent
    ImGui::SetNextWindowSize(menuSize);
    ImGui::Begin("Menu", &show_algorithms,  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove); 
    
    if (ImGui::Button("Breadth-first Search", dropSize))  {
        algorithm = Algorithm_BFS;
        show_algorithms = false;
    }                          
    if (ImGui::Button("Depth-first Search", dropSize))  {
        algorithm = Algorithm_DFS;
        show_algorithms = false;
    }                          
    if (ImGui::Button("Iterative Deepening Search", dropSize)) {
        algorithm = Algorithm_IDS;
        show_algorithms = false;
    }                                 
    if (ImGui::Button("Uniform-cost Search", dropSize)) {
        algorithm = Algorithm_UCS;
        show_algorithms = false;
    }                      
    if (ImGui::Button("Greedy Best-first Search", dropSize)) {
        algorithm = Algorithm_Greedy;
        show_algorithms = false;
    }
    if (ImGui::Button("Dijkstra's Algorithm", dropSize)) {
        algorithm = Algorithm_Dijkstra;
        show_algorithms = false;
    }                        
    if (ImGui::Button("A* Search", dropSize)) {
        algorithm = Algorithm_AStar;
        show_algorithms = false;
    }
    ImGui::End();
}

void speedMenu(bool &show_speed) {

    static ImVec2 menuPos = ImVec2(4 + (642/6), (4*16) + 4);
    static ImVec2 menuSize = ImVec2(4*16 ,6*16);
    static ImVec2 dropSize = ImVec2(4*16, 2*16);
    
    ImGui::SetNextWindowPos(menuPos);
    // ImGui::SetNextWindowBgAlpha(0.0f);  // Make Transparent
    ImGui::SetNextWindowSize(menuSize);
    ImGui::Begin("Menu", &show_speed,  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove); 
    
    if (ImGui::Button("Fast", dropSize))  {
        speed = Speed_Fast;
        show_speed = false;
    }                          
    if (ImGui::Button("Normal", dropSize))  {
        speed = Speed_Normal;
        show_speed = false;
    }   
    if (ImGui::Button("Slow", dropSize))  {
        speed = Speed_Slow;
        show_speed = false;
    }                               
    ImGui::End();
}


/*
* Print path taken to end
*/

void printPath(std::stack<Cell*> path) {

    while (!path.empty()) {
        
        std::cout << std::get<0>(path.top()->coord) << " " << std::get<1>(path.top()->coord) << std::endl;
        path.pop();
    }
}

/*
*  Takes map and creates stack to be used to render path
*/
std::stack<Cell*> reconstructPath(RenderWindow &renderWindow, Grid &grid, ImGuiIO& io, std::map<Cell*,Cell*> prev, Cell *&end) {

    std::stack<Cell*> path;
    Cell *currentCell = end;
    Cell *previousCell = nullptr;

    path.push(currentCell);
    while (!currentCell->isStart()) {
        previousCell = prev[currentCell];
        path.push(previousCell);  
        previousCell->setPathFill();
        currentCell = previousCell;
        renderWindow.render(grid,io);
        SDL_Delay(speed); // So it does not finish immediately
    }
    return path;
}

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::tuple<int, int>, 4> findChildren(Grid &g, Cell *&root) {

    std::array<std::tuple<int, int>, 4> children;  // Coords of neighbors

    int rootX = std::get<0>(root->coord);
    int rootY = std::get<1>(root->coord);

    // Check Above
    if (rootX - 1 >= 0){
        children[0] = std::make_tuple(rootX-1, rootY);
    }  else { 
        children[0] = std::make_tuple(-1,-1); 
    }
    // Check Right
    if (rootY + 1 < g.getWidth()){
        children[1] = std::make_tuple(rootX, rootY + 1);
    }  else { 
        children[1] = std::make_tuple(-1,-1); 
    }
    // Check Below
    if (rootX + 1 < g.getHeight()){
        children[2] = std::make_tuple(rootX + 1, rootY);
    }  else { 
        children[2] = std::make_tuple(-1,-1); 
    }
    // Check Behind
    if (rootY - 1 >= 0){
        children[3] = std::make_tuple(rootX, rootY - 1);
    }  else { 
        children[3] = std::make_tuple(-1,-1); 
    }

    return children;
}

// Search Algorithms 
void BFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io) {
    
    // Declare Queue
    std::queue<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;
    Cell *end = nullptr;

   
    currentCell = &g.start;
    currentCell->visit(); // Mark root visited
    fringe.push(currentCell);

    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.front();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            end = currentCell;
            previous[nullptr] = currentCell;
            renderWindow.render(g,io);
            SDL_Delay(speed); // So it does not finish immediately
            reconstructPath(renderWindow, g, io, previous, end);
            currentCell = nullptr;
            nextCell = nullptr;
            end = nullptr;
            return;
        } 

        // Explore children
        std::array<std::tuple<int, int>, 4> children = findChildren(g, currentCell);
        
        for (const auto &coord : children) {
            int y = std::get<0>(coord);
            int x = std::get<1>(coord);

             if (y < 0 || x < 0 || x >= g.getWidth() || y >= g.getHeight()) {
                continue;
            }
            
            // Add children to queue if they have not been visited and are not a wall
            if (!g.grid[y][x].wasVisited() && !g.grid[y][x].isWall() && !g.grid[y][x].isStart()) { 
                
                // Show Current Cell that is being visited
                g.grid[y][x].setSearchFill();
                renderWindow.render(g, io);
                SDL_Delay(speed);
                g.grid[y][x].visit();
            
                nextCell = &g.grid[y][x];
                fringe.push(nextCell);
                
                previous[nextCell] = currentCell;
                renderWindow.render(g, io);
                SDL_Delay(speed); // So it does not finish immediately

                if (g.grid[y][x].isEnd()) { // If it was an end node reset its fill back to end fill
                    g.grid[y][x].setSearchFill();
                }
            }          
        }   
    }
}