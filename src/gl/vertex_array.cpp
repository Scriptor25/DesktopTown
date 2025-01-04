#include <DesktopTown/GL/GLBuffer.hpp>
#include <DesktopTown/GL/GLVertexArray.hpp>

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

DesktopTown::GLDefer DesktopTown::GLVertexArray::Bind() const
{
    glBindVertexArray(m_Name);
    return GLDefer([]
    {
        glBindVertexArray(0);
    });
}

void DesktopTown::GLVertexArray::VertexBuffer(
    const GLuint binding_index,
    const GLBuffer& buffer,
    const GLintptr offset,
    const GLsizei stride) const
{
    glVertexArrayVertexBuffer(m_Name, binding_index, buffer.GetName(), offset, stride);
}

void DesktopTown::GLVertexArray::EnableAttrib(const GLuint index) const
{
    glEnableVertexArrayAttrib(m_Name, index);
}

void DesktopTown::GLVertexArray::AttribFormat(
    const GLuint attrib_index,
    const GLint size,
    const GLenum type,
    const GLboolean normalized,
    const GLuint offset) const
{
    glVertexArrayAttribFormat(m_Name, attrib_index, size, type, normalized, offset);
}

void DesktopTown::GLVertexArray::AttribBinding(const GLuint attrib_index, const GLuint binding_index) const
{
    glVertexArrayAttribBinding(m_Name, attrib_index, binding_index);
}

DesktopTown::GLVertexArray DesktopTown::GLVertexArray::Create()
{
    GLuint name;
    glCreateVertexArrays(1, &name);
    return GLVertexArray(name);
}
