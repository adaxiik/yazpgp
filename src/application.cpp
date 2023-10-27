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
#include "asset_storage.hpp"
#include "demo_scenes.hpp"

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

        AssetStorage<Mesh> meshes;
        AssetStorage<Shader> shaders;
        AssetStorage<Texture> textures;

        if (not meshes.add("ball", io::load_mesh_from_file("assets/models/ball.obj"))) return 1;
        if (not meshes.add("cube", io::load_mesh_from_file("assets/models/cube.obj"))) return 1;
        if (not meshes.add("tonk", io::load_mesh_from_file("assets/models/tonk.fbx"))) return 1;
        if (not meshes.add("grid", io::load_mesh_from_file("assets/models/grid20m20x20.obj"))) return 1;
        if (not meshes.add("mad", io::load_mesh_from_file("assets/models/mad.obj"))) return 1;
        if (not meshes.add("plane", io::load_mesh_from_file("assets/models/plane.obj"))) return 1;

        if (not textures.add("tonk", io::load_texture_from_file("assets/textures/tonk_diff.png"))) return 1;
        if (not textures.add("mad", io::load_texture_from_file("assets/textures/mad.png"))) return 1;
        if (not textures.add("grass", io::load_texture_from_file("assets/textures/grass.png"))) return 1;

        if (not shaders.add("white", Shader::create_default_shader(1.f, 1.f, 1.f, 1.f))) return 1;

        if (not shaders.add("normal", io::load_shader_from_file(
            "assets/shaders/normals/normals.vs",
            "assets/shaders/normals/normals.fs"
        ))) return 1;

        
        if (not shaders.add("phong", io::load_shader_from_file(
            "assets/shaders/phong/phong.vs",
            "assets/shaders/phong/phong.fs"
        ))) return 1;

        if (not shaders.add("phong_without_clip", io::load_shader_from_file(
            "assets/shaders/phong_without_clip/phong_without_clip.vs",
            "assets/shaders/phong_without_clip/phong_without_clip.fs"
        ))) return 1;

        if (not shaders.add("phong_textured", io::load_shader_from_file(
            "assets/shaders/phong_textured/phong_textured.vs",
            "assets/shaders/phong_textured/phong_textured.fs"
        ))) return 1;

        if (not shaders.add("lambert", io::load_shader_from_file(
            "assets/shaders/lambert/lambert.vs",
            "assets/shaders/lambert/lambert.fs"
        ))) return 1;

        if (not shaders.add("blinn", io::load_shader_from_file(
            "assets/shaders/blinn/blinn.vs",
            "assets/shaders/blinn/blinn.fs"
        ))) return 1;

        if (not shaders.add("constant", io::load_shader_from_file(
            "assets/shaders/constant/constant.vs",
            "assets/shaders/constant/constant.fs"
        ))) return 1;

        if (not shaders.add("skybox", io::load_shader_from_file(
            "assets/shaders/skybox/skybox.vs",
            "assets/shaders/skybox/skybox.fs"
        ))) return 1;

        if (not shaders.add("rtx", io::load_shader_from_file(
            "assets/shaders/rtx/rtx.vs",
            "assets/shaders/rtx/rtx.fs"
        ))) return 1;

        // auto cubemap_ocean = io::load_cubemap_from_files({
        //     "assets/textures/skybox_ocean/right.jpg",
        //     "assets/textures/skybox_ocean/left.jpg",
        //     "assets/textures/skybox_ocean/top.jpg",
        //     "assets/textures/skybox_ocean/bottom.jpg",
        //     "assets/textures/skybox_ocean/front.jpg",
        //     "assets/textures/skybox_ocean/back.jpg",
        // });
        // if (not cubemap_ocean)
        //     return 1;

        // auto cubemap_factory = io::load_cubemap_from_files({
        //     "assets/textures/skybox_factory/face0.png",
        //     "assets/textures/skybox_factory/face1.png",
        //     "assets/textures/skybox_factory/face2.png",
        //     "assets/textures/skybox_factory/face3.png",
        //     "assets/textures/skybox_factory/face4.png",
        //     "assets/textures/skybox_factory/face5.png",
        // });
        // if (not cubemap_factory)
        //     return 1;

        auto cubemap_forest = io::load_cubemap_from_files({
            "assets/textures/skybox_forest/face0.png",
            "assets/textures/skybox_forest/face1.png",
            "assets/textures/skybox_forest/face2.png",
            "assets/textures/skybox_forest/face3.png",
            "assets/textures/skybox_forest/face4.png",
            "assets/textures/skybox_forest/face5.png",
        });

        auto skybox = std::make_shared<Skybox>(cubemap_forest, shaders["skybox"]);

    

        // scenes
        std::vector<Scene> scenes;

        // Solar system scene
        scenes.push_back(DemoScenes::phong_four_balls(meshes, shaders));
        scenes.push_back(DemoScenes::solar_system(meshes, shaders, *m_window));
        scenes.push_back(DemoScenes::ball_between_light_and_camera(meshes, shaders));
        scenes.push_back(DemoScenes::squish_test(meshes, shaders, textures));
        scenes.emplace_back(std::move(DemoScenes::forest(meshes, shaders, textures).set_skybox(skybox)));
        
        // scenes.push_back(std::move(
        //     Scene()
        //     .add_entity(Scene::SceneRenderableEntity{
        //         .shader = phong_textured_shader,
        //         .mesh = tonk_mesh,
        //         .textures = { tonk_texture },
        //         .transform = Transform::default_transform().rotate({-90.f, 0.f, 0.f}).translate({10.f, 0.f, 0.f}),
        //         .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        //     .add_entity(Scene::SceneRenderableEntity{
        //         .shader = white_shader,
        //         .mesh = grid_mesh,
        //         .transform = Transform::default_transform().translate({0.f, -10.f, 0.f}).scale({5.f, 5.f, 5.f})
        //     })
        //     .add_light(
        //         PointLight()
        //     )
        // ));

        // scenes.push_back(std::move(
        //     Scene()
        //     .add_entity(Scene::SceneRenderableEntity{
        //         .shader = phong_textured_shader,
        //         .mesh = mad_mesh,
        //         .textures = { mad_texture },
        //         .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        //     .add_entity(Scene::SceneRenderableEntity{
        //         .shader = white_shader,
        //         .mesh = grid_mesh,
        //         .transform = Transform::default_transform().translate({0.f, -10.f, 0.f}).scale({5.f, 5.f, 5.f})
        //     })
        //     .add_light(
        //         PointLight().set_position({0.f, 5.f, 3.f})
        //     )
        // ));


        // scenes.push_back(std::move(
        //     Scene()
        //     .add_entity(Scene::SceneRenderableEntity{
        //             .shader = phong_shader,
        //             .mesh = ball_mesh,
        //             .transform = Transform::default_transform().translate({0.0f, 0.0f, 3.0f}),
        //             .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        //     .add_entity(Scene::SceneRenderableEntity{
        //             .shader = lambert_shader,
        //             .mesh = ball_mesh,
        //             .transform = Transform::default_transform().translate({0.0f, 0.0f, -3.0f}),
        //             .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        //     .add_entity(Scene::SceneRenderableEntity{
        //             .shader = ambient_shader,
        //             .mesh = ball_mesh,
        //             .transform = Transform::default_transform().translate({0.0f, 3.0f, 0.0f}),
        //             .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        //     .add_entity(Scene::SceneRenderableEntity{
        //             .shader = blinn_shader,
        //             .mesh = ball_mesh,
        //             .transform = Transform::default_transform().translate({0.0f, -3.0f, 0.0f}),
        //             .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        //     .add_light(
        //         PointLight()
        //     )
        // ));

        // scenes.push_back(std::move(
        //     Scene()
        //     .add_entity(Scene::SceneRenderableEntity{
        //         .shader = phong_textured_shader,
        //         .mesh = tonk_mesh,
        //         .textures = { tonk_texture },
        //         .transform = Transform::default_transform().rotate({-90.f, 0.f, 0.f}).translate({10.f, 0.f, 0.f}),
        //         .material = PhongBlinnMaterial::default_material(),
        //     }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader )
        //     .add_light(
        //         PointLight().set_position({0.f, 3.f, 3.f})
        //     )
        //     .set_skybox(skybox)
        // ));

        // scenes.push_back(std::move(
        //     Scene()
        //     .add_entity(Scene::SceneRenderableEntity{
        //         .shader = rtx_shader,
        //         .mesh = ball_mesh,
        //         .textures = { cubemap_factory },
        //         .transform = Transform::default_transform(),
        //         // .material = PhongBlinnMaterial::default_material(),
        //     })
        //     .set_skybox(skybox)
        // ));

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
                    m_window->width() / (float)m_window->height(),
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
                {
                    current_scene = (current_scene + 1) % scenes.size();
                    scenes[current_scene].invoke_distributors();
                }
                else if (event.key == Key::LEFT)
                {
                    current_scene = (current_scene + scenes.size() - 1) % scenes.size();
                    scenes[current_scene].invoke_distributors();
                }
            }}
        );
        scenes[current_scene].invoke_distributors();
        
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
