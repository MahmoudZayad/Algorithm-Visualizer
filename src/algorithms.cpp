

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
    if (ImGui::Button("Uniform-cost Search", dropSize)) {
        algorithm = Algorithm_UCS;
        show_algorithms = false;
    }                      
    if (ImGui::Button("Greedy Best-first Search", dropSize)) {
        algorithm = Algorithm_Greedy;
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
        
        std::cout << path.top()->coord.first << " " << path.top()->coord.second << std::endl;
        path.pop();
    }
}

/*
*  Takes map and creates stack to be used to render path
*/
std::stack<Cell*> reconstructPath(RenderWindow &renderWindow, Grid &grid, ImGuiIO& io, std::map<Cell*,Cell*> prev, Cell *&end, std::vector<std::vector<std::queue<Cell>>>& animGrid) {

    std::stack<Cell*> path;
    Cell *currentCell = end;
    Cell *previousCell = nullptr;

    path.push(currentCell);
    while (!currentCell->isStart()) {
        previousCell = prev[currentCell];
        path.push(previousCell);  
        currentCell = previousCell;

        animGrid[previousCell->coord.first][previousCell->coord.second] = renderWindow.animatePathCell(grid, previousCell->coord.first, previousCell->coord.second);
        
        // Rendering
        for (int i = 0; i < grid.getHeight(); i++) {
            for (int j = 0; j < grid.getWidth(); j++) {
                if (!animGrid[i][j].empty()) {
                    grid.grid[i][j] = animGrid[i][j].front();
                    animGrid[i][j].pop();
                }
            }
        }       

        renderWindow.render(grid,io);
        SDL_Delay(speed); // So it does not finish immediately
    }

    return path;
}

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::pair<int, int>, 4> findChildren(Grid &g, Cell *&root) {

    std::array<std::pair<int, int>, 4> children;  // Coords of neighbors

    int rootY = root->coord.first;
    int rootX = root->coord.second;

    // Check Above
    if (rootY - 1 >= 0){
        children[0] = {rootY - 1, rootX};
    }  else { 
        children[0] = {-1,-1}; 
    }
    // Check Right
    if (rootX + 1 < g.getWidth()){
        children[1] = {rootY, rootX + 1};
    }  else { 
        children[1] = {-1,-1}; 
    }
    // Check Below
    if (rootY + 1 < g.getHeight()){
        children[2] = {rootY + 1, rootX};
    }  else { 
        children[2] = {-1,-1}; 
    }
    // Check Behind
    if (rootX - 1 >= 0){
        children[3] = {rootY, rootX - 1};
    }  else { 
        children[3] = {-1,-1}; 
    }

    return children;
}


// Search Algorithms 

// // General Search
// void generalSearch(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::function<bool(Cell*,Cell*)> func) {
    
//     // Declare Queue and Map for path
//         // auto compare = [](Cell* a, Cell* b) {return a->index > b->index;};
//     // generalSearch(renderWindow, g, io, compare);
//     std::priority_queue<Cell*, std::vector<Cell*>, decltype(func)> fringe(func);
//     std::map<Cell*, Cell*> previous;
    

//     // Temp pointers
//     Cell *currentCell = nullptr;
//     Cell *nextCell = nullptr;

//     int index = 1;

//     currentCell = &g.start; // Root
//     currentCell->index = index;
//     currentCell->visit(); 

//     fringe.push(currentCell);

//     for (const auto &coord : findChildren(g, currentCell)) {
        // int y = coord.first;
        // int x = coord.second;
//         if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}

//         nextCell = &g.grid[y][x];
//         index++;
//         nextCell->index = index;
//         previous[nextCell] = currentCell;
//         fringe.push(nextCell);
//     }


//     while (!fringe.empty()) { // Run till queue is empty
//         currentCell = fringe.top();
//         fringe.pop();

//         if (currentCell->isEnd()) {  // End of Search return path
//             previous[nullptr] = currentCell;

//             reconstructPath(renderWindow, g, io, previous, currentCell);

//             currentCell = nullptr;
//             nextCell = nullptr;
//             return;
//         } 
    
//         // Add children to queue if they have not been visited and are not a wall
//         if (!currentCell->wasVisited()) {
//             // Show Current Cell that is being visited
//             currentCell->setSearchFill();
//             renderWindow.render(g, io);
//             SDL_Delay(speed);
            
//             currentCell->visit();

//             for (const auto &coord : findChildren(g, currentCell)) {
//                 int y = std::get<0>(coord);
//                 int x = std::get<1>(coord);

//                 // No Neighbor so vals are negative
//                 if (y < 0 || x < 0) {continue;}

//                 // Skip if it was visited already
//                 if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}
            
//                 std::cout << y << " " << x << std::endl;
//                 nextCell = &g.grid[y][x];
//                 index++;
//                 nextCell->index = index;
//                 previous[nextCell] = currentCell;
//                 fringe.push(nextCell);
//             }          
//         }   
//     }
// }

// Breadth-first Search
void BFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid) {
    
    // Declare FIFO
    std::queue<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    

    // Temp pointers
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;

    currentCell = &g.start; // Root
    currentCell->visit();

    for (const auto &coord : findChildren(g, currentCell)) {
        int y = coord.first;
        int x = coord.second;
        
        if (g.grid[y][x].isWall()){continue;}

        nextCell = &g.grid[y][x];
        previous[nextCell] = currentCell;
        fringe.push(nextCell);
    }


    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.front();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;

            reconstructPath(renderWindow, g, io, previous, currentCell, animGrid);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
    
        if (!currentCell->wasVisited()) {
            // Show Current Cell that is being visited
            // currentCell->setSearchFill();
            currentCell->visit();
            animGrid[currentCell->coord.first][currentCell->coord.second] = renderWindow.animateSearchCell(g, currentCell->coord.first, currentCell->coord.second);
            
                // Rendering
            for (int i = 0; i < g.getHeight(); i++) {
                for (int j = 0; j < g.getWidth(); j++) {
                    if (!animGrid[i][j].empty()) {
                        g.grid[i][j] = animGrid[i][j].front();
                        animGrid[i][j].pop();
                    }
                }
            }  
            
            renderWindow.render(g, io);
            SDL_Delay(speed);
            // currentCell->visit();

            for (const auto &coord : findChildren(g, currentCell)) {
                int y = coord.first;
                int x = coord.second;

                // No Neighbor so vals are negative
                if (y < 0 || x < 0) {continue;}

                // Add children to queue if they have not been visited and are not a wall
                if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}
            
                nextCell = &g.grid[y][x];
                previous[nextCell] = currentCell;
                fringe.push(nextCell);
            }          
        }   
    }
}

// Depth-first Search
void DFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid) {
    
    // Declare LIFO
    std::stack<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    

    // Temp pointers
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;

    currentCell = &g.start; // Root
    currentCell->visit();

    for (const auto &coord : findChildren(g, currentCell)) {
        int y = coord.first;
        int x = coord.second;
        
        if (g.grid[y][x].isWall()){continue;}

        nextCell = &g.grid[y][x];
        previous[nextCell] = currentCell;
        fringe.push(nextCell);
    }


    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.top();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;

            reconstructPath(renderWindow, g, io, previous, currentCell, animGrid);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
    
        if (!currentCell->wasVisited()) {
            currentCell->visit();

            // Show Current Cell that is being visited
            animGrid[currentCell->coord.first][currentCell->coord.second] = renderWindow.animateSearchCell(g, currentCell->coord.first, currentCell->coord.second);
            
            // Rendering
            for (int i = 0; i < g.getHeight(); i++) {
                for (int j = 0; j < g.getWidth(); j++) {
                    if (!animGrid[i][j].empty()) {
                        g.grid[i][j] = animGrid[i][j].front();
                        animGrid[i][j].pop();
                    }
                }
            }  
            
            renderWindow.render(g, io);
            SDL_Delay(speed);
            
            for (const auto &coord : findChildren(g, currentCell)) {
                int y = coord.first;
                int x = coord.second;

                // No Neighbor so vals are negative
                if (y < 0 || x < 0) {continue;}

                // Add children to queue if they have not been visited and are not a wall
                if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}
            
                nextCell = &g.grid[y][x];
                previous[nextCell] = currentCell;
                fringe.push(nextCell);
            }          
        }   
    }
}

// Uniform-cost Search
void UCS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid) {
    
    // Declare Priority Queue with Min Heap
    auto compare = [](std::pair<std::pair<int,int>, int> a, std::pair<std::pair<int,int>, int> b) {return a.second > b.second;};
    
    std::priority_queue<std::pair<std::pair<int,int>, int>, std::vector<std::pair<std::pair<int,int>, int>>, decltype(compare)>
    fringe(compare);

    std::map<Cell*, Cell*> previous;
    std::map<std::pair<int,int>, int> costs; // Keep track of total costs to reach node

    // Temp ptrs
    std::pair<std::pair<int,int>, int> currCellCostPair;
    Cell* currentCell = nullptr;

    std::pair<std::pair<int,int>, int> nextCellCostPair;
    Cell* nextCell = nullptr;
    
    costs[g.start.coord] = 0; // Add 0 cost to start node
    fringe.push(std::make_pair(g.start.coord, 0)); // Root
    
    // Set Infinite cost for unvisited nodes
    for (int i = 0; i < g.getHeight(); i++) {
        for (int j = 0; j < g.getWidth(); j++) {
            if (!g.grid[i][j].isStart()) {
                costs[g.grid[i][j].coord] =   INT_MAX; 
            }
        }
    }

    // Run till fringe is empty and goal is found
    while (!fringe.empty()) { 
        currCellCostPair = fringe.top();
        currentCell = &g.grid[currCellCostPair.first.first][currCellCostPair.first.second];
        fringe.pop();

         if (currentCell->isEnd()) {  // End of Search return path
            previous[nullptr] = currentCell;

            reconstructPath(renderWindow, g, io, previous, currentCell, animGrid);

            currentCell = nullptr;
            nextCell = nullptr;
            return;
        } 
    
        if (!currentCell->wasVisited()) {
            currentCell->visit();

            // Show Current Cell that is being visited
            animGrid[currentCell->coord.first][currentCell->coord.second] = renderWindow.animateSearchCell(g, currentCell->coord.first, currentCell->coord.second);
            
            // Rendering
            for (int i = 0; i < g.getHeight(); i++) {
                for (int j = 0; j < g.getWidth(); j++) {
                    if (!animGrid[i][j].empty()) {
                        g.grid[i][j] = animGrid[i][j].front();
                        animGrid[i][j].pop();
                    }
                }
            }  

            renderWindow.render(g, io);
            SDL_Delay(speed);

            for (const auto &coord : findChildren(g, currentCell)) {
                int y = coord.first;
                int x = coord.second;

                // No Neighbor so vals are negative
                if (y < 0 || x < 0) {continue;}

                // Add children to queue if they have not been visited and are not a wall
                if (g.grid[y][x].wasVisited() || g.grid[y][x].isWall()){continue;}

                int altCost = costs[currentCell->coord] + g.grid[y][x].getWeight();

                if (altCost < costs[coord]) { // Check if new path is better
                    
                    costs[coord] = altCost; // Update path length for neighbor

                    nextCellCostPair = std::make_pair(g.grid[y][x].coord, altCost); 
                    nextCell = &g.grid[y][x];
                    previous[nextCell] = currentCell;
                    fringe.push(nextCellCostPair);
                }
            }          
        }   
    }
}

// // Iterative Deepening Search
// void IDS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io) {
    
//     int cutoff = g.getWidth()*g.getHeight(); // Width is usually greater than height
//     for (int l = 0; l < cutoff; l++) {
//         depthLimitedSearch(renderWindow, g, io, l);
//     }
// }

// void depthLimitedSearch(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, int l) {
    
//     // Declare LIFO
//     Grid resetGrid = g;
//     std::stack<Cell*> fringe;
//     std::map<Cell*, Cell*> previous;
    

//     // Temp pointers
//     Cell *currentCell = nullptr;
//     Cell *nextCell = nullptr;


//     currentCell = &resetGrid.start; // Root
//     currentCell->index = 0;
   
//     currentCell->visit(); 

//     // If l is 0 do not expand children
//     if (currentCell->index  < l) {
//         for (const auto &coord : findChildren(resetGrid, currentCell)) {
//             int y = std::get<0>(coord);
//             int x = std::get<1>(coord);
//             if (resetGrid.grid[y][x].isWall()){continue;}
            
//             nextCell = &resetGrid.grid[y][x];
//             nextCell->index = 1; // assign depth
//             std::cout << currentCell->index << std::endl;
        
//             previous[nextCell] = currentCell;
//             fringe.push(nextCell);
            
//         }
//     }

//     while (!fringe.empty()) { // Run till queue is empty
//         currentCell = fringe.top();
//         fringe.pop();

        

//         if (currentCell->isEnd()) {  // End of Search return path
//             previous[nullptr] = currentCell;
//             g = resetGrid;

//             reconstructPath(renderWindow, g, io, previous, currentCell);

//             currentCell = nullptr;
//             nextCell = nullptr;
//             return;
//         } 
        
//         // Max depth reached
//         if (currentCell->index > l) {continue;}


//         // Show Current Cell that is being visited 
//         currentCell->setSearchFill();
//         renderWindow.render(resetGrid, io);
//         SDL_Delay(speed);
//         currentCell->isStart();
//         currentCell->visit(); // For visit fill
//         for (const auto &coord : findChildren(resetGrid, currentCell)) {
//             int y = std::get<0>(coord);
//             int x = std::get<1>(coord);

//             // No Neighbor so vals are negative
//             if (y < 0 || x < 0) {continue;}

//             // Skip if it was visited already or is wall
//             if (resetGrid.grid[y][x].isWall()) {continue;}
        
//             std::cout << y << " " << x << std::endl;
//             nextCell = &resetGrid.grid[y][x];
//             nextCell->index = currentCell->index + 1; // assign depth
//             previous[nextCell] = currentCell;
            
//             fringe.push(nextCell);
//         }       
//     }
// }