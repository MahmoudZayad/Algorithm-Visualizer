#pragma once

#include <vector>
#include "button.h"

class DropDown {
    public:
        DropDown();
        DropDown(std::vector<std::string> buttonText);

        // Menu Button
        Button dropButton;

        SDL_Rect comboBox;

        std::array<int, 4> comboBoxFill = buttonFill;

        // Drop Down Buttons
        std::vector<Button> ddButtons;

        void clicked();
        bool isClicked();
        
    private:
     bool clickState = false; // State of menu
};