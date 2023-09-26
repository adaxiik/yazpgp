#include "mesh.hpp"
#include "logger.hpp"

#include <numeric>
#include <cstring>
#include <memory>

namespace yazpgp
{
    Mesh::Mesh(const float* vertices, size_t size_bytes, const VertexAttributeLayout& layout)
        : m_vert_count(size_bytes / layout.get_stride())
    {
        this->init_vbo(vertices, size_bytes);

        
        std::vector<int> indices(m_vert_count);
        std::iota(indices.begin(), indices.end(), 0);
        
        m_index_count = indices.size();
        
        this->init_ebo(indices.data(), indices.size() * sizeof(int));
        this->init_vao(layout);

        YAZPGP_LOG_DEBUG("Mesh created with vao: %d, ebo: %d, vert count: %lu and index count: %lu", m_vao, m_ebo, m_vert_count, m_index_count);
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices, const VertexAttributeLayout& layout)
        : m_vert_count(vertices.size()), m_index_count(indices.size())
    {

        static_assert(sizeof(Vertex) == 6 * sizeof(float));
        YAZPGP_LOG_FATAL_IF(m_vert_count * layout.get_stride() != vertices.size() * sizeof(Vertex), "Vertex size mismatch");
        std::unique_ptr<float[]> vertices_data = std::make_unique<float[]>(vertices.size() * layout.get_stride());

        std::memcpy(vertices_data.get(), vertices.data(), vertices.size() * sizeof(Vertex));

        this->init_vbo(vertices_data.get(), vertices.size() * sizeof(Vertex));
        this->init_ebo(indices.data(), indices.size() * sizeof(int));
        this->init_vao(layout);

        YAZPGP_LOG_DEBUG("Mesh created with vao: %d, ebo: %d, vert count: %lu and index count: %lu", m_vao, m_ebo, m_vert_count, m_index_count);
    }

    void Mesh::init_vao(const VertexAttributeLayout& layout)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao); 
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        layout.use();
        glBindVertexArray(m_vao);
    }

    void Mesh::init_vbo(const float* vertices, size_t size_bytes)
    {
        glGenBuffers(1, &m_vbo); 
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, size_bytes, vertices, GL_STATIC_DRAW);
    }

    void Mesh::init_ebo(const int* indices, size_t size_bytes)
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes, indices, GL_STATIC_DRAW);
    }

    void Mesh::use() const
    {
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    }

    size_t Mesh::get_vert_count() const
    {
        return m_vert_count;
    }

    size_t Mesh::get_index_count() const
    {
        return m_index_count;
    }

    Mesh::~Mesh()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);

        YAZPGP_LOG_DEBUG("Mesh freed with vao: %d, ebo: %d", m_vao, m_ebo);
    }
}
