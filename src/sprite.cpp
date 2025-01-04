#include <stb_image.h>
#include <DesktopTown/Error.hpp>
#include <DesktopTown/Sprite.hpp>

DesktopTown::Sprite::Sprite()
    : m_Program(GLProgram::Create())
{
    const std::vector<Vertex> vertices
    {
        {{0, 0}, {0, 0}},
        {{0, 1}, {0, 1}},
        {{1, 1}, {1, 1}},
        {{1, 0}, {1, 0}},
    };

    const std::vector indices
    {
        0u, 1u, 2u,
        2u, 3u, 0u,
    };

    m_Mesh.SetVertices(vertices);
    m_Mesh.SetIndices(indices);
}

void DesktopTown::Sprite::Load(const std::string& filename)
{
    int width, height;
    const auto pixels = stbi_load(filename.c_str(), &width, &height, nullptr, 4);

    if (!pixels)
    {
        Error(
            GL_DEBUG_TYPE_ERROR,
            0x0002,
            GL_DEBUG_SEVERITY_HIGH,
            "Failed to load image from file '{}': {}",
            filename,
            stbi_failure_reason());
        return;
    }

    m_Texture = GLTexture::Create(GL_TEXTURE_2D);
    m_Texture.Storage(1, GL_RGBA32F, width, height);
    m_Texture.SubImage(0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    if (height % width != 0)
    {
        m_FrameHeight = 0.f;
        m_NumFrames = 0;
    }
    else
    {
        m_FrameHeight = static_cast<float>(width) / static_cast<float>(height);
        m_NumFrames = height / width;
    }
}

void DesktopTown::Sprite::Draw(unsigned frame, float x, float y, float s)
{
}
