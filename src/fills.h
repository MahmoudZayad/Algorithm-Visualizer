#pragma once

#include <array>

// Size of Cell
extern int cellSize;

// Window Size
extern int WIDTH;
extern int HEIGHT;

// Grid
extern std::array<int,4> lineColor;
extern std::array<int,4> defaultFill;

// States
extern std::array<int, 4> highlightFill;

extern std::array<int, 4> startFill; 
extern std::array<int, 4> endFill;  
extern std::array<int, 4> wallFill;

extern std::array<int, 4> visitedFill;
extern std::array<int, 4> pathFill;

extern std::array<int, 4> searchingFill;

// Button
extern std::array<int, 4> buttonFill;
extern std::array<int, 4> buttonHighlight;
extern std::array<int, 4> textFill;

// ComboBox
extern std::array<int, 4> comboClearFill;
