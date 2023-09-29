#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <memory>
#include <string>

namespace yazpgp
{
    class Application
    {
    public:
        struct ApplicationConfig
        {
            std::string title;
            int64_t width;
            int64_t height;
        };
        Application(const ApplicationConfig& config);
        ~Application();

        int run();

    private:
        ApplicationConfig m_config;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
        SDL_GLContext m_context;

        int init_sdl();
        int init_gl();
        int init_imgui();

        /**
         * @brief Update a window with OpenGL rendering and clears buffer
         * 
         * @note not yet :) ..
         * 
         * @return double Delta time in seconds
         */
        double frame();

    
    };
}