#include <SDL2/SDL.h>
#include "Color.h"

#pragma once

/**
 * Class handling the SDL window and renderer
 */
class Renderer {
public:

    Renderer(size_t width, size_t height, Color background);

    void init();

    void present();

    void redrawBackground();

    SDL_Renderer *getRenderer();

    size_t getWindowHeight();

    size_t getWindowWidth();

    void quit() const;

private:
    SDL_Renderer *m_Renderer = nullptr;
    SDL_Window *m_Window = nullptr;

    size_t m_WindowWidth;
    size_t m_WindowHeight;

    Color m_Background;
};


