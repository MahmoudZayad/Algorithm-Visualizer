# Pathfinding Visualizer
Written in C++, using SDL2 and IMGui.

A user interface to allow for visualization of various pathfinding algorithms.
This project is not yet complete, I intend to add more pathing algorithms and 
the ability to add more end nodes.

#### Current Algorithms included:
Breadth-first, Depth-first, Uniform-cost, Greedy Best-first, and A* search.

## Draw on screen with mouse 
Click to add/remove a cell, or drag to keep drawing walls or weights.
All cells are animated.

![Draw](./gifs/draw_walls.gif)

## Searching 
When searching a animation showing the current cell that is being visited is shown.
![BFS](./gifs/BFS.gif)

## Clearing the Grid
You can clear walls and/or the previous search to run a new one

![DFS](./gifs/DFS.gif)

