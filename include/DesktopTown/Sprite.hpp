#pragma once

#include <DesktopTown/GL.hpp>

namespace DesktopTown
{
    class Sprite
    {
    public:
        Sprite();

        void Load(const std::string& filename);

    private:
        GLTexture m_Texture;
    };
}
