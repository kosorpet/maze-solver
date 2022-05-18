#include <vector>
#include <string>
#include "Renderer.h"
#include "Tile.h"

#pragma once

class Maze {
public:
    Maze() = default;

    void print();

    void draw(Renderer renderer);

    void toTiles(std::vector<std::string> &textMaze, int windowWidth, int windowHeight, Coords start, Coords end);

    std::vector<std::vector<Tile>> m_Tiles;
};

