#include <DesktopTown/GL.hpp>

DesktopTown::GLBuffer::GLBuffer(const GLuint id)
    : GLObject(id)
{
}

DesktopTown::GLBuffer::GLBuffer()
    : GLObject(0)
{
}

DesktopTown::GLBuffer::GLBuffer(GLBuffer&& other) noexcept
    : GLObject(other.ID)
{
    other.ID = 0;
}

DesktopTown::GLBuffer& DesktopTown::GLBuffer::operator=(GLBuffer&& other) noexcept
{
    ID = other.ID;
    other.ID = 0;
    return *this;
}

DesktopTown::GLBuffer::~GLBuffer()
{
    glDeleteBuffers(1, &ID);
}

void DesktopTown::GLBuffer::Bind(const GLenum target)
{
    glBindBuffer(m_Target = target, ID);
}

DesktopTown::GLBuffer DesktopTown::GLBuffer::Create()
{
    GLuint id;
    glGenBuffers(1, &id);
    return GLBuffer(id);
}
