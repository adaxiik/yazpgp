#pragma once
#include <memory>
#include "renderable_entity.hpp"
#include "camera.hpp"
#include "input_manager.hpp"

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
        Scene() = default;
        Scene(std::vector<std::unique_ptr<RenderableEntity>> entities);
        Scene(const std::vector<SceneRenderableEntity>& entities);

        void add_entity(std::unique_ptr<RenderableEntity> entity);
        void add_entity(const SceneRenderableEntity& entity);

        auto begin() { return m_entities.begin(); }
        auto end() { return m_entities.end(); }

        void render(const glm::mat4& projection_matrix) const;
        void update(const InputManager& input_manager, double delta_time);

    private:
        std::vector<std::unique_ptr<RenderableEntity>> m_entities;
        Camera m_camera;
    };
} 