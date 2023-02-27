#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <vector>
#include <array>
#include <tuple>
#include <string>

#include "fills.h"
#include "grid.h"

// Button
struct Button {
        // Constructor
        Button(char* text, std::tuple<int,int> coord, int height, int width);

        char* text = nullptr;
        std::tuple<int,int> coord;
        std::array<int, 4> fillState = buttonFill; // Blue

        SDL_Rect button;
        SDL_Texture* texture = nullptr;
};

/*
 * Make a texture for text to be used on button
*/
SDL_Texture* makeStringTexture(SDL_Renderer* renderer, TTF_Font* font, char* text);

/*
 * Make textures for all button texts and return a vector to be used to place them on SDL_Rects
*/
std::vector<SDL_Texture*> makeButtonTextures(SDL_Renderer* renderer, TTF_Font* font, std::vector<char*> buttonText);

/*
* Push Button
*/ 

// Visualize

// Clear Board

/*
* Radio Button
*/

// Wall

// Weight

// Bomb





// Algorithms

// Speed