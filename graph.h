#ifndef GRAPH
#define GRAPH

#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

// Dark theme.
    SDL_Color bG = {22, 22, 22, 255}; // Background - Barely Black
    SDL_Color gL = {44, 44, 44, 255}; // Grid Lines  - Dark grey
    SDL_Color cellColor = {89, 152, 214, 255}; // light blue

/*
* Vertex
* visited - if vertex has been visited before.
* wall - if user made vertex a wall, walls cannot be traversed.
* cost - cost difference to arrive to this vertex (total edge cost = |v2.cost - v1.cost|)
*/
struct Cell {
    bool visited = false;
    bool wall = false; // Can be updated by user
    int cost = 1; // Default cost, can be adjusted by user

    tuple<int, int> coord; // maybe unnecessary

    // SDL Rectangle object - color
    SDL_Color cellFill;
};

/*
* Intialize Rectangle with color, size and coord
*
*/

void intRect(SDL_Renderer *renderer, SDL_Rect &r, tuple<int,int,int> sizexy) {
    r.h = get<0>(sizexy);
    r.w = get<0>(sizexy);
    r.x = get<1>(sizexy); // Must setup proper spacing with coords
    r.y = get<2>(sizexy);

    cout << "intRect function" << endl;
    SDL_SetRenderDrawColor(renderer, 255, gL.g, gL.b, gL.a); // default color off black
    cout << "intRect function" << endl;
    SDL_RenderDrawRect(renderer, &r);
    cout << "intRect function" << endl;

    SDL_RenderPresent(renderer);

}

#endif