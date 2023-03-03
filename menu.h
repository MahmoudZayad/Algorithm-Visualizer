#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <tuple>

#include "fills.h"
#include "button.h"
#include "dropdown.h"


struct Menu {
    Menu();
    int menuHeight; // Size of each cell * 4
    int menuWidth;

    SDL_Rect menuBar;

    // // Push Buttons
    // extern Button visualize;
    // extern Button clear;

    // Radio Button

    // Dropdown menu
    DropDown algorithms;
    // extern DropDown speed;

};


// auto radioFlags = {"wall", "weight", "bomb"}; // Tuple of flags for radio button


