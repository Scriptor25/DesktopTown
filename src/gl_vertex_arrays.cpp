#include <DesktopTown/GL.hpp>

void DesktopTown::GLVertexArrays::EnableVertexAttrib(const GLuint index)
{
    glEnableVertexAttribArray(index);
}

void DesktopTown::GLVertexArrays::DisableVertexAttrib(const GLuint index)
{
    glDisableVertexAttribArray(index);
}

void DesktopTown::GLVertexArrays::VertexAttribPointer(
    const GLuint index,
    const GLint size,
    const GLenum type,
    const GLboolean normalized,
    const GLsizei stride,
    const void* pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void DesktopTown::GLVertexArrays::VertexAttribPointer(
    const GLuint index,
    const GLint size,
    const GLenum type,
    const GLboolean normalized,
    const GLsizei stride,
    const unsigned offset)
{
    glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<const void*>(offset));
}

DesktopTown::GLVertexArrays::GLVertexArrays(const GLsizei n)
    : IDs(n)
{
    glGenVertexArrays(n, IDs.data());
}

DesktopTown::GLVertexArrays::~GLVertexArrays()
{
    glDeleteVertexArrays(IDs.size(), IDs.data());
}

void DesktopTown::GLVertexArrays::Bind(const unsigned index) const
{
    glBindVertexArray(IDs[index]);
}
