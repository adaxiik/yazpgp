#include "vertex_attributes.hpp"
#include "logger.hpp"
#include <algorithm>
namespace yazpgp
{
    VertexAttributeLayout::VertexAttributeLayout(const std::vector<VertexAttribute>& attributes)
        : m_attributes(attributes)
    {
        if (m_attributes.empty())
            YAZPGP_LOG_WARN("VertexAttributeLayout created with no attributes");
        
        if (std::any_of(m_attributes.begin(), m_attributes.end(), [](const auto& attribute) { return attribute.size == 0; }))
            YAZPGP_LOG_WARN("VertexAttributeLayout created with an attribute with size 0");
        
        if (std::any_of(m_attributes.begin(), m_attributes.end(), [](const auto& attribute) { return attribute.type != GL_FLOAT; }))
            YAZPGP_LOG_FATAL("VertexAttributeLayout supports only GL_FLOAT attributes for now");

        m_stride = 0;
        for (const auto& attribute : m_attributes)
            m_stride += attribute.size * sizeof(float);
    }

    void VertexAttributeLayout::use() const
    {
        // size_t offset = 0;
        // for (size_t i = 0; i < m_attributes.size(); i++)
        // {
        //     const auto& attribute = m_attributes[i];
        //     glVertexAttribPointer(i, attribute.size, attribute.type, attribute.normalized, m_stride, (void*)offset);
        //     glEnableVertexAttribArray(i);
        //     offset += attribute.size * sizeof(float);
        // }
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


    }

    size_t VertexAttributeLayout::get_stride() const
    {
        return m_stride;
    }
}