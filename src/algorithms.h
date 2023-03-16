#pragma once

#include <bits/stdc++.h>
#include <map>
#include <stack>
#include <queue>
#include <functional>

#include "render.h"
#include "fills.h"


extern int speed;

enum Speed_ { // Used to set speed of search
    Speed_Fast  = 10,
    Speed_Normal = 30,
    Speed_Slow = 60
};

enum Draw_ { // Used to set speed of search
    Draw_Wall,
    Draw_Weights,
    Draw_Bombs
};

void algorithmsMenu(bool& show_algorithms, int& algorithm);
void speedMenu(bool& show_speed);
void weightMenu(bool &show_weights, int& weight);


/*
* Print path taken to end
*/
void printPath(std::stack<Cell*> path);

/*
*  Takes map and creates stack to be used to render path
*/
std::stack<Cell*> reconstructPath(RenderWindow &renderWindow, Grid &grid, ImGuiIO& io, std::map<Cell*,Cell*> prev, Cell *&end, std::vector<std::vector<std::queue<Cell>>>& animGrid);

/*
* Finds Coords of all children given root and grid size.
*/
std::array<std::pair<int, int>, 4> findChildren(Grid &g, Cell *&root);

// Search Algorithms 
enum Algorithm_ {
    Algorithm_None,
    Algorithm_BFS, // Breadth-first Search
    Algorithm_UCS, // Uniform-cost Search
    Algorithm_DFS, // Depth-first Search
    // Algorithm_IDS, // Iterative Deepening Search
    Algorithm_BDS, // Bidirectional Search
    Algorithm_Greedy, // Greedy Best-first Search
    Algorithm_AStar, // A* Search

};

struct Compare {
    std::pair<int,int> coord;

    int totalCost;

    int index;

    Compare(){}
    Compare(std::pair<int,int> c, int cost, int i) : coord(c), totalCost(cost), index(i) {}
};

void generalSearch(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, 
                   std::vector<std::vector<std::queue<Cell>>>& animGrid,
                   std::function<bool(Compare a, Compare b)> func);
                    
void BFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid);
void DFS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid);
void UCS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid);
// void IDS(RenderWindow &renderWindow, Grid &g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid);
// void depthLimitedSearch(RenderWindow& renderWindow, Grid& g, ImGuiIO& io, std::vector<std::vector<std::queue<Cell>>>& animGrid, int l);

// Bidirectional A∗ search is sometimes more efﬁcient than A∗ itself.
// – IDA∗ (iterative deepening A∗ search) is an iterative deepening version of A∗ , and
// thus adresses the space complexity issue.
// – RBFS (recursive best-ﬁrst search) and SMA∗ (simpliﬁed memory-bounded A∗ )
// are robust, optimal search algorithms that use limited amounts of memory; given
// enough time, they can solve problems for which A∗ runs out of memory.
// 105106
// Chapter 3 Solving Problems by Searching
// – Beam search puts a limit on the size of the frontier; that makes it incomplete
// and suboptimal, but it often ﬁnds reasonably good solutions and runs faster than
// complete searches.
// – Weighted A∗ search focuses the search towards a goal, expanding fewer nodes,
// but sacriﬁcing optimality.