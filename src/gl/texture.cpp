#include <DesktopTown/GL.hpp>

DesktopTown::GLTexture::GLTexture(const GLuint id)
    : GLObject(id)
{
}

DesktopTown::GLTexture::GLTexture()
    : GLObject(0)
{
}

DesktopTown::GLTexture::GLTexture(GLTexture&& other) noexcept
    : GLObject(other.ID)
{
    other.ID = 0;
}

DesktopTown::GLTexture& DesktopTown::GLTexture::operator=(GLTexture&& other) noexcept
{
    ID = other.ID;
    other.ID = 0;
    return *this;
}

DesktopTown::GLTexture::~GLTexture()
{
    glDeleteTextures(1, &ID);
}

GLint DesktopTown::GLTexture::LevelParam(const GLint level, const GLenum pname) const
{
    GLint param;
    glGetTexLevelParameteriv(m_Target, level, pname, &param);
    return param;
}

void DesktopTown::GLTexture::Bind(const GLenum target)
{
    glBindTexture(m_Target = target, ID);
}

void DesktopTown::GLTexture::Param(const GLenum pname, const GLint param) const
{
    glTexParameteri(m_Target, pname, param);
}

void DesktopTown::GLTexture::Image2D(
    const GLint level,
    const GLint internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLenum format,
    const GLenum type,
    const void* pixels) const
{
    glTexImage2D(m_Target, level, internalformat, width, height, 0, format, type, pixels);
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
    glTexSubImage2D(m_Target, level, xoffset, yoffset, width, height, format, type, pixels);
}

DesktopTown::GLTexture DesktopTown::GLTexture::Create()
{
    GLuint id;
    glGenTextures(1, &id);
    return GLTexture(id);
}
