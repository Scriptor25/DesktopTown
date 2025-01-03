#include <DesktopTown/GL.hpp>

DesktopTown::GLRenderbuffer::GLRenderbuffer(const GLuint name)
    : GLObject(GL_RENDERBUFFER, name)
{
}

DesktopTown::GLRenderbuffer::GLRenderbuffer()
    : GLObject(GL_RENDERBUFFER, 0)
{
}

DesktopTown::GLRenderbuffer::~GLRenderbuffer()
{
    glDeleteFramebuffers(1, &m_Name);
}

DesktopTown::GLRenderbuffer::GLRenderbuffer(GLRenderbuffer&& other) noexcept
    : GLObject(GL_RENDERBUFFER, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLRenderbuffer& DesktopTown::GLRenderbuffer::operator=(GLRenderbuffer&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

DesktopTown::GLRenderbuffer DesktopTown::GLRenderbuffer::Create()
{
    GLuint name;
    glCreateRenderbuffers(1, &name);
    return GLRenderbuffer(name);
}
