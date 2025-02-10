#pragma once

#include <string>
#include <DesktopTown/Material.hpp>
#include <DesktopTown/Mesh.hpp>
#include <DesktopTown/GL/GLTexture.hpp>

namespace DesktopTown
{
    class Sprite
    {
    public:
        explicit Sprite(Context *context);

        void Load(const std::string &filename);
        bool Draw(unsigned frame, float left, float bottom, float frame_scale);

    private:
        Context *m_Context;

        GLTexture m_Texture;
        Material m_Material;
        Mesh m_Mesh;

        float m_FrameScale{};
        float m_FrameWidth{}, m_FrameHeight{};
        unsigned m_NumFrames{};
    };
}
