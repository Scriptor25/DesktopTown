#include <DesktopTown/GL.hpp>

DesktopTown::GLBuffers::GLBuffers(const GLsizei n)
    : IDs(n)
{
    glGenBuffers(n, IDs.data());
}

DesktopTown::GLBuffers::~GLBuffers()
{
    glDeleteBuffers(IDs.size(), IDs.data());
}

void DesktopTown::GLBuffers::Bind(const GLenum target, const unsigned index) const
{
    glBindBuffer(target, IDs[index]);
}
