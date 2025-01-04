#pragma once

#include <string>
#include <DesktopTown/Mesh.hpp>
#include <DesktopTown/GL/GLProgram.hpp>
#include <DesktopTown/GL/GLTexture.hpp>

namespace DesktopTown
{
    class Sprite
    {
    public:
        Sprite();

        void Load(const std::string& filename);
        void Draw(unsigned frame, float x, float y, float s);

    private:
        GLTexture m_Texture;
        GLProgram m_Program;
        Mesh m_Mesh;

        float m_FrameHeight{};
        unsigned m_NumFrames{};
    };
}
