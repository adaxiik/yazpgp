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

        void set_uniform(const std::string& name, glm::mat4 value) const;

    };
}
