#include <DesktopTown/Sprite.hpp>

DesktopTown::Sprite::Sprite()
    : m_Texture(GLTexture::Create())
{
}

void DesktopTown::Sprite::Load(const std::string& filename)
{
    int width, height;
    const void* pixels;

    m_Texture.Bind(GL_TEXTURE_2D);
    m_Texture.Image2D(0, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}
