#include "renderable_entity.hpp"
#include "scene.hpp"

namespace yazpgp
{
    RenderableEntity::RenderableEntity(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<Mesh>& mesh,
        const std::vector<std::shared_ptr<Texture>>& textures,
        const Transform& transform,
        const std::shared_ptr<Material>& material,
        TransformModifier transform_modifier
    )
        : m_transform(transform)
        , m_textures(textures)
        , m_shader(shader)
        , m_mesh(mesh)
        , m_material(material)
        , m_transform_modifier(transform_modifier)
    {
    }

    Transform& RenderableEntity::transform()
    {
        return m_transform;
    }

    void RenderableEntity::render(const glm::mat4& view_projection_matrix) const
    {
        
        auto modified_model_matrix = m_transform_modifier(m_transform.model_matrix());

        m_shader->use();
        m_shader->set_uniform("model_matrix", modified_model_matrix);
        m_shader->set_uniform("mvp_matrix", view_projection_matrix * modified_model_matrix);
        m_shader->set_uniform("normal_matrix", glm::mat3(glm::transpose(glm::inverse(modified_model_matrix))));
        
        if (m_material)
            m_material->use(*m_shader);

        for (size_t i = 0; i < m_textures.size(); i++)
            m_textures[i]->use(i);

        m_mesh->use();
        // glDrawArrays(GL_TRIANGLES, 0, m_mesh->get_vert_count());
        glDrawElements(GL_TRIANGLES, m_mesh->get_index_count(), GL_UNSIGNED_INT, 0);
    }

    void RenderableEntity::update(const Scene& scene, double delta_time)
    {
        // TODOO
        (void) scene;
        (void) delta_time;
    }
}