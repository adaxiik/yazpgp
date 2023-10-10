#include "scene.hpp"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
namespace yazpgp
{
    Scene::Scene(std::vector<std::unique_ptr<RenderableEntity>> entities)
        : m_entities(std::move(entities))
    {
    }

    Scene::Scene(const std::vector<SceneRenderableEntity>& entities)
    {
        for (const auto& entity : entities)
            m_entities.push_back(std::make_unique<RenderableEntity>(entity.shader, entity.mesh, entity.textures, entity.transform));
    }

    void Scene::render(const glm::mat4& projection_matrix) const
    {
        auto view_projection_matrix = projection_matrix * m_camera.view_matrix();

        for (const auto& entity : m_entities)
            entity->render(view_projection_matrix);
    }

    void Scene::update(const InputManager& input_manager, double delta_time)
    {
        m_camera.update(input_manager, delta_time);
    }

    void Scene::add_entity(std::unique_ptr<RenderableEntity> entity)
    {
        m_entities.push_back(std::move(entity));
    }

    void Scene::add_entity(const SceneRenderableEntity& entity)
    {
        m_entities.push_back(std::make_unique<RenderableEntity>(entity.shader, entity.mesh));
    }
}