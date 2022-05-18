#include <cstdio>
#include <stdexcept>
#include "Renderer.h"

using namespace std;

Renderer::Renderer(size_t width, size_t height, Color background) : m_WindowWidth(width), m_WindowHeight(height),
                                                                    m_Background(background) {

}

void Renderer::init() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw runtime_error(SDL_GetError());

    m_Window = SDL_CreateWindow("Maze solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowWidth,
                                m_WindowHeight, 0);
    if (!m_Window)
        throw runtime_error(SDL_GetError());

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!m_Renderer) {
        throw runtime_error(SDL_GetError());
    }
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    // SDL_RenderSetLogicalSize(m_Renderer, m_WindowWidth*1.5, m_WindowHeight*1.5); //tell the renderer to work internally with this size, it will be scaled back when displayed on your screen

}

void Renderer::redrawBackground() {
    SDL_SetRenderDrawColor(m_Renderer, m_Background.r, m_Background.g, m_Background.b, 255);
    SDL_RenderClear(m_Renderer);
}

void Renderer::quit() const {
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();
}

void Renderer::present() {

    SDL_RenderPresent(m_Renderer);
}

SDL_Renderer *Renderer::getRenderer() {
    return m_Renderer;
}

size_t Renderer::getWindowHeight() {
    return m_WindowHeight;
}

size_t Renderer::getWindowWidth() {
    return m_WindowWidth;
}


