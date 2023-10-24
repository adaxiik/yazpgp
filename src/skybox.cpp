#include "skybox.hpp"
namespace yazpgp
{
    Skybox::Skybox(std::shared_ptr<CubeMap> cubemap, std::shared_ptr<Shader> shader)
        : m_cubemap(cubemap)
        , m_shader(shader)
        , m_cube_mesh(Mesh::create_cube())
    {
    }
    

    Skybox::~Skybox()
    {
    }

    void Skybox::render(const glm::mat4& projection_matrix, const glm::mat4& view_matrix) const
    {
        auto view_only_rotation = glm::mat4(glm::mat3(view_matrix));
        glDepthMask(GL_FALSE);
        m_shader->use();
        m_shader->set_uniform("view_projection_matrix", projection_matrix * view_only_rotation);
        m_cubemap->use(0);
        m_cube_mesh->use();
        glDrawArrays(GL_TRIANGLES, 0, m_cube_mesh->get_vert_count());
        glDepthMask(GL_TRUE);
    }

   
          
}