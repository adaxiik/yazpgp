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
            m_entities.push_back(std::make_unique<RenderableEntity>(entity.shader, entity.mesh));
    }

    void Scene::render(const glm::mat4& view_projection_matrix) const
    {
        for (const auto& entity : m_entities)
            entity->render(view_projection_matrix);
    }

    void Scene::add_entity(std::unique_ptr<RenderableEntity> entity)
    {
        m_entities.push_back(std::move(entity));
    }

    void Scene::add_entity(const SceneRenderableEntity& entity)
    {
        m_entities.push_back(std::make_unique<RenderableEntity>(entity.shader, entity.mesh));
    }

    void Scene::render_scene_imgui_panel() const
    {
        ImGui::Begin("Scene");
        for (size_t i = 0; i < m_entities.size(); i++)
        {
            ImGui::PushID(i);
            std::string name = "Entity_" + std::to_string(i);
            if (ImGui::CollapsingHeader(name.c_str()))
            {
                auto& transform = m_entities[i]->transform();
                ImGui::SliderFloat3("Position", glm::value_ptr(transform.position), -10.0f, 10.0f);
                ImGui::SliderFloat3("Rotation", glm::value_ptr(transform.rotation), -180.0f, 180.0f);
                ImGui::SliderFloat3("Scale", glm::value_ptr(transform.scale), 0.0f, 10.0f);
            }
            ImGui::PopID();
        }
        ImGui::End();   
    }
}