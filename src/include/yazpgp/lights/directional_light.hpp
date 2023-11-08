#pragma once
#include <glm/glm.hpp>
#include "event_producer.hpp"
namespace yazpgp
{
    struct DirectionalLight : public EventProducer<DirectionalLight>
    {
        constexpr static auto MAX_DIRECTIONAL_LIGHTS = 4;

        glm::vec3 direction = glm::vec3(0.0f);
        glm::vec3 color = glm::vec3(1.0f);
        float ambient_intensity = 0.1f;
        float diffuse_intensity = 1.0f;
        float specular_intensity = 1.0f;

        // DirectionalLight() = default;

        DirectionalLight& set_direction(const glm::vec3& direction)
        {
            this->direction = direction;
            return *this;
        }

        DirectionalLight& set_color(const glm::vec3& color)
        {
            this->color = color;
            return *this;
        }

        DirectionalLight& set_ambient_intensity(float ambient_intensity)
        {
            this->ambient_intensity = ambient_intensity;
            return *this;
        }

        DirectionalLight& set_diffuse_intensity(float diffuse_intensity)
        {
            this->diffuse_intensity = diffuse_intensity;
            return *this;
        }

        DirectionalLight& set_specular_intensity(float specular_intensity)
        {
            this->specular_intensity = specular_intensity;
            return *this;
        }
    };
}