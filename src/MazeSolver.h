#include "Maze.h"
#include "Coords.h"
#include "Renderer.h"
#include "Color.h"
#include <fstream>

#pragma once

class MazeSolver {
public:
    MazeSolver() : m_Renderer(1200, 1200, {255, 255, 255}) {
    }

    void init();

    void load(std::ifstream &inputFile);

    void solve(char algo);

private:

    int update();

    void BFS();

    void DFS();

    void randomSearch();

    void DFSRec(Tile &tile);

    void greedySearch();

    void AStar();

    void backtrace(Tile *tile);

    static double tileDist(Tile &t1, Tile &t2);

    void loop();

    Maze m_Maze;
    Coords m_Start;
    Coords m_End;
    Renderer m_Renderer;

    int m_ExpandedNodes = 0;

};
