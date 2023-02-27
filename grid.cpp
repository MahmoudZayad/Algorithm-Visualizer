#include "grid.h"

// Pixel size of grid
int WIDTH = 640; 
int HEIGHT = 480;

int cellSize = 16;

// Determine size of grid
int gridHeight = HEIGHT/cellSize;
int gridWidth = WIDTH/cellSize;


/*
* Draws lines for the grid, windowWidth and windowHeight are calculated to create bounds
* for the loops. The bounds are the size of the window.
*/
void drawGridLines(SDL_Renderer *renderer) {
    int windowHeight = gridHeight * cellSize; // Determine window size
    int windowWidth = gridWidth * cellSize;

    std::array<int,4> c = lineColor;
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]); // color off black

    // Draw vertical grid lines
    for (int x = 0; x < 1 + windowWidth; x += cellSize) {
        SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
    }

    // Draw horizontal grid lines.
    for (int y = 0; y < 1 + windowHeight; y += cellSize) {
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }
    SDL_RenderPresent(renderer);
}

/*
* Properly spaces each rectangle for rendering on the grid.
* Uses the rectangle size to determine how many pixels the next rectangle should be shifted.
*/
int calculateCellCoords(int coord) {
    return 1 + (coord*cellSize);
}
 
// /*
// * Purpose is to space Start and End cells 2 Quarters away from each other and one quarter
// * away from the edge width wise. And center them Height wise.
// */
// array<tuple<int, int>, 4> calcStartEndCoords(int gridHeight, int gridWidth, int &cellSize) {
//     // Height divide by 2
//     int vSpace = floor(gridHeight/2);
//     // Width Divide by 4
//     int wSpace = floor(gridWidth/4);

//     if

// }

/*
* Intialize Rectangle with color, size and coord
* sizexy contains the rectangle size, and the indices of the element in the grid.
*/
void intRect(SDL_Renderer *renderer, SDL_Rect &r, std::tuple<int,int> coord) {
    r.h = cellSize;
    r.w = cellSize;
    r.x = calculateCellCoords(std::get<1>(coord)); 
    r.y = calculateCellCoords(std::get<0>(coord));

    int hSpace = gridHeight/2; // For Start and End Coords
    int wSpace = gridWidth/4;

    std::array<int, 4> c = defaultFill;

    if (r.y == hSpace && r.x == wSpace) { // Start
        c = startFill;
    } 
    else if (r.y == hSpace && r.x == 3*wSpace) { // End
        c = endFill;
    }

    
    // Draw Rectangles
    SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]); // default color off black
    SDL_RenderFillRect(renderer, &r);
}

/*
* This creates a 2D-vector with a tuple of Cell and SDL_Rect. These are used to 
* keep track of the abstracted and presented properties of each cell on the grid.
* For loop - updates the vector elements with their coords and rectangle colors.
*/
std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> intializeGrid (SDL_Renderer *renderer, Cell &end, Cell &start) {
    
    std::vector<std::vector<std::tuple<Cell, SDL_Rect>>> grid(gridHeight, 
    std::vector<std::tuple<Cell, SDL_Rect>>(gridWidth, std::make_tuple(Cell(), SDL_Rect())));

    // For start and end placement - calculate spacing
    int hSpace = gridHeight/2;
    int wSpace = gridWidth/4;

    // Add Coord data and update colors for the rectangles
    for (int i = 0; i < gridHeight; i++) {
            for (int j = 0; j < gridWidth; j++) {
                if (i == hSpace && j == wSpace) { // Start coord
                    std::get<0>(grid[i][j]).coord = {i,j};
                    std::get<0>(grid[i][j]).start = true;
                    std::get<0>(grid[i][j]).cellFill = startFill;
                    start = std::get<0>(grid[i][j]);
                } 
                else if (i == hSpace && j == 3*wSpace) { // End Coord
                        std::get<0>(grid[i][j]).coord = {i,j};
                        std::get<0>(grid[i][j]).end = true;
                        std::get<0>(grid[i][j]).cellFill = endFill;
                        end = std::get<0>(grid[i][j]);
                }
                std::get<0>(grid[i][j]).coord = {i,j};
                intRect(renderer, std::get<1>(grid[i][j]), {i,j});
            }
        }

    drawGridLines(renderer);
    
    return grid;
}