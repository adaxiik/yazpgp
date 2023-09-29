#pragma once
#include "shader.hpp"
#include "mesh.hpp"

#include <memory>

#include "transform.hpp"

namespace yazpgp
{
    class RenderableEntity
    {
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Mesh> m_mesh;
        Transform m_transform;
    public:
        RenderableEntity(
            const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<Mesh>& mesh,
            const Transform& transform = Transform()
            );
        void render(const glm::mat4& view_projection_matrix) const;
        Transform& transform();
    };
}