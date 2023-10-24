#pragma once

#include <string>
#include <memory>
#include <GL/glew.h>
#include <array>

#include "texture.hpp"

namespace yazpgp
{
    class CubeMap : public Texture
    {
    public:
        struct CubeMapDataPart
        {
            const char* bytes;
            int32_t width;
            int32_t height;
            uint32_t channels;
        };

        CubeMap(const std::array<CubeMapDataPart, 6>& data);
        ~CubeMap();
        virtual void use(uint32_t texture_slot) const override;
    };
}