#pragma once
#include <glm/glm.hpp>
#include "event_producer.hpp"
namespace yazpgp
{
    struct PointLight : public EventProducer<PointLight>
    {
        constexpr static auto MAX_LIGHTS = 4;

        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 color = glm::vec3(1.0f);
        float ambient_intensity = 0.1f;
        float diffuse_intensity = 1.0f;
        float specular_intensity = 1.0f;
        float illumination_radius = 100.0f;

        PointLight() = default;
        
        PointLight& set_position(const glm::vec3& position)
        {
            this->position = position;
            return *this;
        }

        PointLight& set_color(const glm::vec3& color)
        {
            this->color = color;
            return *this;
        }

        PointLight& set_ambient_intensity(float ambient_intensity)
        {
            this->ambient_intensity = ambient_intensity;
            return *this;
        }

        PointLight& set_diffuse_intensity(float diffuse_intensity)
        {
            this->diffuse_intensity = diffuse_intensity;
            return *this;
        }

        PointLight& set_specular_intensity(float specular_intensity)
        {
            this->specular_intensity = specular_intensity;
            return *this;
        }

        PointLight& set_illumination_radius(float illumination_radius)
        {
            this->illumination_radius = illumination_radius;
            return *this;
        }
        
    };
}