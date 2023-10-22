#include "renderable_entity.hpp"
#include "scene.hpp"

namespace yazpgp
{
    RenderableEntity::RenderableEntity(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<Mesh>& mesh,
        const std::vector<std::shared_ptr<Texture>>& textures,
        const Transform& transform,
        std::function<void(const Scene&, double)> on_update
    )
        : m_transform(transform)
        , m_textures(textures)
        , m_shader(shader)
        , m_mesh(mesh)
        , m_on_update(on_update)
    {
    }

    Transform& RenderableEntity::transform()
    {
        return m_transform;
    }

    void RenderableEntity::render(
        const glm::mat4& view_projection_matrix,
        const std::vector<PointLight>& lights,
        const glm::vec3& camera_position
    ) const
    {
        m_shader->use();
        m_shader->set_uniform("model_matrix", m_transform.model_matrix());
        m_shader->set_uniform("mvp_matrix", view_projection_matrix * m_transform.model_matrix());
        m_shader->set_uniform("camera_position", camera_position);
        m_shader->set_uniform("num_point_lights", static_cast<int>(lights.size()));
        for (size_t i = 0; i < lights.size(); i++)
        {
            m_shader->set_uniform("point_lights[" + std::to_string(i) + "].position", lights[i].position);
            m_shader->set_uniform("point_lights[" + std::to_string(i) + "].color", lights[i].color);
            m_shader->set_uniform("point_lights[" + std::to_string(i) + "].ambient_intensity", lights[i].ambient_intensity);
            m_shader->set_uniform("point_lights[" + std::to_string(i) + "].diffuse_intensity", lights[i].diffuse_intensity);
            m_shader->set_uniform("point_lights[" + std::to_string(i) + "].specular_intensity", lights[i].specular_intensity);
        }
        m_shader->set_uniform("normal_matrix", glm::mat3(glm::transpose(glm::inverse(m_transform.model_matrix()))));

        for (size_t i = 0; i < m_textures.size(); i++)
            m_textures[i]->use(i);

        m_mesh->use();
        // glDrawArrays(GL_TRIANGLES, 0, m_mesh->get_vert_count());
        glDrawElements(GL_TRIANGLES, m_mesh->get_index_count(), GL_UNSIGNED_INT, 0);
    }

    void RenderableEntity::update(const Scene& scene, double delta_time)
    {
        if (m_on_update)
            m_on_update(scene, delta_time);
    }
}