#pragma once
#include <memory>
#include <vector>
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "transform.hpp"
#include "light.hpp"

namespace yazpgp
{
    class RenderableEntity
    {
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Mesh> m_mesh;
        std::vector<std::shared_ptr<Texture>> m_textures;
        Transform m_transform;
    public:
        RenderableEntity(
            const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<Mesh>& mesh,
            const std::vector<std::shared_ptr<Texture>>& textures = {},
            const Transform& transform = Transform::default_transform()
        );
        void render(
            const glm::mat4& view_projection_matrix,
            const std::vector<PointLight>& lights,
            const glm::vec3& camera_position) const;
        Transform& transform();
    };
}