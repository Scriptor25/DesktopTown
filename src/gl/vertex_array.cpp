#include <DesktopTown/GL.hpp>

DesktopTown::GLVertexArray::GLVertexArray(const GLuint id)
    : GLObject(id)
{
}

DesktopTown::GLVertexArray::GLVertexArray()
    : GLObject(0)
{
}

DesktopTown::GLVertexArray::GLVertexArray(GLVertexArray&& other) noexcept
    : GLObject(other.ID)
{
    other.ID = 0;
}

DesktopTown::GLVertexArray& DesktopTown::GLVertexArray::operator=(GLVertexArray&& other) noexcept
{
    ID = other.ID;
    other.ID = 0;
    return *this;
}

DesktopTown::GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &ID);
}

void DesktopTown::GLVertexArray::Bind() const
{
    glBindVertexArray(ID);
}

void DesktopTown::GLVertexArray::Stride(const GLsizei stride)
{
    m_Stride = stride;
}

DesktopTown::GLVertexArray DesktopTown::GLVertexArray::Create()
{
    GLuint id;
    glGenVertexArrays(1, &id);
    return GLVertexArray(id);
}
