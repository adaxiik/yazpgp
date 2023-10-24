#pragma once
#include "texture.hpp"
namespace yazpgp
{
    class Texture2D: public Texture
    {
    public:
        Texture2D(const char* bytes, uint32_t width, uint32_t height, uint32_t channels);
        ~Texture2D();
        
        virtual void use(uint32_t texture_slot) const override;
    };
}
