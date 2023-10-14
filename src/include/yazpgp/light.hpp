#pragma once
#include <glm/glm.hpp>
namespace yazpgp
{
    struct PointLight
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 color = glm::vec3(1.0f);
        float ambient_intensity = 0.1f;
        float diffuse_intensity = 1.0f;
        float specular_intensity = 1.0f;
        
        constexpr static auto MAX_LIGHTS = 4;
    };
    static_assert(sizeof(PointLight) == 3*4 + 3*4 + 4 + 4 + 4);
}