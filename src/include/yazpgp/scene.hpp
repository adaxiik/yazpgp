#pragma once
#include "renderable_entity.hpp"
#include <memory>

namespace yazpgp
{
    class Scene
    {
        struct SceneRenderableEntity
        {
            std::shared_ptr<Shader> shader;
            std::shared_ptr<Mesh> mesh;
        };
    public:
        Scene() = default;
        Scene(std::vector<std::unique_ptr<RenderableEntity>> entities);
        Scene(const std::vector<SceneRenderableEntity>& entities);
        void render(const glm::mat4& view_projection_matrix) const;
        void render_scene_imgui_panel() const;
        void add_entity(std::unique_ptr<RenderableEntity> entity);
        void add_entity(const SceneRenderableEntity& entity);
    private:
        std::vector<std::unique_ptr<RenderableEntity>> m_entities;
    };
} 