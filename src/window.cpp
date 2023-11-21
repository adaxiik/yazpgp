#include "window.hpp"
#include "logger.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace yazpgp
{
    std::unique_ptr<Window> Window::create(const WindowConfig& config)
    {
        //********** SDL **********//

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            YAZPGP_LOG_ERROR("SDL could not initialize! SDL_Error: %s", SDL_GetError());
            return nullptr;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        auto window = SDL_WindowPtr( 
            SDL_CreateWindow(config.title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                config.width,
                config.height,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
            ), SDL_DestroyWindow );

        if (not window)
        {
            YAZPGP_LOG_ERROR("Window could not be created! SDL_Error: %s", SDL_GetError());
            return nullptr;
        }

        auto context = SDL_GL_CreateContext(window.get());
        if (not context)
        {
            YAZPGP_LOG_ERROR("Context could not be created! SDL_Error: %s", SDL_GetError());
            return nullptr;
        }
        SDL_GL_SetSwapInterval(1);

        YAZPGP_LOG_INFO("SDL initialized");
        YAZPGP_LOG_INFO("Resolution: %ux%u", config.width, config.height);

        SDL_version sdl_compiled_version;
        SDL_VERSION(&sdl_compiled_version);
        YAZPGP_LOG_INFO("SDL version: %d.%d.%d", sdl_compiled_version.major, sdl_compiled_version.minor, sdl_compiled_version.patch);


        //********** GLEW **********//

        if (not glewInit() == GLEW_OK)
        {
            YAZPGP_LOG_ERROR("Failed to init glew");
            return nullptr;
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);  
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);

        YAZPGP_LOG_INFO("Glew initialized");

        YAZPGP_LOG_INFO("Using GLEW %s", glewGetString(GLEW_VERSION));
        YAZPGP_LOG_INFO("OpenGL version: %s", glGetString(GL_VERSION));
        YAZPGP_LOG_INFO("Vendor %s", glGetString(GL_VENDOR));
        YAZPGP_LOG_INFO("Renderer %s", glGetString(GL_RENDERER));
        YAZPGP_LOG_INFO("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


        //********** ImGui **********//

        IMGUI_CHECKVERSION();
        if (not ImGui::CreateContext())
        {
            YAZPGP_LOG_ERROR("Failed to init imgui");
            return nullptr;
        }

        // ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        if (not ImGui_ImplSDL2_InitForOpenGL(window.get(), context))
        {
            YAZPGP_LOG_ERROR("Failed to init imgui sdl2");
            return nullptr;
        }

        if (not ImGui_ImplOpenGL3_Init("#version 330"))
        {
            YAZPGP_LOG_ERROR("Failed to init imgui opengl3");
            return nullptr;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        YAZPGP_LOG_INFO("ImGui initialized");

        return std::make_unique<Window>(std::move(window), context);
    }

    void Window::swap_buffers()
    {
        static double last_time = SDL_GetTicks();
        double current_time = SDL_GetTicks();
        m_delta_time = (current_time - last_time) / 1000.0;
        last_time = current_time;
        SDL_GL_SwapWindow(m_window.get());
    }

    Window::~Window()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        YAZPGP_LOG_INFO("ImGui shutdown");

        SDL_GL_DeleteContext(m_context);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);

        YAZPGP_LOG_INFO("SDL shutdown");
    }

    Window::Window(std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>&& window, SDL_GLContext context)
        : m_window(std::move(window))
        , m_context(context)
        , m_input_manager(InputManager())
        , m_is_running(true)
    {
        m_input_manager.add_listener(QuitEvent::Callback{[this](auto) { m_is_running = false; }});

        m_input_manager.add_listener(WindowResizeEvent::Callback{[this](WindowResizeEvent event) {
            glViewport(0, 0, event.width, event.height);
            m_width = event.width;
            m_height = event.height;
        }});
    }

    void Window::clear(glm::vec3 color)
    {
        glClearColor(color.r, color.g, color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    bool Window::is_running() const
    {
        return m_is_running;
    }

    void Window::pool_events()
    {
        m_input_manager.pool_events();
    }

    InputManager& Window::input_manager()
    {
        return m_input_manager;
    }

    void Window::set_relative_mouse_mode(bool enabled) const
    {
        SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
    }

    bool Window::mouse_is_relative() const
    {
        return SDL_GetRelativeMouseMode() == SDL_TRUE;
    }

    double Window::delta_time() const
    {
        return m_delta_time;
    }

    double Window::time() const
    {
        return SDL_GetTicks() / 1000.0;
    }

    int Window::width() const
    {
        return m_width;
    }

    int Window::height() const
    {
        return m_height;
    }

    uint32_t Window::get_stencil_value(int x, int y) const
    {
        uint32_t stencil_value = 0;
        glReadPixels(x, m_height - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil_value);
        return stencil_value;
    }

    float Window::get_depth_value(int x, int y) const
    {
        float depth_value = 0;
        glReadPixels(x, m_height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth_value);
        return depth_value;
    }
}