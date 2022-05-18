#pragma once

#include "Color.h"
#include "State.h"
#include "Coords.h"
#include "Renderer.h"

class Tile {
public:
    Tile(int width, int height, bool wall, int x, int y, int j, int i) : m_Width(width), m_Height(height),
                                                                         m_Wall(wall), m_Color(), m_State(State::FRESH),
                                                                         m_i(i), m_j(j) {
        m_Pos.x = x;
        m_Pos.y = y;
    }

    void draw(Renderer renderer);

    friend bool operator<(const Tile &lhs, const Tile &rhs) {
        return lhs.m_EuclideanDist > rhs.m_EuclideanDist;
    }

    bool m_Start = false;
    bool m_End = false;
    int m_Distance = 0;
    int m_Width;
    int m_Height;
    Coords m_Pos;
    bool m_Wall;
    Color m_Color;
    State m_State;
    Tile *m_Pred = nullptr;
    int m_i;
    int m_j;
    double m_EuclideanDist = 0;
};

