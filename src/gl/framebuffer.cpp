#include <DesktopTown/GL.hpp>

DesktopTown::GLFramebuffer::GLFramebuffer(const GLuint name)
    : GLObject(GL_FRAMEBUFFER, name)
{
}

DesktopTown::GLFramebuffer::GLFramebuffer()
    : GLObject(GL_FRAMEBUFFER, 0)
{
}

DesktopTown::GLFramebuffer::~GLFramebuffer()
{
    glDeleteFramebuffers(1, &m_Name);
}

DesktopTown::GLFramebuffer::GLFramebuffer(GLFramebuffer&& other) noexcept
    : GLObject(GL_FRAMEBUFFER, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLFramebuffer& DesktopTown::GLFramebuffer::operator=(GLFramebuffer&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

DesktopTown::GLFramebuffer DesktopTown::GLFramebuffer::Create()
{
    GLuint name;
    glCreateFramebuffers(1, &name);
    return GLFramebuffer(name);
}
