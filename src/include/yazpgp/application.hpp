#pragma once

#include <memory>
#include <string>

#include "window.hpp"

namespace yazpgp
{
    class Application
    {
    public:
        struct ApplicationConfig
        {
            std::string title;
            uint32_t width;
            uint32_t height;
        };
        Application(const ApplicationConfig& config);
        ~Application() = default;

        int run();

    private:
        ApplicationConfig m_config;
        std::unique_ptr<Window> m_window;
        void frame();
    };
}