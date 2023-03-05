#include "menu.h"

Menu::Menu() {
    menuHeight = 4*cellSize; // Size of each cell * 4
    menuWidth = WIDTH;
    menuBar = {0,0, menuWidth, menuHeight};

    // Button visualize = Button("Visualize", std::make_tuple(menuWidth/6,0), menuHeight, menuWidth/6);
    std::vector<std::string> algorithmsText{"Algorithms", "Gamerbois", "Scooby"};
    algorithms = DropDown(algorithmsText);
}



