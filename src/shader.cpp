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

        GLint success;
        glGetShaderiv(compiled_vertex_shader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            GLchar info_log[512];
            glGetShaderInfoLog(compiled_vertex_shader, 512, NULL, info_log);
            YAZPGP_LOG_ERROR("Failed to compile vertex shader: %s", info_log);
            return nullptr;
        }

        GLuint compiled_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(compiled_fragment_shader, 1, &very_unsafe_and_scary_fragment_source, NULL);
        glCompileShader(compiled_fragment_shader);

        glGetShaderiv(compiled_fragment_shader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            GLchar info_log[512];
            glGetShaderInfoLog(compiled_fragment_shader, 512, NULL, info_log);
            YAZPGP_LOG_ERROR("Failed to compile fragment shader: %s", info_log);
            glDeleteShader(compiled_vertex_shader);
            return nullptr;
        }

        GLuint linked_shader_program = glCreateProgram();
        glAttachShader(linked_shader_program, compiled_fragment_shader);
        glAttachShader(linked_shader_program, compiled_vertex_shader);
        glLinkProgram(linked_shader_program);

        glGetProgramiv(linked_shader_program, GL_LINK_STATUS, &success);
        if (not success)
        {
            GLchar info_log[512];
            glGetProgramInfoLog(linked_shader_program, 512, NULL, info_log);
            YAZPGP_LOG_ERROR("Failed to link shader program: %s", info_log);
            glDeleteShader(compiled_vertex_shader);
            glDeleteShader(compiled_fragment_shader);
            return nullptr;
        }

        glDeleteShader(compiled_vertex_shader);
        glDeleteShader(compiled_fragment_shader);

        YAZPGP_LOG_DEBUG("Shader loaded with id: %d", linked_shader_program);

        return std::make_shared<Shader>(linked_shader_program);
    }

    std::shared_ptr<Shader> Shader::create_default_shader(float r, float g, float b, float a)
    {
        const std::string default_vertex_shader =
            "#version 330\n"
            "layout(location=0) in vec3 vp;"
            "uniform mat4 view_projection_matrix;"
            "uniform mat4 mvp_matrix;"
            "void main () {"
            "     gl_Position = mvp_matrix * vec4 (vp, 1.0);"
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

    void Shader::set_uniform(const std::string& name, const glm::mat4& value) const
    {
        auto location = glGetUniformLocation(m_program, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}