#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/vec3.hpp>                 
#include <glm/vec4.hpp>                 
#include <glm/mat4x4.hpp>               
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>         

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "application.hpp"
#include "logger.hpp"
#include "shader.hpp"
#include "renderable_entity.hpp"
#include "io.hpp"

#include "scene.hpp"

#include "models/suzi_flat.h"
#include "models/sphere.h"


namespace yazpgp
{
    Application::Application(const ApplicationConfig& config)
        : m_config(config)
        , m_window({nullptr, SDL_DestroyWindow})
    {

    }

    Application::~Application()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        
        SDL_GL_DeleteContext(m_context);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        // SDL_Quit();
    }

    int Application::init_sdl()
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
            SDL_CreateWindow(m_config.title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                m_config.width,
                m_config.height,
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
        YAZPGP_LOG_INFO("Resolution: %lux%lu", m_config.width, m_config.height);

        SDL_version sdl_compiled_version;
        SDL_VERSION(&sdl_compiled_version);
        YAZPGP_LOG_INFO("SDL version: %d.%d.%d", sdl_compiled_version.major, sdl_compiled_version.minor, sdl_compiled_version.patch);

        return 0;
    }

    int Application::init_gl()
    {
        if (not glewInit() == GLEW_OK)
        {
            YAZPGP_LOG_ERROR("Failed to init glew");
            return 1;
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);  
        glEnable(GL_DEPTH_TEST);

        YAZPGP_LOG_INFO("Glew initialized");

        YAZPGP_LOG_INFO("Using GLEW %s", glewGetString(GLEW_VERSION));
        YAZPGP_LOG_INFO("OpenGL version: %s", glGetString(GL_VERSION));
        YAZPGP_LOG_INFO("Vendor %s", glGetString(GL_VENDOR));
        YAZPGP_LOG_INFO("Renderer %s", glGetString(GL_RENDERER));
        YAZPGP_LOG_INFO("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

        return 0;
    }

    int Application::init_imgui()
    {
        IMGUI_CHECKVERSION();
        if (not ImGui::CreateContext())
        {
            YAZPGP_LOG_ERROR("Failed to init imgui");
            return 1;
        }

        // ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        if (not ImGui_ImplSDL2_InitForOpenGL(m_window.get(), m_context))
        {
            YAZPGP_LOG_ERROR("Failed to init imgui sdl2");
            return 1;
        }

        if (not ImGui_ImplOpenGL3_Init("#version 330"))
        {
            YAZPGP_LOG_ERROR("Failed to init imgui opengl3");
            return 1;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        YAZPGP_LOG_INFO("ImGui initialized");
        return 0;
    }

    double Application::frame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_window.get());
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        return 0.0;
    }

    int Application::run()
    {
        if (init_sdl())
            return 1;

        if (init_gl())
            return 1;

        if (init_imgui())
            return 1;

        // float tris[] = {
        //     0.0f, 0.5f, 0.0f,
        //     0.5f, -0.5f, 0.0f,
        //     -0.5f, -0.5f, 0.0f
        // };

        // float quad[] = {
        //     -0.1f, 0.1f, 0.0f,
        //     0.1f, 0.1f, 0.0f,
        //     0.1f, -0.1f, 0.0f,

        //     -0.1f, 0.1f, 0.0f,
        //     0.1f, -0.1f, 0.0f,
        //     -0.1f, -0.1f, 0.0f
        // };

        // RenderableEntity triangle_entity(triangle_shader, std::make_shared<Mesh>(
        //     tris, 
        //     sizeof(tris),
        //     VertexAttributeLayout({
        //         {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
        //     })));

        // RenderableEntity quad_entity(quad_shader, std::make_shared<Mesh>(
        //     quad, 
        //     sizeof(quad),
        //     VertexAttributeLayout({
        //         {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
        //     })));

        // RenderableEntity sphere_entity(normal_shader, std::make_shared<Mesh>(
        //     sphere_verts, 
        //     sizeof(sphere_verts),
        //     VertexAttributeLayout({
        //         {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
        //         {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
        //     })));

        // RenderableEntity suzi_entity(normal_shader, std::make_shared<Mesh>(
        //     suzi_flat_verts, 
        //     sizeof(suzi_flat_verts),
        //     VertexAttributeLayout({
        //         {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
        //         {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
        //     })));

        // auto suzi_mesh = io::load_mesh_from_file("models/suzi.obj");
        // RenderableEntity suzi_entity(normal_shader, suzi_mesh);

        auto rat_texure = io::load_texture_from_file("assets/textures/rat_diff.jpg");

        auto normal_shader = io::load_shader_from_file(
            "assets/shaders/normals/normals.vs",
            "assets/shaders/normals/normals.fs"
        );

        if (not normal_shader)
            return 1;

        Scene scene({
            {normal_shader, io::load_mesh_from_file("assets/models/suzi.obj")},
            {normal_shader, io::load_mesh_from_file("assets/models/rat.obj")},
            {
                normal_shader, std::make_shared<Mesh>(
                sphere_verts, 
                sizeof(sphere_verts),
                VertexAttributeLayout({
                    {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                    {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
                }))
            }
        });

        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)m_config.width / (float)m_config.height, 0.1f, 100.0f);
        glm::mat4 view_matrix = glm::lookAt(
            glm::vec3(0, 0, 5), 
            glm::vec3(0, 0, 0), 
            glm::vec3(0, 1, 0)
        );

        auto view_projection_matrix = projection_matrix * view_matrix;

        bool running = true;
        while (running)
        {
            // Ill move this to a separate class later.. trust me c:
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    running = false;

                if (event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        m_config.width = event.window.data1;
                        m_config.height = event.window.data2;
                        glViewport(0, 0,  m_config.width, m_config.height);
                        YAZPGP_LOG_INFO("Resized to %lux%lu", m_config.width, m_config.height);
                    }
                }
                ImGui_ImplSDL2_ProcessEvent(&event);
            }
     
            scene.render(view_projection_matrix);
            scene.render_scene_imgui_panel();
            (void)this->frame();
        }

        return 0;
    }


}
