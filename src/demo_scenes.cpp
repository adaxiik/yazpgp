#include "demo_scenes.hpp"
#include "phong_blinn_material.hpp"

#include <random>

namespace yazpgp::DemoScenes
{
    Scene phong_four_balls(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders)
    {
        Scene s;
        const auto phong_shader = shaders["phong"];
        const auto ball_mesh = meshes["ball"];

        s.add_entity(Scene::SceneRenderableEntity{
                .shader = phong_shader,
                .mesh = ball_mesh,
                .transform = Transform::default_transform().translate({0.0f, 0.0f, 3.0f}),
                .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_shader,
                .mesh = ball_mesh,
                .transform = Transform::default_transform().translate({0.0f, 0.0f, -3.0f}),
                .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_shader,
                .mesh = ball_mesh,
                .transform = Transform::default_transform().translate({0.0f, -3.0f, 0.0f}),
                .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
                .shader = phong_shader,
                .mesh = ball_mesh,
                .transform = Transform::default_transform().translate({0.0f, 3.0f, 0.0f}),
                .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_light(
            PointLight()
        );

        return s;
    }

    Scene solar_system(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const Window& window)
    {
        Scene s;
        const auto constant_shader = shaders["constant"];
        const auto blinn_shader = shaders["blinn"];
        const auto ball_mesh = meshes["ball"];
        const auto grid_mesh = meshes["grid"];
        const auto white_shader = shaders["white"];

        static Transform::Mat4Compositor center_planet;
        static Transform::Mat4Compositor first_planet;
        static Transform::Mat4Compositor second_planet;

        s.add_entity(Scene::SceneRenderableEntity{
            .shader = white_shader,
            .mesh = grid_mesh,
            .transform = Transform::default_transform().translate({0.f, -10.f, 0.f}).scale({5.f, 5.f, 5.f})
        })
        .add_entity(Scene::SceneRenderableEntity{
            .shader = constant_shader,
            .mesh = ball_mesh,
            .material = PhongBlinnMaterial::create_shared({1.f, 1.f, 0.0f}),
            .transform_modifier = [&](const glm::mat4& m) {
                center_planet = m;
                return m;
            }
        })
        .add_entity(Scene::SceneRenderableEntity{
            .shader = blinn_shader,
            .mesh = ball_mesh,
            .material = PhongBlinnMaterial::create_shared({0.8f, 0.0f, 0.f}),
            .transform_modifier = [&](const glm::mat4& m) {
                float angle = window.time() * 100;
                first_planet = Transform::Mat4Compositor::Composite({
                    Transform::Mat4Compositor::Rotate({0, angle, 0}),
                    Transform::Mat4Compositor::Translate({0, 0, 5}),
                    center_planet,
                })(m);
                return first_planet.compose();
            }
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = blinn_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().scale({0.7f, 0.7f, 0.7f}),
            .material = PhongBlinnMaterial::create_shared({0.0f, 0.0f, 0.4f}),
            .transform_modifier = [&](const glm::mat4& m) {
                float angle = window.time() * 150;
                return Transform::Mat4Compositor::Composite({
                    Transform::Mat4Compositor::Composite({
                        Transform::Mat4Compositor::Rotate({0, angle, 0}),
                        Transform::Mat4Compositor::Translate({0, 0, 8}),
                    }),
                    first_planet,
                })(m);
        }}, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = blinn_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().scale({0.8f, 0.8f, 0.8f}),
            .material = PhongBlinnMaterial::create_shared({0.0f, 0.6f, 0.0f}),
            .transform_modifier = [&](const glm::mat4& m) {
                float angle = window.time() * 10;
                second_planet = Transform::Mat4Compositor::Composite({
                    Transform::Mat4Compositor::Composite({
                        Transform::Mat4Compositor::Rotate({0, -angle, 0}),
                        Transform::Mat4Compositor::Translate({0, 0, 20}),
                    }),
                    center_planet,
                })(m);
                return second_planet.compose();
        }}, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = blinn_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().scale({0.7f, 0.7f, 0.7f}),
            .material = PhongBlinnMaterial::create_shared({0.6f, 0.0f, 0.0f}),
            .transform_modifier = [&](const glm::mat4& m) {
                float angle = window.time() * 50;
                return Transform::Mat4Compositor::Composite({
                    Transform::Mat4Compositor::Composite({
                        Transform::Mat4Compositor::Rotate({0, angle, 0}),
                        Transform::Mat4Compositor::Translate({0, 0, 10}),
                    }),
                    second_planet,
                })(m);
        }}, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_light(
            PointLight()
        );

        return s;
    }


    Scene ball_between_light_and_camera(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders)
    {
        Scene s;
        const auto phong_shader = shaders["phong"];
        const auto ball_mesh = meshes["ball"];
        const auto phong_wihout_clip_shader = shaders["phong_without_clip"];
        const auto grid_mesh = meshes["grid"];
        const auto white_shader = shaders["white"];

        s.add_entity(Scene::SceneRenderableEntity{
            .shader = phong_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().translate({-5.0f, 0.0f, 0.0f}),
            .material = PhongBlinnMaterial::create_shared(glm::vec3(1.f), 1.f)
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = phong_wihout_clip_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().translate({5.0f, 0.0f, 0.0f}),
            .material = PhongBlinnMaterial::create_shared(glm::vec3(1.f), 1.f)
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = white_shader,
            .mesh = grid_mesh,
            .transform = Transform::default_transform().translate({0.f, -2.f, 0.f})
        })
        .add_light(
            PointLight()
        );

        return s;
    }

    Scene squish_test(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures)
    {
        Scene s;
        const auto phong_shader = shaders["phong"];
        const auto phong_textured_shader = shaders["phong_textured"];
        const auto tonk_mesh = meshes["tonk"];
        const auto ball_mesh = meshes["ball"];
        const auto mad_mesh = meshes["mad"];
        const auto tonk_texture = textures["tonk"];
        const auto mad_mesh_texture = textures["mad"];

        s.add_entity(Scene::SceneRenderableEntity{
            .shader = phong_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().translate({0.0f, 0.0f, 0.0f}),
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = phong_textured_shader,
            .mesh = tonk_mesh,
            .textures = {tonk_texture},
            .transform = Transform::default_transform().translate({0.f, 3.f, -8.f}).rotate({-90.f, 0.f, 0.f}),
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = phong_textured_shader,
            .mesh = mad_mesh,
            .textures = {mad_mesh_texture},
            .transform = Transform::default_transform().translate({0.f, 0.f, 8.f}),
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_light(
            PointLight().set_position({0.f, 5.f, 5.f})
        );

        return s;
    }

    Scene forest(const AssetStorage<Mesh>& meshes, const AssetStorage<Shader>& shaders, const AssetStorage<Texture>& textures)
    {
        Scene s;
        const auto phong_textured_shader = shaders["phong_textured"];
        const auto blinn_shader = shaders["blinn"];
        const auto rtx_shader = shaders["rtx"];
        const auto plane_mesh = meshes["plane"];
        const auto tree_mesh = meshes["tree"];
        const auto tree_texture = textures["mad"];
        const auto bush_mesh = meshes["bush"];
        const auto ball_mesh = meshes["ball"];
        const auto suzi_mesh = meshes["suzi"];
        const auto normal_shader = shaders["normal"];
        const auto tonk_mesh = meshes["tonk"];
        const auto tonk_texture = textures["tonk"];
        const auto rat_mesh = meshes["rat"];
        const auto rat_texture = textures["rat"];


        s.add_entity(Scene::SceneRenderableEntity{
            .shader = phong_textured_shader,
            .mesh = plane_mesh,
            .textures = {textures["grass"]},
            .transform = Transform::default_transform().translate({0.f, 0.f, 0.f}).scale({20.f, 1.f, 20.f}),
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_light(
            PointLight().set_position({0.f, 50.f, 50.f})
        );
        
        std::mt19937 gen(69);
        std::uniform_real_distribution<> dis(-15.0, 15.0);
        std::uniform_real_distribution<> scale_dis(0.5, 1.5);
        std::uniform_real_distribution<> rot_dis(0.0, 360.0);

        for (int i = 0; i < 30; i++)
        {
            float scale = scale_dis(gen);
            s.add_entity(Scene::SceneRenderableEntity{
                .shader = phong_textured_shader,
                .mesh = tree_mesh,
                .textures = {tree_texture},
                .transform = Transform::default_transform()
                    .translate({dis(gen), 0.f, dis(gen)})
                    .scale({scale, scale, scale})
                    .rotate({0.f, rot_dis(gen), 0.f}),
                .material = PhongBlinnMaterial::default_material(),
            }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader);
        

            s.add_entity(Scene::SceneRenderableEntity{
                .shader = phong_textured_shader,
                .mesh = bush_mesh,
                .textures = {tree_texture},
                .transform = Transform::default_transform()
                    .translate({dis(gen), 0.f, dis(gen)})
                    .scale({scale+1.0f, scale+1.0f, scale+1.0f})
                    .rotate({0.f, rot_dis(gen), 0.f}),
                .material = PhongBlinnMaterial::default_material(),
            }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader);
        
        }

        s.add_entity(Scene::SceneRenderableEntity{
            .shader = rtx_shader,
            .mesh = ball_mesh,
            .transform = Transform::default_transform().translate({0.f, 10.f, 0.f}),
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = phong_textured_shader,
            .mesh = tonk_mesh,
            .textures = {tonk_texture},
            .transform = Transform::default_transform()
                .translate({0.f, 2.2f, 0.f})
                .rotate({-90.f, 0.f, 0.f})
                .scale({0.3f, 0.3f, 0.3f}),
            .material = PhongBlinnMaterial::default_material(),
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader)
        .add_entity(Scene::SceneRenderableEntity{
            .shader = phong_textured_shader,
            .mesh = rat_mesh,
            .textures = {rat_texture},
            .transform = Transform::default_transform().translate({0.f, 0.f, 3.f}),
            .material = PhongBlinnMaterial::default_material(),
            .transform_modifier = [&](const glm::mat4& m) {
                static float angle = 0.f;
                angle += 0.5f;
                return Transform::Mat4Compositor::Composite({
                    Transform::Mat4Compositor::Rotate({0, angle, 0}),
                    m
                })();
            }
        }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader);


        constexpr int num_balls = 100;
        constexpr float rot = 360.f / num_balls;

        for (int i = 0; i < num_balls; i++)
        {
            const float pos_x = 10.f * std::cos(glm::radians(rot * i));
            const float pos_z = 10.f * std::sin(glm::radians(rot * i)); 

            if (i % 2)
            {
                s.add_entity(Scene::SceneRenderableEntity{
                    .shader = blinn_shader,
                    .mesh = ball_mesh,
                    .textures = {tree_texture},
                    .transform = Transform::default_transform().translate({pos_x, 2.f, pos_z}).scale({0.3f, 0.3f, 0.3f}),
                    .material = PhongBlinnMaterial::default_material(),
                }, Scene::AddEntityOptions::PassLightToShader | Scene::AddEntityOptions::PassCameraPostitionToShader);
            }
            else
            {
                s.add_entity(Scene::SceneRenderableEntity{
                    .shader = normal_shader,
                    .mesh = suzi_mesh,
                    .transform = Transform::default_transform()
                        .translate({pos_x, 2.f, pos_z})
                        .scale({0.3f, 0.3f, 0.3f})
                        .rotate({0.f, -rot * i - 90, 0.f}),
                    .material = PhongBlinnMaterial::default_material(),
                });
            }
            
        }


        return s;
    }
}