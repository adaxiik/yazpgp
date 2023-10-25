#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "transform.hpp"
#include "light.hpp"

namespace yazpgp
{
    class Scene;
    class RenderableEntity
    {
        Transform m_transform;
        std::vector<std::shared_ptr<Texture>> m_textures;
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Mesh> m_mesh;
        std::function<glm::mat4(const glm::mat4&)> m_transform_modifier;
    public:
        using TransformModifier = std::function<glm::mat4(const glm::mat4&)>;
        RenderableEntity(
            const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<Mesh>& mesh,
            const std::vector<std::shared_ptr<Texture>>& textures = {},
            const Transform& transform = Transform::default_transform(),
            TransformModifier transform_modifier = [](const glm::mat4& m) { return m; }
        );

        void render(
            const glm::mat4& view_projection_matrix,
            const std::vector<PointLight>& lights,
            const glm::vec3& camera_position
        ) const;

        void update(const Scene& scene, double delta_time);
        
        Transform& transform();
    };
}