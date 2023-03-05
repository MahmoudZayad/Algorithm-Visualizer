#include "dropdown.h"

/*
* Dropdown Box Constructor
*/ 
DropDown::DropDown() {return;}
DropDown::DropDown(std::vector<std::string> buttonText) {
    int buttonHeight = 4*cellSize;
    int buttonWidth = std::floor(WIDTH/6);
    int numButtons = buttonText.size() - 1; // Number of buttons in drop down


    // Menu Button for Dropdown

    DropDown::dropButton = Button(buttonText[0], std::make_tuple(0,0), buttonWidth, buttonHeight);
 

                          // Coords      // Size holds number of buttons
    DropDown::comboBox = {0+4, 5*cellSize, WIDTH/6, buttonHeight*numButtons}; // Drop down rectangle
    
    // Dropdown Buttons
    for (int i = 0; i < buttonText.size()-1; i++) {
            DropDown::ddButtons.push_back(Button(buttonText[i+1],  
                    {0+4,(5*cellSize)+((i)*buttonHeight)}, buttonWidth, buttonHeight));
        }
}


bool DropDown::isClicked() {return clickState;}
void DropDown::clicked() {
        if (dropButton.isClicked()) {
                clickState = true;
        } else {
                clickState = false;
        }
}



