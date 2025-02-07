#include "fills.h"

int cellSize = 16;

// Pixel size of Window
int HEIGHT = 480 + 4*16 + 1;
int WIDTH = 640 + 1; 

// Grid
std::array<int,4> lineColor = {22, 22, 22, 255}; // Grid Lines - Barely Black
std::array<int,4> defaultFill = {44, 44, 44, 255}; //  Dark grey

// States
std::array<int, 4> highlightFill = {88, 88, 88, 255}; // lighter grey

std::array<int, 4> startFill = {171, 237, 198, 255};  // Celadon - bluish 
std::array<int, 4> endFill = {237, 171, 198, 255};  // Navy blue  
std::array<int, 4> wallFill = {255, 88, 88, 255}; // red
std::array<int, 4> weightFill = {88, 255, 88, 255}; // Green

std::array<int, 4> visitedFill = {24, 78, 119, 255};  // dark blue
std::array<int, 4> pathFill = {234, 225, 81, 255};  // yellow

std::array<int, 4> searchingFill {217, 237, 146, 255}; // pale green

// Button
std::array<int, 4> buttonFill = {128, 164, 237, 255}; // Blue
std::array<int, 4> buttonHighlight = {0, 137, 123, 255}; // Green
std::array<int, 4> textFill  = {255, 255, 255, 255}; // White