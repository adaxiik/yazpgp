#pragma once
#include <string>
#include <memory>
#include <GL/glew.h>

namespace yazpgp
{
    class Texture
    {
        using TextureId = GLuint;
        TextureId m_texture;
    public:
        Texture(const char* bytes, uint32_t width, uint32_t height, uint32_t channels);
        ~Texture();
        
        void use(uint32_t texture_slot) const;
    };
}