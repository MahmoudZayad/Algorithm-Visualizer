#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <tuple>

#include "fills.h"

// Button Class

class Button {
    public:
        Button(std::string text, std::tuple<int,int> coord, int width, int height);

        std::string text;
        std::tuple<int,int> coord;
        SDL_Rect button;
        std::array<int, 4> fillState = buttonFill; // Blue

};

Button::Button(std::string text, std::tuple<int,int> coord, int width, int height) {
    Button::text = text;
    Button::button.h = height;
    Button::button.w = width;
    Button::button.x = std::get<0>(coord);
    Button::button.y = std::get<0>(coord);
}