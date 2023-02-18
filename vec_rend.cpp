#include <SDL2/SDL.h>
#include <vector>

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

int main() {
    
    // Declare Window and Render structs
    SDL_Window* window = nullptr; 
    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING); // Intialize SDL Modules
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer); // Create Window

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer); // Clear Screen Black

    // Create a vector of 100 white points
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    
    vector<SDL_Point> v;
    vector<SDL_Rect> rv;


    // Creat random coords
    for (int i = 0; i < 100; i++) {
        int w = rand()%640;
        int h = rand()%480;
        v.push_back(SDL_Point({w, h}));
        rv.push_back(SDL_Rect({w, h, 10, 10}));
    }   

    // First argument looks for array NOT vector so use data() member function to get array.
    // Second argument needs size of array since array does not contain that information
    // to know how many points there are.
    SDL_RenderDrawPoints(renderer, v.data(), v.size()); 
    SDL_RenderDrawLines(renderer, v.data(), v.size());
    SDL_RenderDrawRects(renderer, rv.data(), rv.size());
    SDL_RenderFillRects(renderer, rv.data(), rv.size());
    SDL_RenderPresent(renderer); // Show to user
    SDL_Delay(5000); // So it does not close immediately
    
    return 0;
}