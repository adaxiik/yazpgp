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
#include "bezier_list.hpp"
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
        if (not meshes.add("tree", io::load_mesh_from_file("assets/models/tree.obj"))) return 1;
        if (not meshes.add("bush", io::load_mesh_from_file("assets/models/bush.obj"))) return 1;
        if (not meshes.add("suzi", io::load_mesh_from_file("assets/models/suzi.obj"))) return 1;
        if (not meshes.add("rat", io::load_mesh_from_file("assets/models/rat.obj"))) return 1;
        if (not meshes.add("terrain", io::load_mesh_from_file("assets/models/terrain.obj"))) return 1;
        if (not meshes.add("backpack", io::load_mesh_from_file("assets/models/backpack.obj"))) return 1;


        if (not textures.add("tonk", io::load_texture_from_file("assets/textures/tonk_diff.png"))) return 1;
        if (not textures.add("tonk_normal", io::load_texture_from_file("assets/textures/tonk_normal.png"))) return 1;
        if (not textures.add("mad", io::load_texture_from_file("assets/textures/mad.png"))) return 1;
        if (not textures.add("grass", io::load_texture_from_file("assets/textures/grass.png"))) return 1;
        if (not textures.add("rat", io::load_texture_from_file("assets/textures/rat_diff.jpg"))) return 1;
        if (not textures.add("wall", io::load_texture_from_file("assets/textures/brickwall_diff.jpg"))) return 1;
        if (not textures.add("wall_normal", io::load_texture_from_file("assets/textures/brickwall_normal.jpg"))) return 1;
        if (not textures.add("rat_normal", io::load_texture_from_file("assets/textures/rat_normal.png"))) return 1;
        if (not textures.add("backpack", io::load_texture_from_file("assets/textures/backpack_diff.jpg"))) return 1;
        if (not textures.add("backpack_normal", io::load_texture_from_file("assets/textures/backpack_normal.png"))) return 1;


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

        if (not shaders.add("phong_textured_normals", io::load_shader_from_file(
            "assets/shaders/phong_textured_normals/phong_textured_normals.vs",
            "assets/shaders/phong_textured_normals/phong_textured_normals.fs"
        ))) return 1;

        if (not shaders.add("grass", io::load_shader_from_file(
            "assets/shaders/grass/grass.vs",
            "assets/shaders/grass/grass.fs"
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

        auto cubemap_forest = io::load_cubemap_from_files({
            "assets/textures/skybox_forest/face0.png",
            "assets/textures/skybox_forest/face1.png",
            "assets/textures/skybox_forest/face2.png",
            "assets/textures/skybox_forest/face3.png",
            "assets/textures/skybox_forest/face4.png",
            "assets/textures/skybox_forest/face5.png",
        });

        auto cubemap_nightsky = io::load_cubemap_from_files({
            "assets/textures/skybox_nightsky/face0.png",
            "assets/textures/skybox_nightsky/face1.png",
            "assets/textures/skybox_nightsky/face2.png",
            "assets/textures/skybox_nightsky/face3.png",
            "assets/textures/skybox_nightsky/face4.png",
            "assets/textures/skybox_nightsky/face5.png",
        });

        auto skybox_nightsky = std::make_shared<Skybox>(cubemap_nightsky, shaders["skybox"]);
        auto skybox_factory = std::make_shared<Skybox>(cubemap_factory, shaders["skybox"]);
        auto skybox_forest = std::make_shared<Skybox>(cubemap_forest, shaders["skybox"]);
    

        // scenes
        std::vector<Scene> scenes;

        // Solar system scene
        scenes.push_back(DemoScenes::phong_four_balls(meshes, shaders));
        scenes.push_back(DemoScenes::solar_system(meshes, shaders, *m_window));
        // scenes.push_back(DemoScenes::ball_between_light_and_camera(meshes, shaders));
        scenes.push_back(DemoScenes::squish_test(meshes, shaders, textures));
        scenes.emplace_back(std::move(DemoScenes::forest(meshes, shaders, textures).set_skybox(skybox_nightsky)));
        scenes.emplace_back(std::move(DemoScenes::normal_mapping(meshes, shaders, textures).set_skybox(skybox_factory)));
        scenes.emplace_back(std::move(DemoScenes::shell_texturing(meshes, shaders, textures).set_skybox(skybox_forest)));
        scenes.emplace_back(std::move(DemoScenes::terrain(meshes, shaders, textures).set_skybox(skybox_forest)));
        scenes.push_back(DemoScenes::bezier_curve(meshes, shaders));


        float fov = 60.0f;
        glm::mat4 projection_matrix = glm::perspective(fov, (float)m_config.width / (float)m_config.height, 0.1f, 100.0f);
        
        m_window->input_manager().add_listener(
            WindowResizeEvent::Callback{[&](WindowResizeEvent event) {  
                projection_matrix = glm::perspective(
                    glm::radians(fov), 
                    (float)event.width / (float)event.height, 
                    0.1f, 
                    300.0f
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
                    300.0f
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

        // m_window->input_manager().add_listener(
        //     MouseMoveEvent::Callback{[&](MouseMoveEvent event) {
                
                
        //     }}
        // );

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

        constexpr auto bezier_points = 4;
        std::vector<glm::vec3> current_bezier_points;
        BezierList<bezier_points> bezier_list;
        Scene s;
        s.add_entity(Scene::SceneRenderableEntity{
            .shader = shaders["phong"],
            .mesh = meshes["ball"],
            .material = PhongBlinnMaterial::default_material(),
            .transform_modifier = [&](const glm::mat4& m) {
                static float dt = 0.f;
                dt += 0.01f;

                float t = std::sin(dt) * 0.5f + 0.5f;

                glm::vec3 position = bezier_list(t);

                return Transform::Mat4Compositor::Composite({
                    Transform::Mat4Compositor::Translate(position),
                })(m);
            }
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = shaders["phong_textured"],
            .mesh = meshes["terrain"],
            .textures = {textures["grass"]},
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_light(DirectionalLight().set_direction({0.f, -1.f, 0.f}))
        .set_skybox(skybox_forest)
        .camera().move_up(5.f);
        scenes.push_back(std::move(s));
        

        while (m_window->is_running())
        {
            auto& scene = scenes[current_scene];
            m_window->pool_events();
            scene.update(m_window->input_manager(), m_window->delta_time());
            scene.render(projection_matrix);
            DebugUI::scene_window(scene);

            // ImGui::Begin("Info");
            // ImGui::Text("FPS: %.2f", 1.f / m_window->delta_time());
            // ImGui::Text("Imgui FPS: %.2f", ImGui::GetIO().Framerate);
            // ImGui::End();
            auto& input_manager = this->m_window->input_manager();

            uint32_t entity_id_under_mouse = m_window->get_stencil_value(input_manager.mouse_x(), input_manager.mouse_y());
            if (entity_id_under_mouse > 0 and not m_window->mouse_is_relative())
            {
                auto& entity = scene.entities()[entity_id_under_mouse - 1];
                ImGui::Begin("Entity Info", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
                ImGui::SetWindowPos({static_cast<float>(input_manager.mouse_x()), static_cast<float>(input_manager.mouse_y())});
                ImGui::Text("Entity ID: %d", entity_id_under_mouse - 1);
                ImGui::Text("Entity Position: (%f, %f, %f)", entity->transform().position_data.x, entity->transform().position_data.y, entity->transform().position_data.z);
                ImGui::Text("Entity Rotation: (%f, %f, %f)", entity->transform().rotation_data.x, entity->transform().rotation_data.y, entity->transform().rotation_data.z);
                ImGui::Text("Entity Scale: (%f, %f, %f)", entity->transform().scale_data.x, entity->transform().scale_data.y, entity->transform().scale_data.z);
                ImGui::End();
            }

            if (entity_id_under_mouse > 0 and input_manager.get_key_down(Key::T) and not m_window->mouse_is_relative())
            {
                scene.entities().erase(scene.entities().begin() + entity_id_under_mouse - 1);
            }


            if (input_manager.get_key_down(Key::P) or input_manager.get_key_down(Key::B))
            {
                int x = input_manager.mouse_x();
                int y = m_window->height() - input_manager.mouse_y();
                float depth_at = m_window->get_depth_value(x, input_manager.mouse_y());
                glm::vec3 screen_x = glm::vec3(x, y, depth_at);

                glm::vec4 viewport = glm::vec4(0, 0, m_window->width(), m_window->height());
                auto unprojected = glm::unProject(screen_x, scene.camera().view_matrix(), projection_matrix, viewport);

                if (input_manager.get_key_down(Key::P))
                {
                    scene.add_entity(Scene::SceneRenderableEntity{
                        .shader = shaders["phong_textured"],
                        .mesh = meshes["tree"],
                        .textures = {textures["mad"]},
                        .transform = Transform::default_transform().translate(unprojected),
                        .material = PhongBlinnMaterial::default_material(),
                    }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader);
                }

                if (input_manager.get_key_down(Key::B))
                {
                    current_bezier_points.push_back(unprojected);
                    if (current_bezier_points.size() % 4 == 0)
                    {
                        std::array<glm::vec3, bezier_points> points;
                        for (size_t i = 0; i < bezier_points; i++)
                            points[i] = current_bezier_points[i];
                        
                        bezier_list.add_curve(BezierCurve<bezier_points>(points));
                        current_bezier_points.clear();
                        current_bezier_points.push_back(unprojected);
                    }
                }
            }

            this->frame();
        }

        return 0;
    }


}
