#pragma once
#include "shader.hpp"
#include "mesh.hpp"

#include <memory>

// #include <glm...         // TODOO

namespace yazpgp
{
    class RenderableEntity
    {
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Mesh> m_mesh;

    public:
        RenderableEntity(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh);
        void render() const;
    };
}