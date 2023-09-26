#include "renderable_entity.hpp"

namespace yazpgp
{
    RenderableEntity::RenderableEntity(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh)
        : m_shader(shader)
        , m_mesh(mesh)
    {
    }

    void RenderableEntity::render() const
    {
        m_shader->use();
        m_mesh->use();
        // glDrawArrays(GL_TRIANGLES, 0, m_mesh->get_vert_count());
        glDrawElements(GL_TRIANGLES, m_mesh->get_index_count(), GL_UNSIGNED_INT, 0);
    }
}