#include "scene.hpp"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "logger.hpp"
#include <iostream>
namespace yazpgp
{
    Scene::Scene() 
    : m_lights(std::make_unique<std::vector<PointLight>>())
    , m_camera_event_distributor(std::make_unique<EventDistributor<Camera>>())
    , m_light_event_distributor(std::make_unique<EventDistributor<PointLight>>())
    {
        m_camera.set_notify_callback([event_distributor = m_camera_event_distributor.get()](const Camera& camera)
        {
            event_distributor->notify(camera);
        });

        m_camera.move_forward(-10.0f);
    }

    void Scene::render(const glm::mat4& projection_matrix) const
    {
        auto view_projection_matrix = projection_matrix * m_camera.view_matrix();

        if (m_skybox)
            m_skybox->render(projection_matrix, m_camera.view_matrix());

        for (const auto& entity : m_entities)
            entity->render(view_projection_matrix);
        
        
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

    Scene& Scene::add_entity(const SceneRenderableEntity& entity, AddEntityOptions options)
    {
        if (options & AddEntityOptions::PassCameraPostitionToShader)
        {
            m_camera_event_distributor->subscribe([shader = entity.shader.get()](const Camera& camera)
            {
                shader->set_uniform("camera_position", camera.position());
            });
        }

        if (options & AddEntityOptions::PassLightToShader)
        {
            m_light_event_distributor->subscribe([shader = entity.shader.get(), lights = m_lights.get()](const PointLight& )
            {
                for (size_t i = 0; i < lights->size(); i++)
                {
                    shader->set_uniform("point_lights[" + std::to_string(i) + "].position", (*lights)[i].position);
                    shader->set_uniform("point_lights[" + std::to_string(i) + "].color", (*lights)[i].color);
                    shader->set_uniform("point_lights[" + std::to_string(i) + "].ambient_intensity", (*lights)[i].ambient_intensity);
                    shader->set_uniform("point_lights[" + std::to_string(i) + "].diffuse_intensity", (*lights)[i].diffuse_intensity);
                    shader->set_uniform("point_lights[" + std::to_string(i) + "].specular_intensity", (*lights)[i].specular_intensity);
                    shader->set_uniform("point_lights[" + std::to_string(i) + "].illumination_radius", (*lights)[i].illumination_radius);
                }
            });
        }

        m_entities.push_back(std::make_unique<RenderableEntity>(
            entity.shader,
            entity.mesh,
            entity.textures,
            entity.transform,
            entity.material,
            entity.transform_modifier
        ));
        return *this;
    }

    Scene& Scene::add_light(const PointLight& light)
    {
        if (m_lights->size() >= PointLight::MAX_LIGHTS)
        {
            YAZPGP_LOG_WARN("Maximum number of lights reached, ignoring light");
            return *this;
        }

        auto copy = light;
        copy.set_notify_callback([event_distributor = m_light_event_distributor.get()](const PointLight& light)
        {
            event_distributor->notify(light);
        });

        m_lights->push_back(copy);
        m_light_event_distributor->notify(copy);

        return *this;
    }

    Scene& Scene::set_skybox(std::shared_ptr<Skybox> skybox)
    {
        m_skybox = skybox;
        return *this;
    }

    Camera& Scene::camera()
    {
        return m_camera;
    }

    Scene& Scene::invoke_distributors()
    {
        m_camera_event_distributor->notify(m_camera);
        for (const auto& light : *m_lights)
            m_light_event_distributor->notify(light);
        return *this;
    }
}