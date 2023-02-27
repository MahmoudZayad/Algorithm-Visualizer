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
std::stack<Cell*> reconstructPath(SDL_Renderer *renderer, std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> &grid, 
                            std::map<Cell*,Cell*> prev, Cell *&end) {

    std::stack<Cell*> path;
    Cell *currentCell = end;
    Cell *previousCell = nullptr;

    int gridWidth = grid[0].size();
    int gridHeight = grid.size();

    path.push(currentCell);
    while (!currentCell->start) {
        previousCell = prev[currentCell];
        path.push(previousCell);  
        previousCell->cellFill = pathFill;
        currentCell = previousCell;
        updateScreen(renderer, grid);
        SDL_Delay(10); // So it does not finish immediately
    }
    return path;
}

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::tuple<int, int>, 4> findChildren(Cell *&root) {

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
    if (rootY + 1 < gridWidth){
        children[1] = std::tuple(rootX, rootY + 1);
    }  else { 
        children[1] = std::tuple(-1,-1); 
    }
    // Check Below
    if (rootX + 1 < gridHeight){
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
void BFS(std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> &grid, Cell &start, SDL_Renderer *renderer) {

    // Declare Queue
    std::queue<Cell*> fringe;
    std::map<Cell*, Cell*> previous;
    Cell *currentCell = nullptr;
    Cell *nextCell = nullptr;
    Cell *end = nullptr;

    currentCell = &start;
    currentCell->visited = true; // Mark root visited
    fringe.push(currentCell);

    while (!fringe.empty()) { // Run till queue is empty
        currentCell = fringe.front();
        fringe.pop();

        if (currentCell->end) {  // End of Search return path
            end = currentCell;
            end->visited = true;
            previous[nullptr] = currentCell;
            updateScreen(renderer, grid);
            SDL_Delay(10); // So it does not finish immediately
            reconstructPath(renderer, grid, previous, end);
            currentCell = nullptr;
            nextCell = nullptr;
            end = nullptr;
            return;
            
        } 

        // Explore children
        std::array<std::tuple<int, int>, 4> children = findChildren(currentCell);

        for (const auto &coord : children) {
            int y = std::get<0>(coord);
            int x = std::get<1>(coord);

            
             if (y < 0 || x < 0 || x >= gridWidth || y >= gridHeight) {
                continue;
            }
            
            // Add children to queue if they have not been visited and are not a wall
            if (!std::get<0>(grid[y][x]).visited && !std::get<0>(grid[y][x]).wall && !std::get<0>(grid[y][x]).start) { 
                std::get<0>(grid[y][x]).visited = true;
                if(!std::get<0>(grid[y][x]).end) {std::get<0>(grid[y][x]).cellFill = visitedFill;}
                
                nextCell = &std::get<0>(grid[y][x]);
                fringe.push(nextCell);
                
                previous[nextCell] = currentCell;
                updateScreen(renderer, grid);
                SDL_Delay(10); // So it does not finish immediately
            }          
        }   
    }
}