#pragma once
#include <glm/glm.hpp>
#include "event_producer.hpp"
namespace yazpgp
{
    struct SpotLight : public EventProducer<SpotLight>
    {
        constexpr static auto MAX_SPOT_LIGHTS = 4;
        
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f);
        glm::vec3 color = glm::vec3(1.0f);
        float ambient_intensity = 0.0f;
        float diffuse_intensity = 1.0f;
        float specular_intensity = 1.0f;
        float illumination_radius = 100.0f;
        float inner_cone_angle_degrees = 10.0f;
        float outer_cone_angle_degrees = 25.0f;

        SpotLight() = default;

        SpotLight& set_position(const glm::vec3& position)
        {
            this->position = position;
            return *this;
        }

        SpotLight& set_direction(const glm::vec3& direction)
        {
            this->direction = direction;
            return *this;
        }

        SpotLight& set_color(const glm::vec3& color)
        {
            this->color = color;
            return *this;
        }

        SpotLight& set_ambient_intensity(float ambient_intensity)
        {
            this->ambient_intensity = ambient_intensity;
            return *this;
        }

        SpotLight& set_diffuse_intensity(float diffuse_intensity)
        {
            this->diffuse_intensity = diffuse_intensity;
            return *this;
        }

        SpotLight& set_specular_intensity(float specular_intensity)
        {
            this->specular_intensity = specular_intensity;
            return *this;
        }

        SpotLight& set_illumination_radius(float illumination_radius)
        {
            this->illumination_radius = illumination_radius;
            return *this;
        }

        SpotLight& set_inner_cone_angle_degrees(float inner_cone_angle_degrees)
        {
            this->inner_cone_angle_degrees = inner_cone_angle_degrees;
            return *this;
        }

        SpotLight& set_outer_cone_angle_degrees(float outer_cone_angle_degrees)
        {
            this->outer_cone_angle_degrees = outer_cone_angle_degrees;
            return *this;
        }

        SpotLight& invoke()
        {
            notify(*this);
            return *this;
        }
    };
}