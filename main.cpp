#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

int main() {

    // Intialize SDL Modules
    SDL_Init(SDL_INIT_EVERYTHING);

    // Creates window and renderer
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    return 0;
}