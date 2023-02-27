#include "fills.h"

// Grid
std::array<int,4> lineColor = {22, 22, 22, 255}; // Grid Lines - Barely Black
std::array<int,4> defaultFill = {44, 44, 44, 255}; //  Dark grey

// States
std::array<int, 4> highlightFill = {88, 88, 88, 255}; // lighter grey

std::array<int, 4> startFill = {171, 237, 198, 255};  // Celadon - bluish 
std::array<int, 4> endFill = {35, 61, 77, 255};  // Navy blue  
std::array<int, 4> wallFill = {255, 88, 88, 255}; // red

std::array<int, 4> visitedFill = {171, 146, 191, 255};  // purple
std::array<int, 4> pathFill = {234, 225, 81, 255};  // yellow

// Button
std::array<int, 4> buttonFill = {128, 164, 237, 255}; // Blue
std::array<int, 4> buttonHighlight = {0, 137, 123, 255}; // Green
std::array<int, 4> textFill  = {255, 255, 255, 255}; // White