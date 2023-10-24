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

#include "debug/debug_ui.hpp"

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

        // Normal shader
        auto normal_shader = io::load_shader_from_file(
            "assets/shaders/normals/normals.vs",
            "assets/shaders/normals/normals.fs"
        );
        if (not normal_shader)
            return 1;

        // Ball mesh
        auto ball_mesh = io::load_mesh_from_file("assets/models/ball.obj");
        if (not ball_mesh)
            return 1;

        // phone shader
        auto phong_shader = io::load_shader_from_file(
            "assets/shaders/phong/phong.vs",
            "assets/shaders/phong/phong.fs"
        );
        if (not phong_shader)
            return 1;  

        // tonk mesh
        auto tonk_mesh = io::load_mesh_from_file("assets/models/tonk.fbx");
        if (not tonk_mesh)
            return 1;

        // tonk texture
        auto tonk_texture = io::load_texture_from_file("assets/textures/tonk_diff.png");
        if (not tonk_texture)
            return 1;

        // phong textured shader
        auto phong_textured_shader = io::load_shader_from_file(
            "assets/shaders/phong_textured/phong_textured.vs",
            "assets/shaders/phong_textured/phong_textured.fs"
        );
        if (not phong_textured_shader)
            return 1;

        // grid mesh
        auto grid_mesh = io::load_mesh_from_file("assets/models/grid20m20x20.obj");
        if (not grid_mesh)
            return 1;

        // lambert shader
        auto lambert_shader = io::load_shader_from_file(
            "assets/shaders/lambert/lambert.vs",
            "assets/shaders/lambert/lambert.fs"
        );
        if (not lambert_shader)
            return 1;

         auto blinn_shader = io::load_shader_from_file(
            "assets/shaders/blinn/blinn.vs",
            "assets/shaders/blinn/blinn.fs"
        );
        if (not blinn_shader)
            return 1;

        // ambient
        auto ambient_shader = io::load_shader_from_file(
            "assets/shaders/ambient/ambient.vs",
            "assets/shaders/ambient/ambient.fs"
        );
        if (not ambient_shader)
            return 1;
        
        
        // mad textrure
        auto mad_texture = io::load_texture_from_file("assets/textures/mad.png");
        if (not mad_texture)
            return 1;

        // mad mesh
        auto mad_mesh = io::load_mesh_from_file("assets/models/mad.obj");
        if (not mad_mesh)
            return 1;


        auto white_shader = Shader::create_default_shader(1.f, 1.f, 1.f, 1.f);

        // scenes
        std::vector<Scene> scenes;
        
        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_textured_shader,
                .mesh = tonk_mesh,
                .textures = { tonk_texture },
                .transform = Transform::default_transform().rotate({-90.f, 0.f, 0.f}).translate({10.f, 0.f, 0.f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                .shader = white_shader,
                .mesh = grid_mesh,
                .transform = Transform::default_transform().translate({0.f, -10.f, 0.f}).scale({5.f, 5.f, 5.f})
            })
            .add_light(
                PointLight{}
            )
        ));

        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_textured_shader,
                .mesh = mad_mesh,
                .textures = { mad_texture },
            })
            .add_entity(Scene::SceneRenderableEntity{
                .shader = white_shader,
                .mesh = grid_mesh,
                .transform = Transform::default_transform().translate({0.f, -10.f, 0.f}).scale({5.f, 5.f, 5.f})
            })
            .add_light(
                PointLight{
                    .position = {0.f, 5.f, 3.f},
                }
            )
        ));

        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, 3.0f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, -3.0f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, -3.0f, 0.0f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 3.0f, 0.0f})
            })
            .add_light(
                PointLight{}
            )
        ));

        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({-5.0f, 0.0f, 0.0f})
            })
            .add_light(
                PointLight{}
            )
        ));


        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, 3.0f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = lambert_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, -3.0f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = ambient_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 3.0f, 0.0f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = blinn_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, -3.0f, 0.0f})
            })
            .add_light(
                PointLight{}
            )
        ));

        float fov = 60.0f;
        glm::mat4 projection_matrix = glm::perspective(fov, (float)m_config.width / (float)m_config.height, 0.1f, 100.0f);
        
        m_window->input_manager().add_listener(
            WindowResizeEvent::Callback{[&](WindowResizeEvent event) {  
                projection_matrix = glm::perspective(
                    glm::radians(fov), 
                    (float)event.width / (float)event.height, 
                    0.1f, 
                    100.0f
                );
            }}
        );

        m_window->input_manager().add_listener(
            ScrollEvent::Callback{[&](ScrollEvent event) {
                fov -= event.y_offset * 2;
                projection_matrix = glm::perspective(
                    glm::radians(fov),
                    (float)m_config.width / (float)m_config.height, 
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

        int current_scene = 0;
        m_window->input_manager().add_listener(
            KeyDownEvent::Callback{[&](KeyDownEvent event) {
                if (event.key == Key::RIGHT)
                    current_scene = (current_scene + 1) % scenes.size();
                else if (event.key == Key::LEFT)
                    current_scene = (current_scene + scenes.size() - 1) % scenes.size();
            }}
        );

        while (m_window->is_running())
        {
            auto& scene = scenes[current_scene];
            m_window->pool_events();
            scene.update(m_window->input_manager(), m_window->delta_time());
            scene.render(projection_matrix);
            DebugUI::scene_window(scene);

            this->frame();
        }

        return 0;
    }


}
