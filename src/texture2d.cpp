#include "texture_2d.hpp"
#include "logger.hpp"
#include <iostream>
namespace yazpgp
{
    Texture2D::Texture2D(const char* bytes, uint32_t width, uint32_t height, uint32_t channels)
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        auto mode = channels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, bytes);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        YAZPGP_LOG_DEBUG(
            "Texture loaded: %d (%dx%dx%d)",
            m_texture,
            width,
            height,
            channels
        );

    }

    void Texture2D::use(uint32_t texture_slot) const
    {
        YAZPGP_LOG_FATAL_IF(texture_slot > 31, "Texture slot must be between 0 and 31");
        glActiveTexture(GL_TEXTURE0 + texture_slot);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_texture);
        YAZPGP_LOG_DEBUG("Texture deleted id: %d", m_texture);
    }
    
}
