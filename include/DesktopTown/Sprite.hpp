#pragma once

#include <string>
#include <DesktopTown/GL.hpp>

namespace DesktopTown
{
    class Sprite
    {
    public:
        void Load(const std::string& filename);

    private:
        GLTexture m_Texture;

        float m_FrameHeight{};
        unsigned m_NumFrames{};
    };
}
