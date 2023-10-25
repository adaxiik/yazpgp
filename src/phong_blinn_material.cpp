#include "phong_blinn_material.hpp"

namespace yazpgp
{
    PhongBlinnMaterial::PhongBlinnMaterial(
        const glm::vec3& ambient_color,
        const glm::vec3& diffuse_color,
        const glm::vec3& specular_color,
        float specular_shininess
    ) 
    : m_ambient_color(ambient_color)
    , m_diffuse_color(diffuse_color)
    , m_specular_color(specular_color)
    , m_specular_shininess(specular_shininess) {}

    void PhongBlinnMaterial::use(const Shader& shader)
    {
        shader.set_uniform("material.ambient_color", m_ambient_color);
        shader.set_uniform("material.diffuse_color", m_diffuse_color);
        shader.set_uniform("material.specular_color", m_specular_color);
        shader.set_uniform("material.specular_shininess", m_specular_shininess);
    }

    std::shared_ptr<PhongBlinnMaterial> PhongBlinnMaterial::default_material()
    {
        return std::make_shared<PhongBlinnMaterial>();
    }

    std::shared_ptr<PhongBlinnMaterial> PhongBlinnMaterial::create_shared(
        const glm::vec3& ambient_color,
        const glm::vec3& diffuse_color,
        const glm::vec3& specular_color,
        float specular_shininess
    )
    {
        return std::make_shared<PhongBlinnMaterial>(ambient_color, diffuse_color, specular_color, specular_shininess);
    }

    std::shared_ptr<PhongBlinnMaterial> PhongBlinnMaterial::create_shared(
        const glm::vec3& color,
        float specular_shininess
    )
    {
        return std::make_shared<PhongBlinnMaterial>(color, color, color, specular_shininess);
    }

    Material::Kind PhongBlinnMaterial::kind() const
    {
        return Material::Kind::PhongBlinn;
    }
}