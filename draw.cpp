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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set draw color to white
    SDL_RenderDrawPoint(renderer, WIDTH/2, HEIGHT/2); // Draw single pixel at canvas center

    // Declare rectangle struct
    SDL_Rect rect;
    rect.w = 100; // Declare width and height
    rect.h = 100;
    rect.y = 0; // Declare location
    rect.x = 0;
     SDL_RenderDrawRect(renderer, &rect);

    // Collision detection example

    SDL_Rect rect2;
    rect2.w = 100; // Declare width and height
    rect2.h = 100;
    rect2.y = 50; // Declare location
    rect2.x = 50;
    SDL_RenderDrawRect(renderer, &rect2);

    SDL_Rect intersection; // Collision data

    // Third argument becomes populated with intersection
    // data between first two rectangles
    SDL_IntersectRect(&rect, &rect2, &intersection);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Intersection visualized Red
    SDL_RenderFillRect(renderer, &intersection);

    // Now present renderer to window
    SDL_RenderPresent(renderer);
    SDL_Delay(5000); // So program does not close immediately, 5 seconds in this case


    return 0;
}