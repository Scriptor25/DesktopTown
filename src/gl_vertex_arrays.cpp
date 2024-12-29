#include <DesktopTown/GL.hpp>

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
