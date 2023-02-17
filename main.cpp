#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

int main() {

    // Declare window and renderer structs
    SDL_Window* window = nullptr; // Shows to user
    SDL_Renderer* renderer = nullptr;  // Draws to user
    
    SDL_Init(SDL_INIT_EVERYTHING); // Intialize all SDL modules
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer); // Pass addresses of window and renderer objects
    
    SDL_SetRenderDrawColor(renderer, 0,0,0,255); // Set draw color to use on renderer
    SDL_RenderClear(renderer); // Clear the screen to black (in memory atm)

    SDL_SetRenderDrawColor(renderer, 255,255,255,255); // Set draw color to white
    SDL_RenderDrawPoint(renderer, WIDTH/2, HEIGHT/2); // Draw single pixel at canvas center

    // Now present renderer to window
    SDL_RenderPresent(renderer);
    SDL_Delay(10000); // So program does not close immediately, 10 seconds this case


    return 0;
}