#include "button.h"


Button::Button() {
    selectState = false;
    clickState = false;
    fillState = buttonFill;
}


std::array<int, 4> Button::returnFill() {return fillState;}
std::string Button::returnText() {return text;}
bool Button::isClicked() {return clickState;}
bool Button::isSelected() {return selectState;}

Button::Button(std::string text, std::tuple<int,int> coord, int width, int height) {
    Button::text = text;
    Button::button = {std::get<0>(coord), std::get<1>(coord), width, height};
    // x originally + 24 changed to (WIDTH/6)/4
    Button::center = {std::get<0>(coord) + width/4, std::get<1>(coord) + cellSize, width/2, 2*cellSize};

    selectState = false;
    clickState = false;
    fillState = buttonFill;
}

/*
* If mouse hovers in button rectangle highlight it and set it to selected.
*/
void Button::update(SDL_Point& mouse) {
    if (SDL_PointInRect(&mouse, &button)) {
        std::cout << "Highlight" << text << std::endl;
        selectState = true;
        fillState = buttonHighlight;
    } else if (!clickState) {
        selectState = false;
        fillState = buttonFill;
    } else {
        selectState = false;
        fillState = buttonFill;
    }
}

/*
* During Mouse click event check if a button is selected if it was then the click event clicked the button
*/
void Button::clicked() {
    if (selectState) {
        clickState = true;
        fillState = buttonHighlight;
    } else {
        clickState = false;
    }
}
