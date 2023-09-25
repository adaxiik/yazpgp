#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

#include "application.hpp"
#include "logger.hpp"
#include "shader.hpp"
#include "renderable_entity.hpp"




namespace yazpgp
{
    Application::Application(const std::string& title, int64_t width, int64_t height)
        : m_window({nullptr, SDL_DestroyWindow})
        , m_title(title)
        , m_width(width)
        , m_height(height) 
    {

    }

    Application::~Application()
    {
        SDL_GL_DeleteContext(m_context);

        SDL_DestroyWindow(m_window.get());
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        // SDL_Quit();
    }

    int Application::init_SDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            YAZPGP_LOG_ERROR("SDL could not initialize! SDL_Error: %s", SDL_GetError());
            return 1;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetSwapInterval(1);

        m_window = { 
            SDL_CreateWindow(m_title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                m_width,
                m_height,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
            ), SDL_DestroyWindow };

        if (not m_window)
        {
            YAZPGP_LOG_ERROR("Window could not be created! SDL_Error: %s", SDL_GetError());
            return 1;
        }

        m_context = SDL_GL_CreateContext(m_window.get());
        if (not m_context)
        {
            YAZPGP_LOG_ERROR("Context could not be created! SDL_Error: %s", SDL_GetError());
            return 1;
        }

        if (not glewInit() == GLEW_OK)
        {
            YAZPGP_LOG_ERROR("Failed to init glew");
            return 1;
        }

        YAZPGP_LOG_INFO("SDL initialized");
        YAZPGP_LOG_INFO("Resolution: %lux%lu", m_width, m_height);

        return 0;
    }

    double Application::frame()
    {
        SDL_GL_SwapWindow(m_window.get());
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return 0.0;
    }

    int Application::run()
    {
        if (init_SDL())
            return 1;

        auto triangle_shader = Shader::create_default_shader(1.f, 0.f, 0.f, 1.f);
        if (not triangle_shader)
            return 1;

        auto quad_shader = Shader::create_default_shader(0.f, 1.f, 0.f, 1.f);
        if (not quad_shader)
            return 1;


        float tris[] = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };

        float quad[] = {
            -0.1f, 0.1f, 0.0f,
            0.1f, 0.1f, 0.0f,
            0.1f, -0.1f, 0.0f,

            -0.1f, 0.1f, 0.0f,
            0.1f, -0.1f, 0.0f,
            -0.1f, -0.1f, 0.0f
        };

        RenderableEntity triangle_entity(triangle_shader, std::make_shared<Mesh>(tris, sizeof(tris)));
        RenderableEntity quad_entity(quad_shader, std::make_shared<Mesh>(quad, sizeof(quad)));


        bool running = true;
        while (running)
        {
            // Ill move this to a separate class later.. trust me c:
            SDL_Event event;
            while (SDL_PollEvent(&event))
                if (event.type == SDL_QUIT)
                    running = false;
            
            triangle_entity.render();
            quad_entity.render();

            (void)this->frame();
        }

        return 0;
    }


}
