#pragma once

#include "button.h"

Button::Button(char* text, std::tuple<int,int> coord, int height, int width) {
    Button::text = text;
    Button::button.h = height;
    Button::button.w = width;
    Button::button.x = std::get<0>(coord);
    Button::button.y = std::get<0>(coord);
}

/*
 * Make a texture for text to be used on button
*/
SDL_Texture* makeStringTexture(SDL_Renderer* renderer, TTF_Font* font, char* text) {
    
    // Pixels from text
    SDL_Surface* surfaceText = TTF_RenderText_Solid(font, text, {255, 255, 255});

    // Setup texture
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

    // Free surface since we loaded it to the texture
    SDL_FreeSurface(surfaceText);

    return textureText;
}

/*
 * Make textures for all button texts and return a vector to be used to place them on SDL_Rects
*/
std::vector<SDL_Texture*> makeButtonTextures(SDL_Renderer* renderer, TTF_Font* font, std::vector<char*> buttonText) {
    std::vector<SDL_Texture*> textures;
    
    for (auto &text : buttonText) {
        textures.push_back(makeStringTexture(renderer, font, text));
    }

    return textures;
}

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

/*
* Dropdown Box
*/ 

std::vector<SDL_Texture*> dropDownMenu(SDL_Renderer* renderer, TTF_Font* font, std::vector<Button> &buttons) {

    int buttonHeight = 4*cellSize;
    int buttonWidth = std::floor(WIDTH/6);

    std::vector<char*> buttonText;

    // Dropdown button
    buttons.push_back(Button("Algorithms", std::make_tuple(0,0), buttonHeight, buttonWidth));
                    
                     // Size holds four buttons  // Coords
    SDL_Rect window = {buttonHeight*4, WIDTH/6, 0, 5*cellSize}; // Drop down rectangle

    // Dropdown options
    buttons.push_back(Button("Breadth-first Search",std::make_tuple(0,5*cellSize),                    buttonHeight, buttonWidth));
    buttons.push_back(Button("Depth-first Search",  std::make_tuple(0,(5*cellSize)+buttonHeight),     buttonHeight, buttonWidth));
    buttons.push_back(Button("A* Search",           std::make_tuple(0,(5*cellSize)+(2*buttonHeight)), buttonHeight, buttonWidth));
    buttons.push_back(Button("Uniform-cost Search", std::make_tuple(0,(5*cellSize)+(3*buttonHeight)), buttonHeight, buttonWidth));

    for (auto &b : buttons) {
        buttonText.push_back(b.text);
    }

    return makeButtonTextures(renderer, font, buttonText);
}



// Algorithms

// Speed