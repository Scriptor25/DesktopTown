#include <stb_image.h>
#include <DesktopTown/DesktopTown.hpp>
#include <DesktopTown/Sprite.hpp>

DesktopTown::Sprite::Sprite()
    : m_Texture(GLTexture::Create())
{
}

void DesktopTown::Sprite::Load(const std::string& filename)
{
    int width, height;
    const auto pixels = stbi_load(filename.c_str(), &width, &height, nullptr, 4);

    if (!pixels)
    {
        Error("[Sprite::Load] failed to load image file '{}': {}", filename, stbi_failure_reason());
        return;
    }

    m_Texture.Bind(GL_TEXTURE_2D);
    m_Texture.Image2D(0, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

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
