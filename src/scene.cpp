#include "scene.hpp"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "logger.hpp"
namespace yazpgp
{
    Scene::Scene()
    {
        m_camera.move_forward(-10.0f);
    }

    Scene::Scene(std::vector<std::unique_ptr<RenderableEntity>> entities): Scene()
    {
        m_entities = std::move(entities);
    }

    Scene::Scene(const std::vector<SceneRenderableEntity>& entities): Scene()
    {
        for (const auto& entity : entities)
        {   
            m_entities.push_back(std::make_unique<RenderableEntity>(
                entity.shader,
                entity.mesh,
                entity.textures,
                entity.transform,
                entity.transform_modifier
            ));
        }
    }

    void Scene::render(const glm::mat4& projection_matrix) const
    {
        auto view_projection_matrix = projection_matrix * m_camera.view_matrix();

        if (m_skybox)
            m_skybox->render(projection_matrix, m_camera.view_matrix());

        for (const auto& entity : m_entities)
            entity->render(view_projection_matrix, m_lights, m_camera.position());
        
        
    }

    void Scene::update(const InputManager& input_manager, double delta_time)
    {
        m_camera.update(input_manager, delta_time);
        for (const auto& entity : m_entities)
            entity->update(*this, delta_time);
    }

    Scene& Scene::add_entity(std::unique_ptr<RenderableEntity> entity)
    {
        m_entities.push_back(std::move(entity));
        return *this;
    }

    Scene& Scene::add_entity(const SceneRenderableEntity& entity)
    {
        m_entities.push_back(std::make_unique<RenderableEntity>(
            entity.shader,
            entity.mesh,
            entity.textures,
            entity.transform,
            entity.transform_modifier
        ));
        return *this;
    }

    Scene& Scene::add_light(const PointLight& light)
    {
        if (m_lights.size() >= PointLight::MAX_LIGHTS)
        {
            YAZPGP_LOG_WARN("Maximum number of lights reached, ignoring light");
            return *this;
        }

        m_lights.push_back(light);
        return *this;
    }

    Scene& Scene::set_skybox(std::shared_ptr<Skybox> skybox)
    {
        m_skybox = skybox;
        return *this;
    }
}