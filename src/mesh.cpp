#include "mesh.hpp"
#include "logger.hpp"

namespace yazpgp
{
    Mesh::Mesh(const float* vertices, size_t size_bytes) : m_vert_count(size_bytes / sizeof(float) / 3)
    {
        glGenBuffers(1, &m_vbo); 
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, size_bytes, vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao); 
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        YAZPGP_LOG_DEBUG("Mesh with %ld vertices and id %d created successfully", m_vert_count, m_vao);
    }

    void Mesh::use() const
    {
        glBindVertexArray(m_vao);
    }

    size_t Mesh::get_vert_count() const
    {
        return m_vert_count;
    }

    Mesh::~Mesh()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);

        YAZPGP_LOG_DEBUG("Mesh with id %d freed successfully", m_vao);
    }
}
