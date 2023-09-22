#include "application.hpp"
#include "logger.hpp"


#include <GL/gl.h>
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

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
    }

    int Application::init_SDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            YAZPGP_LOG_ERROR("SDL could not initialize! SDL_Error: %s", SDL_GetError());
            return 1;
        }

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

        YAZPGP_LOG_INFO("SDL initialized");
        YAZPGP_LOG_INFO("Resolution: %lux%lu", m_width, m_height);

        return 0;
    }

    int Application::run()
    {
        if (init_SDL())
            return 1;


        glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

        // Camera matrix
        glm::mat4 biew = glm::lookAt(
            glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
            glm::vec3(0, 0, 0),    // and looks at the origin
            glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
        );
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 model = glm::mat4(1.0f);

        bool running = true;
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
                if (event.type == SDL_QUIT)
                    running = false;

            glViewport(0, 0, m_width, m_height);
            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(SDL_GetTicks64()/1000.0 * 50.f, 0.f, 0.f, 1.f);

            glBegin(GL_TRIANGLES);
            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-0.6f, -0.4f, 0.f);
            glColor3f(0.f, 1.f, 0.f);
            glVertex3f(0.6f, -0.4f, 0.f);
            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(0.f, 0.6f, 0.f);
            glEnd();
            SDL_GL_SwapWindow(m_window.get());
        }

        return 0;
    }


}
