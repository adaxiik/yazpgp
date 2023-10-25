#pragma once
// #include <glm/glm.hpp>
#include "shader.hpp"

namespace yazpgp
{
    struct Material
    {
        enum class Kind
        {
            PhongBlinn,
            
            COUNT
        };
        virtual void use(const Shader& shader) = 0;
        virtual ~Material() = default;
        virtual Material::Kind kind() const = 0;
    };
}