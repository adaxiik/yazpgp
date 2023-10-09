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
#include <iostream>


#include "application.hpp"
#include "logger.hpp"
#include "shader.hpp"
#include "renderable_entity.hpp"
#include "io.hpp"

#include "scene.hpp"

#include "models/suzi_flat.h"
#include "models/sphere.h"

#include "camera.hpp"

namespace yazpgp
{
    Application::Application(const ApplicationConfig& config)
        : m_config(config)
        , m_window(nullptr)
    {

    }

    void Application::frame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_window->swap_buffers();
        m_window->clear({0.1f, 0.1f, 0.1f});
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
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
        
        auto white_shader = Shader::create_default_shader(1.f, 1.f, 1.f, 1.f);

        #if 1
        Scene scene({
            Scene::SceneRenderableEntity{
                .shader = textured_shader,
                .mesh = io::load_mesh_from_file("assets/models/rat.obj"),
                .textures = { rat_texture }
            },
            Scene::SceneRenderableEntity{
                .shader = white_shader,
                .mesh = io::load_mesh_from_file("assets/models/grid20m20x20.obj"),
            },
            Scene::SceneRenderableEntity{
                .shader = white_shader,
                .mesh = io::load_mesh_from_file("assets/models/grid20m20x20.obj"),
                .transform = Transform::default_transform()
                                        .translate({0.0f, 5.0f,0.0f})
            },
            // Scene::SceneRenderableEntity{
            //     .shader = textured_shader,
            //     .mesh = io::load_mesh_from_file("assets/models/backpack.obj"),
            //     .textures = { backpack_texture },
            //     .transform = Transform::default_transform()
            //                             .translate({2.0f, 0.0f, -3.0f})
                                        
            // },
            // Scene::SceneRenderableEntity{
            //     .shader = normal_shader,
            //     .mesh = std::make_shared<Mesh>(
            //         sphere_verts, 
            //         sizeof(sphere_verts),
            //         VertexAttributeLayout({
            //             {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},
            //             {.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE}
            //     }))
            // }
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

        const float fov = glm::radians(60.0f);
        glm::mat4 projection_matrix = glm::perspective(fov, (float)m_config.width / (float)m_config.height, 0.1f, 100.0f);
        
        m_window->input_manager().add_listener(
            WindowResizeEvent::Callback{[&](WindowResizeEvent event) {  
                projection_matrix = glm::perspective(
                    fov, 
                    (float)event.width / (float)event.height, 
                    0.1f, 
                    100.0f
                );
            }}
        );

        m_window->input_manager().add_listener(
            KeyDownEvent::Callback{[&](KeyDownEvent event) {
                static bool mouse_locked = false;
                if (event.key == Key::ESCAPE)
                {
                    mouse_locked = !mouse_locked;
                    m_window->set_relative_mouse_mode(mouse_locked);
                }
            }}
        );

        Camera camera(glm::vec3(-3, 0.3, 0));


        while (m_window->is_running())
        {
            m_window->pool_events();
            camera.update(m_window->input_manager(), m_window->delta_time());
            auto view_projection_matrix = projection_matrix * camera.view_matrix();
          
            // for (auto& entity : scene)
            // {
            //     entity->transform() = Transform::Compositor::Composite({
            //             Transform::Compositor::Rotate({0, 1, 0}),
            //             Transform::Compositor::Composite({
            //                 Transform::Compositor::Translate({0, 0, 0.1}),
            //                 Transform::Compositor::Rotate({0, 1, 0}),
            //             })
            //         })(entity->transform());
            //     if (m_window->input_manager().get_key(Key::A))
            //         entity->transform().rotate({0,1,0});
                
            //     if (m_window->input_manager().get_key(Key::D))
            //         entity->transform().rotate({0,-1,0});// if (m_window->input_manager().get_key(Key::D))
            //         entity->transform().rotate({0,-1,0});

            // }

            ImGui::Begin("Info");
            ImGui::Text("FPS: %f", 1.0f / m_window->delta_time());
            ImGui::End();
     
            scene.render(view_projection_matrix);
            scene.render_scene_imgui_panel();
            this->frame();
        }

        return 0;
    }


}
