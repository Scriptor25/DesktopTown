#include <DesktopTown/GL.hpp>

DesktopTown::GLTextures::GLTextures(const GLsizei n)
    : IDs(n)
{
    glGenTextures(n, IDs.data());
}

DesktopTown::GLTextures::~GLTextures()
{
    glDeleteTextures(IDs.size(), IDs.data());
}

void DesktopTown::GLTextures::Bind(const GLenum target, const GLuint index) const
{
    glBindTexture(target, IDs[index]);
}
