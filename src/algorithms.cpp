

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

void weightMenu(bool &show_weights, int& weight) {

    static ImVec2 menuPos = ImVec2(4 + 642 - (642/6), (4*16) + 4);
    static ImVec2 menuSize = ImVec2(4*16 ,6*16);
    static ImVec2 dropSize = ImVec2(4*16, 2*16);
    
    ImGui::SetNextWindowPos(menuPos);
    // ImGui::SetNextWindowBgAlpha(0.0f);  // Make Transparent
    ImGui::SetNextWindowSize(menuSize);
    ImGui::Begin("Menu", &show_weights,  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove); 
    
    if (ImGui::Button("Walls", dropSize))  {
        weight = Draw_Wall;
        show_weights = false;
    }                          
    if (ImGui::Button("Weights", dropSize))  {
        weight = Draw_Weights;
        show_weights = false;
    }
    if (ImGui::Button("Bombs", dropSize))  {
        weight = Draw_Bombs;
        show_weights = false;
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

    int rootY = std::get<0>(root->coord);
    int rootX = std::get<1>(root->coord);

    // Check Above
    if (rootY - 1 >= 0){
        children[0] = std::make_tuple(rootY - 1, rootX);
    }  else { 
        children[0] = std::make_tuple(-1,-1); 
    }
    // Check Right
    if (rootX + 1 < g.getWidth()){
        children[1] = std::make_tuple(rootY, rootX + 1);
    }  else { 
        children[1] = std::make_tuple(-1,-1); 
    }
    // Check Below
    if (rootY + 1 < g.getHeight()){
        children[2] = std::make_tuple(rootY + 1, rootX);
    }  else { 
        children[2] = std::make_tuple(-1,-1); 
    }
    // Check Behind
    if (rootX - 1 >= 0){
        children[3] = std::make_tuple(rootY, rootX - 1);
    }  else { 
        children[3] = std::make_tuple(-1,-1); 
    }

    return children;
}


// Search Algorithms 

// General Search
void generalSearch(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::function<bool(Cell*,Cell*)> func) {
    
    // Declare Queue and Map for path
        // auto compare = [](Cell* a, Cell* b) {return a->index > b->index;};
    // generalSearch(renderWindow, g, io, compare);
    std::priority_queue<Cell*, std::vector<Cell*>, decltype(func)> fringe(func);
    std::map<Cell*, Cell*> previous;
    

    // Temp pointers
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;

    int index = 1;

    currentCell = &g.start; // Root
    currentCell->index = index;
    currentCell->visit(); 

    fringe.push(currentCell);

    for (const auto &coord : findChildren(g, currentCell)) {
        int y = std::get<0>(coord);
        int x = std::get<1>(coord);
        if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}

        nextCell = &g.grid[y][x];
        index++;
        nextCell->index = index;
        previous[nextCell] = currentCell;
        fringe.push(nextCell);
    }


    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.top();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;

            reconstructPath(renderWindow, g, io, previous, currentCell);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
    
        // Add children to queue if they have not been visited and are not a wall
        if (!currentCell->wasVisited()) {
            // Show Current Cell that is being visited
            currentCell->setSearchFill();
            renderWindow.render(g, io);
            SDL_Delay(speed);
            
            currentCell->visit();

            for (const auto &coord : findChildren(g, currentCell)) {
                int y = std::get<0>(coord);
                int x = std::get<1>(coord);

                // No Neighbor so vals are negative
                if (y < 0 || x < 0) {continue;}

                // Skip if it was visited already
                if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}
            
                std::cout << y << " " << x << std::endl;
                nextCell = &g.grid[y][x];
                index++;
                nextCell->index = index;
                previous[nextCell] = currentCell;
                fringe.push(nextCell);
            }          
        }   
    }
}

// Breadth-first Search
void BFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io) {
    
    // Declare FIFO
    std::queue<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    

    // Temp pointers
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;

    int index = 1;

    
    currentCell = &g.start; // Root
    currentCell->index = index;
    currentCell->visit();

    for (const auto &coord : findChildren(g, currentCell)) {
        int y = std::get<0>(coord);
        int x = std::get<1>(coord);
        
        if (g.grid[y][x].isWall()){continue;}

        nextCell = &g.grid[y][x];
        index++;
        nextCell->index = index;
        previous[nextCell] = currentCell;
        fringe.push(nextCell);
    }


    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.front();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;

            reconstructPath(renderWindow, g, io, previous, currentCell);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
    
        if (!currentCell->wasVisited() && !currentCell->isStart()) {
            // Show Current Cell that is being visited
            currentCell->setSearchFill();
            renderWindow.render(g, io);
            SDL_Delay(speed);
            
            currentCell->visit();

            for (const auto &coord : findChildren(g, currentCell)) {
                int y = std::get<0>(coord);
                int x = std::get<1>(coord);

                // No Neighbor so vals are negative
                if (y < 0 || x < 0) {continue;}

                // Add children to queue if they have not been visited and are not a wall
                if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}
            
                std::cout << y << " " << x << std::endl;
                nextCell = &g.grid[y][x];
                index++;
                nextCell->index = index;
                previous[nextCell] = currentCell;
                fringe.push(nextCell);
            }          
        }   
    }
}

// Depth-first Search
void DFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io) {
    
    // Declare LIFO
    std::stack<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    

    // Temp pointers
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;

    int index = 1;

    currentCell = &g.start; // Root
    currentCell->index = index;
    currentCell->visit(); 

    for (const auto &coord : findChildren(g, currentCell)) {
        int y = std::get<0>(coord);
        int x = std::get<1>(coord);
        if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}

        nextCell = &g.grid[y][x];
        index++;
        nextCell->index = index;
        previous[nextCell] = currentCell;
        fringe.push(nextCell);
    }


    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.top();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;

            reconstructPath(renderWindow, g, io, previous, currentCell);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
    
        // Add children to queue if they have not been visited and are not a wall
        if (!currentCell->wasVisited() && !currentCell->isStart()) {
            // Show Current Cell that is being visited
            currentCell->setSearchFill();
            renderWindow.render(g, io);
            SDL_Delay(speed);
            
            currentCell->visit();

            for (const auto &coord : findChildren(g, currentCell)) {
                int y = std::get<0>(coord);
                int x = std::get<1>(coord);

                // No Neighbor so vals are negative
                if (y < 0 || x < 0) {continue;}

                // Skip if it was visited already
                if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}
            
                std::cout << y << " " << x << std::endl;
                nextCell = &g.grid[y][x];
                index++;
                nextCell->index = index;
                previous[nextCell] = currentCell;
                fringe.push(nextCell);
            }          
        }   
    }
}

// Iterative Deepening Search
void IDS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io) {
    
    int cutoff = g.getWidth()*g.getHeight(); // Width is usually greater than height
    for (int l = 0; l < cutoff; l++) {
        depthLimitedSearch(renderWindow, g, io, l);
    }
}

void depthLimitedSearch(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, int l) {
    
    // Declare LIFO
    Grid resetGrid = g;
    std::stack<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    

    // Temp pointers
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;


    currentCell = &resetGrid.start; // Root
    currentCell->index = 0;
   
    currentCell->visit(); 

    // If l is 0 do not expand children
    if (currentCell->index  < l) {
        for (const auto &coord : findChildren(resetGrid, currentCell)) {
            int y = std::get<0>(coord);
            int x = std::get<1>(coord);
            if (resetGrid.grid[y][x].isWall()){continue;}
            
            nextCell = &resetGrid.grid[y][x];
            nextCell->index = 1; // assign depth
            std::cout << currentCell->index << std::endl;
        
            previous[nextCell] = currentCell;
            fringe.push(nextCell);
            
        }
    }

    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.top();
        fringe.pop();

        

        if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;
            g = resetGrid;

            reconstructPath(renderWindow, g, io, previous, currentCell);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
        
        // Max depth reached
        if (currentCell->index > l) {continue;}


        // Show Current Cell that is being visited 
        currentCell->setSearchFill();
        renderWindow.render(resetGrid, io);
        SDL_Delay(speed);
        currentCell->isStart();
        currentCell->visit(); // For visit fill
        for (const auto &coord : findChildren(resetGrid, currentCell)) {
            int y = std::get<0>(coord);
            int x = std::get<1>(coord);

            // No Neighbor so vals are negative
            if (y < 0 || x < 0) {continue;}

            // Skip if it was visited already or is wall
            if (resetGrid.grid[y][x].isWall()) {continue;}
        
            std::cout << y << " " << x << std::endl;
            nextCell = &resetGrid.grid[y][x];
            nextCell->index = currentCell->index + 1; // assign depth
            previous[nextCell] = currentCell;
            
            fringe.push(nextCell);
        }       
    }
}