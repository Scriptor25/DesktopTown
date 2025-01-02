#include <DesktopTown/GL.hpp>

DesktopTown::GLBuffer::GLBuffer(const GLuint name)
    : GLObject(GL_BUFFER, name)
{
}

DesktopTown::GLBuffer::GLBuffer()
    : GLObject(GL_BUFFER, 0)
{
}

DesktopTown::GLBuffer::~GLBuffer()
{
    glDeleteBuffers(1, &m_Name);
}

DesktopTown::GLBuffer::GLBuffer(GLBuffer&& other) noexcept
    : GLObject(GL_BUFFER, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLBuffer& DesktopTown::GLBuffer::operator=(GLBuffer&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void DesktopTown::GLBuffer::Bind(const GLenum target) const
{
    glBindBuffer(target, m_Name);
}

DesktopTown::GLBuffer DesktopTown::GLBuffer::Create()
{
    GLuint name;
    glGenBuffers(1, &name);
    return GLBuffer(name);
}
