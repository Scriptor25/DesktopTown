#include <DesktopTown/GL.hpp>

DesktopTown::GLProgram::GLProgram(const GLuint name)
    : GLObject(GL_PROGRAM, name)
{
}

DesktopTown::GLProgram::GLProgram()
    : GLObject(GL_PROGRAM, 0)
{
}

DesktopTown::GLProgram::~GLProgram()
{
    glDeleteProgram(m_Name);
}

DesktopTown::GLProgram::GLProgram(GLProgram&& other) noexcept
    : GLObject(GL_PROGRAM, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLProgram& DesktopTown::GLProgram::operator=(GLProgram&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void DesktopTown::GLProgram::Attach(const GLShader& shader) const
{
    glAttachShader(m_Name, shader.GetName());
}

void DesktopTown::GLProgram::Detach(const GLShader& shader) const
{
    glDetachShader(m_Name, shader.GetName());
}

void DesktopTown::GLProgram::Link() const
{
    glLinkProgram(m_Name);
}

void DesktopTown::GLProgram::Use() const
{
    glUseProgram(m_Name);
}


void DesktopTown::GLProgram::Validate() const
{
    glValidateProgram(m_Name);
}

DesktopTown::GLProgram DesktopTown::GLProgram::Create()
{
    return GLProgram(glCreateProgram());
}
