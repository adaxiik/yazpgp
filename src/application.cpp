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
        , m_window(nullptr)
    {

    }

    double Application::frame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_window->swap_buffers();
        m_window->clear({0.1f, 0.1f, 0.1f});
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        return 0.0;
    }

    int Application::run()
    {
        m_window = Window::create({
            .title = m_config.title,
            .width = m_config.width,
            .height = m_config.height,
        });

        if (not m_window)
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

        auto rat_texture = io::load_texture_from_file("assets/textures/rat_diff.jpg");
        auto backpack_texture = io::load_texture_from_file("assets/textures/backpack_diff.jpg");
        
        auto normal_shader = io::load_shader_from_file(
            "assets/shaders/normals/normals.vs",
            "assets/shaders/normals/normals.fs"
        );
        if (not normal_shader)
            return 1;

        auto textured_shader = io::load_shader_from_file(
            "assets/shaders/textured/textured.vs",
            "assets/shaders/textured/textured.fs"
        );
        if (not textured_shader)
            return 1;

        #if 0
        Scene scene({
            Scene::SceneRenderableEntity{
                .shader =  normal_shader,
                .mesh   = io::load_mesh_from_file("assets/models/m4.obj")
            },
            Scene::SceneRenderableEntity{
                .shader = textured_shader,
                .mesh = io::load_mesh_from_file("assets/models/rat.obj"),
                .textures = { rat_texture }
            },
            Scene::SceneRenderableEntity{
                .shader = textured_shader,
                .mesh = io::load_mesh_from_file("assets/models/backpack.obj"),
                .textures = { backpack_texture }
            },
            Scene::SceneRenderableEntity{
                .shader = normal_shader,
                .mesh = std::make_shared<Mesh>(
                    sphere_verts, 
                    sizeof(sphere_verts),
                    VertexAttributeLayout({
                        {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                        {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
                }))
            }
        });
        #else
        Scene scene({
            Scene::SceneRenderableEntity{
                .shader = normal_shader,
                .mesh = std::make_shared<Mesh>(
                    sphere_verts, 
                    sizeof(sphere_verts),
                    VertexAttributeLayout({
                        {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                        {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
                }))
            },
            Scene::SceneRenderableEntity{
                .shader = normal_shader,
                .mesh = std::make_shared<Mesh>(
                    sphere_verts, 
                    sizeof(sphere_verts),
                    VertexAttributeLayout({
                        {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
                        {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
                }))
            }
        });
        #endif
        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)m_config.width / (float)m_config.height, 0.1f, 100.0f);
        glm::mat4 view_matrix = glm::lookAt(
            glm::vec3(0, 0, 5), 
            glm::vec3(0, 0, 0), 
            glm::vec3(0, 1, 0)
        );

        auto view_projection_matrix = projection_matrix * view_matrix;

        while (m_window->is_running())
        {
            m_window->pool_events();
     
            scene.render(view_projection_matrix);
            scene.render_scene_imgui_panel();
            (void)this->frame();
        }

        return 0;
    }


}
