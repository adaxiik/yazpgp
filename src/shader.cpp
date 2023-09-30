#include "shader.hpp"
#include "logger.hpp"
#include <memory>

#include <glm/gtc/type_ptr.hpp>

namespace yazpgp
{
    std::shared_ptr<Shader> Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
    { 

        if (vertex_shader.empty())
        {
            YAZPGP_LOG_ERROR("Vertex shader source is empty");
            return nullptr;
        }

        if (fragment_shader.empty())
        {
            YAZPGP_LOG_ERROR("Fragment shader source is empty");
            return nullptr;
        }

        const GLchar* very_unsafe_and_scary_vertex_source {&vertex_shader[0]};
        const GLchar* very_unsafe_and_scary_fragment_source {&fragment_shader[0]};

        GLuint compiled_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(compiled_vertex_shader, 1, &very_unsafe_and_scary_vertex_source, NULL);
        glCompileShader(compiled_vertex_shader);

        GLuint compiled_shader_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(compiled_shader_shader, 1, &very_unsafe_and_scary_fragment_source, NULL);
        glCompileShader(compiled_shader_shader);

        GLuint linked_shader_program = glCreateProgram();
        glAttachShader(linked_shader_program, compiled_shader_shader);
        glAttachShader(linked_shader_program, compiled_vertex_shader);
        glLinkProgram(linked_shader_program);

        GLint status;
        glGetProgramiv(linked_shader_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint info_log_length;
            glGetProgramiv(linked_shader_program, GL_INFO_LOG_LENGTH, &info_log_length);
            std::unique_ptr<GLchar> std_info_log = std::make_unique<GLchar>(info_log_length + 1);
            glGetProgramInfoLog(linked_shader_program, info_log_length, NULL, std_info_log.get());
            YAZPGP_LOG_ERROR("Linker failure: %s\n", std_info_log.get());
            return nullptr;
        }

        YAZPGP_LOG_DEBUG("Shader loaded with id: %d", linked_shader_program);

        return std::make_shared<Shader>(linked_shader_program);
    }

    std::shared_ptr<Shader> Shader::create_default_shader(float r, float g, float b, float a)
    {
        const std::string default_vertex_shader =
            "#version 330\n"
            "layout(location=0) in vec3 vp;"
            "void main () {"
            "     gl_Position = vec4 (vp, 1.0);"
            "}";

            

        const std::string default_fragment_shader =
            "#version 330\n"
            "out vec4 frag_colour;"
            "void main () {"
            "     frag_colour = vec4 (" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ");"
            "}";
        return create_shader(default_vertex_shader, default_fragment_shader);
    }

    void Shader::use() const
    {
        glUseProgram(m_program);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_program);
        YAZPGP_LOG_DEBUG("Shader deleted id: %d", m_program);
    }

    void Shader::set_uniform(const std::string& name, glm::mat4 value) const
    {
        auto location = glGetUniformLocation(m_program, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}