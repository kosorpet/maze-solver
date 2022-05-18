#include "Tile.h"

void Tile::draw(Renderer renderer) {
    SDL_Rect r{m_Pos.x, m_Pos.y, m_Width, m_Height};

    if (m_Start) {
        m_Color = {255, 0, 0};
    } else if (m_End) {
        m_Color = {0, 0, 255};
    } else {
        switch (m_State) {
            case State::FRESH:
                if (m_Wall) {
                    m_Color = {0, 0, 0};
                } else {
                    m_Color = {255, 255, 255};
                }
                break;
            case State::OPENED:
                m_Color = {0, 255, 0};
                break;
            case State::CLOSED:
                m_Color = {255, 255, 0};
                break;
            case State::PATH:
                m_Color = {100, 100, 100};
                break;
        }
    }
    SDL_SetRenderDrawColor(renderer.getRenderer(), m_Color.r, m_Color.g, m_Color.b, 255);
    SDL_RenderFillRect(renderer.getRenderer(), &r);
}
