#include "renderable_entity.hpp"

namespace yazpgp
{
    RenderableEntity::RenderableEntity(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<Mesh>& mesh,
        const Transform& transform)
        : m_shader(shader)
        , m_mesh(mesh)
        , m_transform(transform)
    {
    }

    Transform& RenderableEntity::transform()
    {
        return m_transform;
    }

    void RenderableEntity::render(const glm::mat4& view_projection_matrix) const
    {
        m_shader->set_uniform("mvp", view_projection_matrix * m_transform.get_model_matrix());
        m_shader->use();
        m_mesh->use();
        // glDrawArrays(GL_TRIANGLES, 0, m_mesh->get_vert_count());
        glDrawElements(GL_TRIANGLES, m_mesh->get_index_count(), GL_UNSIGNED_INT, 0);
    }
}