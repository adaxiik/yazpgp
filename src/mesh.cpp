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

        this->init_vao();
        this->init_vbo(vertices, size_bytes);
        layout.use();

        
        std::vector<uint32_t> indices(m_vert_count);
        std::iota(indices.begin(), indices.end(), 0);
        m_index_count = indices.size();

        this->init_ebo(indices.data(), indices.size() * sizeof(uint32_t));

        YAZPGP_LOG_DEBUG("Mesh loaded with vao: %d, ebo: %d, verts: %lu, indices: %lu, tris: %lu", m_vao, m_ebo, m_vert_count, m_index_count, m_index_count / 3);
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const VertexAttributeLayout& layout)
        : m_vert_count(vertices.size()), m_index_count(indices.size())
    {

        static_assert(sizeof(Vertex) == 11 * sizeof(float));
        YAZPGP_LOG_FATAL_IF(m_vert_count * layout.get_stride() != vertices.size() * sizeof(Vertex), "Vertex size mismatch");
        std::unique_ptr<float[]> vertices_data = std::make_unique<float[]>(vertices.size() * layout.get_stride());

        std::memcpy(vertices_data.get(), vertices.data(), vertices.size() * sizeof(Vertex));

        this->init_vao();
        this->init_vbo(vertices_data.get(), vertices.size() * sizeof(Vertex));
        layout.use();
        this->init_ebo(indices.data(), indices.size() * sizeof(uint32_t));

        YAZPGP_LOG_DEBUG("Mesh loaded with vao: %d, ebo: %d, verts: %lu, indices: %lu, tris: %lu", m_vao, m_ebo, m_vert_count, m_index_count, m_index_count / 3);
    }

    void Mesh::init_vao()
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao); 
        // glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        // layout.use();
        // glBindVertexArray(m_vao);
    }

    void Mesh::init_vbo(const float* vertices, size_t size_bytes)
    {
        glGenBuffers(1, &m_vbo); 
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, size_bytes, vertices, GL_STATIC_DRAW);
    }

    void Mesh::init_ebo(const uint32_t* indices, size_t size_bytes)
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
        glDeleteBuffers(1, &m_ebo);
        glDeleteVertexArrays(1, &m_vao);

        YAZPGP_LOG_DEBUG("Mesh deleted with vao: %d, ebo: %d", m_vao, m_ebo);
    }

    std::unique_ptr<Mesh> Mesh::create_cube()
    {
        // Kindly borrowed from:
        // https://learnopengl.com/code_viewer.php?code=advanced/cubemaps_skybox_data
        const static float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        return std::make_unique<Mesh>(
            skyboxVertices,
            sizeof(skyboxVertices),
            VertexAttributeLayout({{.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE},})
        );
            
    }
}
