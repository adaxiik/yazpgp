#pragma once
#include <memory>
#include "renderable_entity.hpp"
#include "camera.hpp"
#include "input_manager.hpp"
#include "light.hpp"

namespace yazpgp
{
    class Scene
    {
    public:
        struct SceneRenderableEntity
        {
            std::shared_ptr<Shader> shader;
            std::shared_ptr<Mesh> mesh;
            std::vector<std::shared_ptr<Texture>> textures = {};
            Transform transform = Transform::default_transform();
        };
        Scene();
        Scene(std::vector<std::unique_ptr<RenderableEntity>> entities);
        Scene(const std::vector<SceneRenderableEntity>& entities);

        Scene& add_entity(std::unique_ptr<RenderableEntity> entity);
        Scene& add_entity(const SceneRenderableEntity& entity);
        Scene& add_light(const PointLight& light);

        auto begin() { return m_entities.begin(); }
        auto end() { return m_entities.end(); }

        void render(const glm::mat4& projection_matrix) const;
        void update(const InputManager& input_manager, double delta_time);

    private:
        Camera m_camera;
        std::vector<std::unique_ptr<RenderableEntity>> m_entities;
        std::vector<PointLight> m_lights;  
    };
} 