#include <DesktopTown/GL/GLBuffer.hpp>

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

DesktopTown::GLBuffer::GLBuffer(GLBuffer &&other) noexcept
    : GLObject(GL_BUFFER, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLBuffer &DesktopTown::GLBuffer::operator=(GLBuffer &&other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void *DesktopTown::GLBuffer::Map(const GLbitfield access) const
{
    return glMapNamedBuffer(m_Name, access);
}

void *DesktopTown::GLBuffer::MapRange(const GLintptr offset, const GLsizeiptr length, const GLbitfield access) const
{
    return glMapNamedBufferRange(m_Name, offset, length, access);
}

bool DesktopTown::GLBuffer::Unmap() const
{
    return glUnmapNamedBuffer(m_Name);
}

DesktopTown::GLDefer DesktopTown::GLBuffer::Bind(const GLenum target) const
{
    glBindBuffer(target, m_Name);
    return GLDefer(
        [target]
        {
            glBindBuffer(target, 0);
        });
}

void DesktopTown::GLBuffer::Copy(
    const GLBuffer &write_buffer,
    const GLintptr read_offset,
    const GLintptr write_offset,
    const GLsizeiptr size) const
{
    glCopyNamedBufferSubData(m_Name, write_buffer.GetName(), read_offset, write_offset, size);
}

DesktopTown::GLBuffer DesktopTown::GLBuffer::Create()
{
    GLuint name;
    glCreateBuffers(1, &name);
    return GLBuffer(name);
}
