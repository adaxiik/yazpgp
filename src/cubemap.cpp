#include "cubemap.hpp"
#include "logger.hpp"

namespace yazpgp
{
    CubeMap::CubeMap(const std::array<CubeMapDataPart, 6>& data)
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

        for (size_t i = 0; i < 6; i++)
        {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                data[i].width,
                data[i].height,
                0,
                data[i].channels == 4 ? GL_RGBA : GL_RGB,
                GL_UNSIGNED_BYTE,
                data[i].bytes
            );

            YAZPGP_LOG_DEBUG(
                "Cubemap face loaded: %d (%dx%dx%d)",
                m_texture,
                data[i].width,
                data[i].height,
                data[i].channels
            );
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    }

    CubeMap::~CubeMap()
    {
        glDeleteTextures(1, &m_texture);
    }

    void CubeMap::use(uint32_t texture_slot) const
    {
        YAZPGP_LOG_FATAL_IF(texture_slot > 31, "Texture slot must be between 0 and 31");

        glActiveTexture(GL_TEXTURE0 + texture_slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    }
}