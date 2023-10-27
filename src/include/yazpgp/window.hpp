#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "input_manager.hpp"

namespace yazpgp
{
    class Window
    {
    public:
        using SDL_WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
        struct WindowConfig
        {
            std::string title;
            uint32_t width;
            uint32_t height;
        };

        // Window(const WindowConfig& config);
        static std::unique_ptr<Window> create(const WindowConfig& config);
        Window(std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>&& window, SDL_GLContext context);
        ~Window();
        void swap_buffers();
        void clear(glm::vec3 color);
        bool is_running() const;

        void pool_events();
        InputManager& input_manager();
        void set_relative_mouse_mode(bool enabled) const;
        double delta_time() const;
        double time() const;
        int width() const;
        int height() const;

    private:
        WindowConfig m_config;
        SDL_WindowPtr m_window;
        SDL_GLContext m_context;
        InputManager m_input_manager;
        bool m_is_running;
        double m_delta_time;
        int m_width;
        int m_height;
    };
}