#include <DesktopTown/GL/GLTexture.hpp>

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

DesktopTown::GLDefer DesktopTown::GLTexture::Bind(const GLenum target) const
{
    glBindTexture(target, m_Name);
    return GLDefer([target]
    {
        glBindTexture(target, 0);
    });
}

void DesktopTown::GLTexture::BindUnit(const GLuint unit) const
{
    glBindTextureUnit(unit, m_Name);
}

void DesktopTown::GLTexture::SetParameter(const GLenum pname, const GLint param) const
{
    glTextureParameteri(m_Name, pname, param);
}

void DesktopTown::GLTexture::Storage(const GLsizei levels, const GLenum internalformat, const GLsizei width) const
{
    glTextureStorage1D(m_Name, levels, internalformat, width);
}

void DesktopTown::GLTexture::Storage(
    const GLsizei levels,
    const GLenum internalformat,
    const GLsizei width,
    const GLsizei height) const
{
    glTextureStorage2D(m_Name, levels, internalformat, width, height);
}

void DesktopTown::GLTexture::Storage(
    const GLsizei levels,
    const GLenum internalformat,
    const GLsizei width,
    const GLsizei height,
    const GLsizei depth) const
{
    glTextureStorage3D(m_Name, levels, internalformat, width, height, depth);
}

void DesktopTown::GLTexture::SubImage(
    const GLint level,
    const GLint xoffset,
    const GLsizei width,
    const GLenum format,
    const GLenum type,
    const void* pixels) const
{
    glTextureSubImage1D(m_Name, level, xoffset, width, format, type, pixels);
}

void DesktopTown::GLTexture::SubImage(
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

void DesktopTown::GLTexture::SubImage(
    const GLint level,
    const GLint xoffset,
    const GLint yoffset,
    const GLint zoffset,
    const GLsizei width,
    const GLsizei height,
    const GLsizei depth,
    const GLenum format,
    const GLenum type,
    const void* pixels) const
{
    glTextureSubImage3D(m_Name, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

DesktopTown::GLTexture DesktopTown::GLTexture::View(
    const GLenum target,
    const GLenum internalformat,
    const GLuint min_level,
    const GLuint num_levels,
    const GLuint min_layer,
    const GLuint num_layers) const
{
    GLuint dest;
    glGenTextures(1, &dest);

    glTextureView(dest, target, m_Name, internalformat, min_level, num_levels, min_layer, num_layers);

    return GLTexture(dest);
}

DesktopTown::GLTexture DesktopTown::GLTexture::Create(const GLenum target)
{
    GLuint name;
    glCreateTextures(target, 1, &name);
    return GLTexture(name);
}
