#pragma once
#include <memory>
#include "renderable_entity.hpp"
#include "camera.hpp"
#include "input_manager.hpp"
#include "lights/light.hpp"
#include "skybox.hpp"
#include "material.hpp"
#include "debug/debug_ui_def.hpp"
#include "event_distributor.hpp"

namespace yazpgp
{
    class Scene
    {
    public:
        ENABLE_DEBUG_UI();
        struct SceneRenderableEntity
        {
            std::shared_ptr<Shader> shader;
            std::shared_ptr<Mesh> mesh;
            std::vector<std::shared_ptr<Texture>> textures = {};
            Transform transform = Transform::default_transform();
            std::shared_ptr<Material> material = nullptr;
            RenderableEntity::TransformModifier transform_modifier = [](const glm::mat4& m) { return m; };
        };

        enum AddEntityOptions
        {
            None = 1 << 0,
            PassCameraPostitionToShader = 1 << 1, 
            PassLightToShader = 1 << 2
        };

        

        Scene();
        // Scene(std::vector<std::unique_ptr<RenderableEntity>> entities);
        // Scene(const std::vector<SceneRenderableEntity>& entities);
        ~Scene() = default;
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = default;
        Scene& operator=(Scene&&) = default;

        Scene& add_entity(std::unique_ptr<RenderableEntity> entity);
        Scene& add_entity(const SceneRenderableEntity& entity, AddEntityOptions options = AddEntityOptions::None);
        Scene& add_light(const PointLight& light);
        Scene& add_light(const SpotLight& light);
        Scene& add_light(const DirectionalLight& light);
        Scene& set_skybox(std::shared_ptr<Skybox> skybox);

        auto begin() { return m_entities.begin(); }
        auto end() { return m_entities.end(); }

        void render(const glm::mat4& projection_matrix) const;
        void update(const InputManager& input_manager, double delta_time);

        Scene& invoke_distributors();

        Camera& camera();
    private:
        Camera m_camera;
        std::vector<std::unique_ptr<RenderableEntity>> m_entities;
        std::unique_ptr<std::vector<PointLight>> m_point_lights;
        std::unique_ptr<std::vector<SpotLight>> m_spot_lights;
        std::unique_ptr<std::vector<DirectionalLight>> m_directional_lights;
        std::unique_ptr<EventDistributor<Camera>> m_camera_event_distributor;
        std::unique_ptr<EventDistributor<PointLight>> m_point_light_event_distributor;
        std::unique_ptr<EventDistributor<SpotLight>> m_spot_light_event_distributor;
        std::unique_ptr<EventDistributor<DirectionalLight>> m_directional_light_event_distributor;
        std::shared_ptr<Skybox> m_skybox;

    };

    constexpr enum Scene::AddEntityOptions operator|(Scene::AddEntityOptions lhs, Scene::AddEntityOptions rhs)
    {     
        return static_cast<Scene::AddEntityOptions>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }
} 