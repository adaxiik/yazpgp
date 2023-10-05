#pragma once
#include "renderable_entity.hpp"
#include <memory>

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
        void render(const glm::mat4& view_projection_matrix) const;
        void render_scene_imgui_panel() const;
        void add_entity(std::unique_ptr<RenderableEntity> entity);
        void add_entity(const SceneRenderableEntity& entity);

        auto begin() { return m_entities.begin(); }
        auto end() { return m_entities.end(); }
    private:
        std::vector<std::unique_ptr<RenderableEntity>> m_entities;
    };
} 