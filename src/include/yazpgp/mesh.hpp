#pragma once
#include <GL/glew.h>
namespace yazpgp
{
    class Mesh
    {
        GLuint m_vao;
        GLuint m_vbo;
        size_t m_vert_count;

    public:
        Mesh(const float* vertices, size_t size_bytes);
        ~Mesh();
        void use() const;
        size_t get_vert_count() const;        
    };
}