#include "algorithms.h"


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
std::stack<Cell*> reconstructPath(RenderWindow &renderWindow, Grid &grid, std::map<Cell*,Cell*> prev, Cell *&end) {

    std::stack<Cell*> path;
    Cell *currentCell = end;
    Cell *previousCell = nullptr;

    path.push(currentCell);
    while (!currentCell->isStart()) {
        previousCell = prev[currentCell];
        path.push(previousCell);  
        previousCell->setPathFill();
        currentCell = previousCell;
        renderWindow.drawGrid(grid);
        SDL_Delay(10); // So it does not finish immediately
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
        children[0] = std::tuple(rootX-1, rootY);
    }  else { 
        children[0] = std::tuple(-1,-1); 
    }
    // Check Right
    if (rootY + 1 < g.getWidth()){
        children[1] = std::tuple(rootX, rootY + 1);
    }  else { 
        children[1] = std::tuple(-1,-1); 
    }
    // Check Below
    if (rootX + 1 < g.getHeight()){
        children[2] = std::tuple(rootX + 1, rootY);
    }  else { 
        children[2] = std::tuple(-1,-1); 
    }
    // Check Behind
    if (rootY - 1 >= 0){
        children[3] = std::tuple(rootX, rootY - 1);
    }  else { 
        children[3] = std::tuple(-1,-1); 
    }

    return children;
}

// Search Algorithms 
void BFS(RenderWindow &renderWindow, Grid &g) {
    
    // Declare Queue
    std::queue<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;
    Cell *end = nullptr;

    currentCell = g.getStart();
    currentCell->visit(); // Mark root visited
    fringe.push(currentCell);

    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.front();
        fringe.pop();

        if (currentCell->isEnd()) {  // End of Search return path
            end = currentCell;
            end->visit();
            previous[nullptr] = currentCell;
            renderWindow.drawGrid(g);
            SDL_Delay(10); // So it does not finish immediately
            reconstructPath(renderWindow, g, previous, end);
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
                
                g.grid[y][x].visit();
            
                nextCell = &g.grid[y][x];
                fringe.push(nextCell);
                
                previous[nextCell] = currentCell;
                renderWindow.drawGrid(g);
                SDL_Delay(10); // So it does not finish immediately
            }          
        }   
    }
}