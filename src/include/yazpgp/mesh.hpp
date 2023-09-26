#pragma once
#include <GL/glew.h>
#include "vertex_attributes.hpp"
#include "vertex.hpp"
#include <vector>
namespace yazpgp
{
    class Mesh
    {
        GLuint m_vao, m_vbo, m_ebo;
        size_t m_vert_count;
        size_t m_index_count;

        void init_vao(const VertexAttributeLayout& layout);
        void init_vbo(const float* vertices, size_t size_bytes);
        void init_ebo(const int* indices, size_t size_bytes);

    public:
        Mesh(const float* vertices, size_t size_bytes, const VertexAttributeLayout& layout);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices, const VertexAttributeLayout& layout);
        ~Mesh();
        void use() const;
        size_t get_vert_count() const; 
        size_t get_index_count() const;       
    };
}