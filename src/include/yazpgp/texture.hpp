#pragma once
#include <string>
#include <memory>
#include <GL/glew.h>

namespace yazpgp
{
    class Texture
    {
    public:
        using TextureId = GLuint;
        virtual void use(uint32_t texture_slot) const = 0;
    protected:
        TextureId m_texture;
    };
}