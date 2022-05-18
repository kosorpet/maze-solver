#include "Maze.h"
#include <iostream>

using namespace std;

void Maze::print() {
    for (auto &i: m_Tiles) {
        for (auto &j : i) {
            cout << (j.m_Wall ? " " : "X");
        }
        cout << endl;
    }
}

void Maze::draw(Renderer renderer) {
    for (auto &line : m_Tiles) {
        for (auto &tile : line) {
            tile.draw(renderer);
        }
    }
}

void Maze::toTiles(vector<std::string> &textMaze, int windowWidth, int windowHeight, Coords start, Coords end) {
    int width = windowWidth / (int) textMaze[0].size();
    int height = windowHeight / (int) textMaze.size();

    if (width < 1 || height < 1) {
        width = height = 1;
    }

    if (width > height) {
        width = height;
    } else {
        height = width;
    }

    for (int i = 0; i < (int) textMaze.size(); i++) {
        vector<Tile> tileLine;
        for (int j = 0; j < (int) textMaze[i].size(); j++) {
            Tile newTile(width, height, textMaze[i][j] != ' ', j * width, i * height, j, i);

            if (j == start.x && i == start.y) {
                newTile.m_Start = true;
            }

            if (j == end.x && i == end.y) {
                newTile.m_End = true;
            }

            tileLine.push_back(newTile);
        }
        m_Tiles.push_back(tileLine);
    }
}
