#pragma once
#include <string>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace yazpgp
{
    class Shader 
    {
        using ShaderProgramId = GLuint;
        
        ShaderProgramId m_program;
    public:
        Shader(ShaderProgramId linked_program) : m_program(linked_program){}
        ~Shader();
        static std::shared_ptr<Shader> create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
        static std::shared_ptr<Shader> create_default_shader(float r = 1.f, float g = 0.f, float b = 0.f, float a = 1.f);
        void use() const;

        void set_uniform(const std::string& name, const glm::mat4& value) const;
        void set_uniform(const std::string& name, const glm::mat3& value) const;
        void set_uniform(const std::string& name, const glm::vec3& value) const;
        void set_uniform(const std::string& name, const glm::vec4& value) const;
        void set_uniform(const std::string& name, const float value) const;
        void set_uniform(const std::string& name, const int value) const;

        static void unuse();

    };
}
