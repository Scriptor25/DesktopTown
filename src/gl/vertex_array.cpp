#include <DesktopTown/GL.hpp>

DesktopTown::GLVertexArray::GLVertexArray(const GLuint name)
    : GLObject(GL_VERTEX_ARRAY, name)
{
}

DesktopTown::GLVertexArray::GLVertexArray()
    : GLObject(GL_VERTEX_ARRAY, 0)
{
}

DesktopTown::GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &m_Name);
}

DesktopTown::GLVertexArray::GLVertexArray(GLVertexArray&& other) noexcept
    : GLObject(GL_VERTEX_ARRAY, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLVertexArray& DesktopTown::GLVertexArray::operator=(GLVertexArray&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void DesktopTown::GLVertexArray::Bind() const
{
    glBindVertexArray(m_Name);
}

void DesktopTown::GLVertexArray::EnableAttrib(const GLuint index) const
{
    glEnableVertexArrayAttrib(m_Name, index);
}

void DesktopTown::GLVertexArray::AttribPointer(
    const GLuint index,
    const GLint size,
    const GLenum type,
    const GLboolean normalized,
    const GLsizei stride,
    const unsigned offset) const
{
    glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<const void*>(offset));
}

DesktopTown::GLVertexArray DesktopTown::GLVertexArray::Create()
{
    GLuint name;
    glGenVertexArrays(1, &name);
    return GLVertexArray(name);
}
