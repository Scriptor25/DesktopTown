#include <DesktopTown/GL.hpp>

DesktopTown::GLShader::GLShader(const GLuint name)
    : GLObject(GL_SHADER, name)
{
}

DesktopTown::GLShader::GLShader()
    : GLObject(GL_SHADER, 0)
{
}

DesktopTown::GLShader::~GLShader()
{
    glDeleteShader(m_Name);
}

DesktopTown::GLShader::GLShader(GLShader&& other) noexcept
    : GLObject(GL_SHADER, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLShader& DesktopTown::GLShader::operator=(GLShader&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void DesktopTown::GLShader::SetSource(const std::string& string) const
{
    const auto char_string = string.c_str();
    glShaderSource(m_Name, 1, &char_string, nullptr);
}

void DesktopTown::GLShader::Compile() const
{
    glCompileShader(m_Name);
}

DesktopTown::GLShader DesktopTown::GLShader::Create(const GLenum stage)
{
    return GLShader(glCreateShader(stage));
}
