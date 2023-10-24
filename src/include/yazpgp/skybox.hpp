#pragma once
#include "cubemap.hpp"
#include "mesh.hpp"
#include "shader.hpp"

#include <memory>
#include <glm/glm.hpp>
namespace yazpgp
{
    class Skybox
    {
        std::shared_ptr<CubeMap> m_cubemap;
        std::shared_ptr<Shader> m_shader;       
        std::unique_ptr<Mesh> m_cube_mesh;

    public:
        Skybox(std::shared_ptr<CubeMap> cubemap, std::shared_ptr<Shader> shader);
        ~Skybox();
        void render(const glm::mat4& projection_matrix, const glm::mat4& view_matrix) const;   
    };
}