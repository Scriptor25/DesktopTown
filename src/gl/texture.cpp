#include <DesktopTown/GL.hpp>

DesktopTown::GLTexture::GLTexture(const GLuint name)
    : GLObject(GL_TEXTURE, name)
{
}

DesktopTown::GLTexture::GLTexture()
    : GLObject(GL_TEXTURE, 0)
{
}

DesktopTown::GLTexture::~GLTexture()
{
    glDeleteTextures(1, &m_Name);
}

DesktopTown::GLTexture::GLTexture(GLTexture&& other) noexcept
    : GLObject(GL_TEXTURE, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLTexture& DesktopTown::GLTexture::operator=(GLTexture&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void DesktopTown::GLTexture::Bind(const GLenum target) const
{
    glBindTexture(target, m_Name);
}

void DesktopTown::GLTexture::SetParameter(const GLenum pname, const GLint param) const
{
    glTextureParameteri(m_Name, pname, param);
}

void DesktopTown::GLTexture::Image2D(
    const GLenum target,
    const GLint level,
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLenum format,
    const GLenum type, const void* pixels) const
{
    glTexImage2D(target, level, internalformat, width, height, 0, format, type, pixels);
}

void DesktopTown::GLTexture::SubImage2D(
    const GLint level,
    const GLint xoffset,
    const GLint yoffset,
    const GLsizei width,
    const GLsizei height,
    const GLenum format,
    const GLenum type,
    const void* pixels) const
{
    glTextureSubImage2D(m_Name, level, xoffset, yoffset, width, height, format, type, pixels);
}

DesktopTown::GLTexture DesktopTown::GLTexture::Create()
{
    GLuint name;
    glGenTextures(1, &name);
    return GLTexture(name);
}
