#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <memory>
#include <string>

namespace yazpgp
{
    class Application
    {
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
        SDL_GLContext m_context;
        std::string m_title;
        int64_t m_width;
        int64_t m_height;

        int init_SDL();

    public:
        Application(const std::string& title, int64_t width, int64_t height);
        ~Application();

        int run();
    };
}