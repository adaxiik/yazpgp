#pragma once

#include <memory>
#include "material.hpp"
#include "debug/debug_ui_def.hpp"
namespace yazpgp
{
    class PhongBlinnMaterial : public Material
    {
        ENABLE_DEBUG_UI();
    public:
        PhongBlinnMaterial(
            const glm::vec3& ambient_color = {1.0f, 1.0f, 1.0f},
            const glm::vec3& diffuse_color = {1.0f, 1.0f, 1.0f},
            const glm::vec3& specular_color = {1.0f, 1.0f, 1.0f},
            float specular_shininess = 32.0f
        );
        
        virtual void use(const Shader& shader) override;
        virtual ~PhongBlinnMaterial() = default;
        virtual Material::Kind kind() const override;

        static std::shared_ptr<PhongBlinnMaterial> default_material();
        static std::shared_ptr<PhongBlinnMaterial> create_shared(
            const glm::vec3& ambient_color,
            const glm::vec3& diffuse_color,
            const glm::vec3& specular_color,
            float specular_shininess = 32.0f
        );

        static std::shared_ptr<PhongBlinnMaterial> create_shared(
            const glm::vec3& color,
            float specular_shininess = 32.0f
        );

    private:
        glm::vec3 m_ambient_color;
        glm::vec3 m_diffuse_color;
        glm::vec3 m_specular_color;
        float m_specular_shininess;
    };
}