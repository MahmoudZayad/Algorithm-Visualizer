#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <tuple>
#include <cstring>

#include <iostream>

#include "fills.h"

// Button
class Button {
   public:
        // Constructor
        Button();
        Button(std::string text, std::tuple<int,int> coord, int height, int width);

        // Members
        void update(SDL_Point& mouse);
        void clicked();
        
        bool isClicked();
        bool isSelected();
        std::array<int, 4> returnFill();
        std::string returnText();


        SDL_Rect button;
        SDL_Rect center; // For text to be centered

        SDL_Texture* texture = nullptr;

    private:
        bool selectState;
        bool clickState;
        std::string text;
        std::array<int, 4> fillState; // Blue
};