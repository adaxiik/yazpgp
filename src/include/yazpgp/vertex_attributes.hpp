#pragma once
#include <vector>
#include <GL/glew.h>
namespace yazpgp
{
    struct VertexAttribute
    {
        GLint size;
        GLenum type;
        GLboolean normalized;
    };

    class VertexAttributeLayout
    {
        std::vector<VertexAttribute> m_attributes;
        size_t m_stride;
    
    public:
        VertexAttributeLayout(const std::vector<VertexAttribute>& attributes);
        void use() const;
        size_t get_stride() const;
    };
}