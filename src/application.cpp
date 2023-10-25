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
#include "phong_blinn_material.hpp"

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

        // normal shader
        auto normal_shader = io::load_shader_from_file(
            "assets/shaders/normals/normals.vs",
            "assets/shaders/normals/normals.fs"
        );
        if (not normal_shader)
            return 1;

        // ball mesh
        auto ball_mesh = io::load_mesh_from_file("assets/models/ball.obj");
        if (not ball_mesh)
            return 1;
        
        // cube mesh
        auto cube_mesh = io::load_mesh_from_file("assets/models/cube.obj");
        if (not cube_mesh)
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

        auto cubemap_ocean = io::load_cubemap_from_files({
            "assets/textures/skybox_ocean/right.jpg",
            "assets/textures/skybox_ocean/left.jpg",
            "assets/textures/skybox_ocean/top.jpg",
            "assets/textures/skybox_ocean/bottom.jpg",
            "assets/textures/skybox_ocean/front.jpg",
            "assets/textures/skybox_ocean/back.jpg",
        });
        if (not cubemap_ocean)
            return 1;

        auto cubemap_factory = io::load_cubemap_from_files({
            "assets/textures/skybox_factory/face0.png",
            "assets/textures/skybox_factory/face1.png",
            "assets/textures/skybox_factory/face2.png",
            "assets/textures/skybox_factory/face3.png",
            "assets/textures/skybox_factory/face4.png",
            "assets/textures/skybox_factory/face5.png",
        });
        if (not cubemap_factory)
            return 1;

        auto skybox_shader = io::load_shader_from_file(
            "assets/shaders/skybox/skybox.vs",
            "assets/shaders/skybox/skybox.fs"
        );
        if (not skybox_shader)
            return 1;

        auto skybox = std::make_shared<Skybox>(cubemap_factory, skybox_shader);


        auto rtx_shader = io::load_shader_from_file(
            "assets/shaders/rtx/rtx.vs",
            "assets/shaders/rtx/rtx.fs"
        );


        auto white_shader = Shader::create_default_shader(1.f, 1.f, 1.f, 1.f);



        // scenes
        std::vector<Scene> scenes;
        Transform::Mat4Compositor center_planet;
        Transform::Mat4Compositor first_planet;
        // Solar system scene
        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                .shader = white_shader,
                .mesh = grid_mesh,
                .transform = Transform::default_transform().translate({0.f, -10.f, 0.f}).scale({5.f, 5.f, 5.f})
            })
            .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_shader,
                .mesh = ball_mesh,
                .material = PhongBlinnMaterial::create_shared({0.f, 0.f, 0.8f}),
                .transform_modifier = [&](const glm::mat4& m) {
                    float angle = m_window->time() * 50;
                    center_planet = m;
                    return m;
                }
            })
            .add_entity(Scene::SceneRenderableEntity{
                .shader = normal_shader,
                .mesh = ball_mesh,
                .transform_modifier = [&](const glm::mat4& m) {
                    float angle = m_window->time() * 100;
                    first_planet = Transform::Mat4Compositor::Composite({
                        Transform::Mat4Compositor::Rotate({0, angle, 0}),
                        Transform::Mat4Compositor::Translate({0, 0, 5}),
                        center_planet,
                    })(m);
                    return first_planet.compose();
                }
            })
            .add_entity(Scene::SceneRenderableEntity{
                .shader = normal_shader,
                .mesh = ball_mesh,
                .transform_modifier = [&](const glm::mat4& m) {
                    float angle = m_window->time() * 150;
                    return Transform::Mat4Compositor::Composite({
                        Transform::Mat4Compositor::Composite({
                            Transform::Mat4Compositor::Rotate({0, angle, 0}),
                            Transform::Mat4Compositor::Translate({0, 0, 3}),
                        }),
                        first_planet,
                    })(m);
            }})
            .add_light(
                PointLight{
                    .position = {0.f, 5.f, 3.f},
                }
            )
        ));

        
        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_textured_shader,
                .mesh = tonk_mesh,
                .textures = { tonk_texture },
                .transform = Transform::default_transform().rotate({-90.f, 0.f, 0.f}).translate({10.f, 0.f, 0.f}),
                .material = PhongBlinnMaterial::default_material(),
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
                .material = PhongBlinnMaterial::default_material(),
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
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, 3.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, -3.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, -3.0f, 0.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = phong_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 3.0f, 0.0f}),
                    .material = PhongBlinnMaterial::default_material(),
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
                    .transform = Transform::default_transform().translate({-5.0f, 0.0f, 0.0f}),
                    .material = PhongBlinnMaterial::default_material(),
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
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, 3.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = lambert_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 0.0f, -3.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = ambient_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, 3.0f, 0.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_entity(Scene::SceneRenderableEntity{
                    .shader = blinn_shader,
                    .mesh = ball_mesh,
                    .transform = Transform::default_transform().translate({0.0f, -3.0f, 0.0f}),
                    .material = PhongBlinnMaterial::default_material(),
            })
            .add_light(
                PointLight{}
            )
        ));

        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_textured_shader,
                .mesh = tonk_mesh,
                .textures = { tonk_texture },
                .transform = Transform::default_transform().rotate({-90.f, 0.f, 0.f}).translate({10.f, 0.f, 0.f}),
                .material = PhongBlinnMaterial::default_material(),
            })
            .add_light(
            PointLight{.position = {0.f, 3.f, 3.f}}
            )
            .set_skybox(skybox)
        ));

        scenes.push_back(std::move(
            Scene()
            .add_entity(Scene::SceneRenderableEntity{
                .shader = rtx_shader,
                .mesh = ball_mesh,
                .textures = { cubemap_factory },
                .transform = Transform::default_transform(),
                // .material = PhongBlinnMaterial::default_material(),
            })
            .add_light(
                PointLight{.position = {0.f, 3.f, 3.f}}
            )
            .set_skybox(skybox)
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
