#include "scene.hpp"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "logger.hpp"
#include <iostream>

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
 


namespace yazpgp
{
    Scene::Scene() 
    : m_point_lights(std::make_unique<std::vector<PointLight>>())
    , m_spot_lights(std::make_unique<std::vector<SpotLight>>())
    , m_directional_lights(std::make_unique<std::vector<DirectionalLight>>())
    , m_camera_event_distributor(std::make_unique<EventDistributor<Camera>>())
    , m_point_light_event_distributor(std::make_unique<EventDistributor<PointLight>>())
    , m_spot_light_event_distributor(std::make_unique<EventDistributor<SpotLight>>())
    , m_directional_light_event_distributor(std::make_unique<EventDistributor<DirectionalLight>>())
    , m_light_count_event_distributor(std::make_unique<EventDistributor<LightCountData>>())
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
            m_point_light_event_distributor->subscribe([shader = entity.shader.get(), lights = m_point_lights.get()](const PointLight& light)
            {
                for (size_t i = 0; i < lights->size(); i++)
                    LightUseVisitor{ *shader, i }(light);
                
                shader->set_uniform("light.num_point_lights", static_cast<int>(lights->size()));
            });

            m_spot_light_event_distributor->subscribe([shader = entity.shader.get(), lights = m_spot_lights.get()](const SpotLight& light)
            {
                for (size_t i = 0; i < lights->size(); i++)
                    LightUseVisitor{ *shader, i }(light);
                
                shader->set_uniform("light.num_spot_lights", static_cast<int>(lights->size()));
            });

            m_directional_light_event_distributor->subscribe([shader = entity.shader.get(), lights = m_directional_lights.get()](const DirectionalLight& light)
            {
                for (size_t i = 0; i < lights->size(); i++)
                    LightUseVisitor{ *shader, i }(light);
                
                shader->set_uniform("light.num_directional_lights", static_cast<int>(lights->size()));
            });

            m_light_count_event_distributor->subscribe([shader = entity.shader.get()](const LightCountData& light_count_data)
            {
                shader->set_uniform("light.num_point_lights", static_cast<int>(light_count_data.point_light_count));
                shader->set_uniform("light.num_spot_lights", static_cast<int>(light_count_data.spot_light_count));
                shader->set_uniform("light.num_directional_lights", static_cast<int>(light_count_data.directional_light_count));
            });

            entity.shader->set_uniform("light.num_point_lights", static_cast<int>(m_point_lights->size()));
            entity.shader->set_uniform("light.num_spot_lights", static_cast<int>(m_spot_lights->size()));
            entity.shader->set_uniform("light.num_directional_lights", static_cast<int>(m_directional_lights->size()));
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
        if (m_point_lights->size() >= PointLight::MAX_POINT_LIGHTS)
        {
            YAZPGP_LOG_WARN("Maximum number of lights reached, ignoring light");
            return *this;
        }

        auto copy = light;
        copy.set_notify_callback([event_distributor = m_point_light_event_distributor.get()](const PointLight& light)
        {
            event_distributor->notify(light);
        });

        m_point_lights->push_back(copy);
        m_point_light_event_distributor->notify(copy);

        return *this;
    }

    Scene& Scene::add_light(const SpotLight& light)
    {
        if (m_spot_lights->size() >= SpotLight::MAX_SPOT_LIGHTS)
        {
            YAZPGP_LOG_WARN("Maximum number of lights reached, ignoring light");
            return *this;
        }

        auto copy = light;

        copy.set_notify_callback([event_distributor = m_spot_light_event_distributor.get()](const SpotLight& light)
        {
            event_distributor->notify(light);
        });

        m_spot_lights->push_back(copy);
        m_spot_light_event_distributor->notify(copy);

        return *this;
    }

    Scene& Scene::add_light(const DirectionalLight& light)
    {
        if (m_directional_lights->size() >= DirectionalLight::MAX_DIRECTIONAL_LIGHTS)
        {
            YAZPGP_LOG_WARN("Maximum number of lights reached, ignoring light");
            return *this;
        }

        auto copy = light;

        copy.set_notify_callback([event_distributor = m_directional_light_event_distributor.get()](const DirectionalLight& light)
        {
            event_distributor->notify(light);
        });

        m_directional_lights->push_back(copy);
        m_directional_light_event_distributor->notify(copy);

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
        for (const auto& light : *m_point_lights)
            m_point_light_event_distributor->notify(light);

        for (const auto& light : *m_spot_lights)
            m_spot_light_event_distributor->notify(light);
        
        for (const auto& light : *m_directional_lights)
            m_directional_light_event_distributor->notify(light);

        m_light_count_event_distributor->notify(
            LightCountData{ 
                m_point_lights->size(),
                m_spot_lights->size(),
                m_directional_lights->size()
            }
        );
            
        return *this;
    }
}