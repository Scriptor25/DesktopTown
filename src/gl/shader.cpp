#include <DesktopTown/Error.hpp>
#include <DesktopTown/GL/GLShader.hpp>

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

GLint DesktopTown::GLShader::Get(const GLenum pname) const
{
    GLint param;
    glGetShaderiv(m_Name, pname, &param);
    return param;
}

std::string DesktopTown::GLShader::GetInfoLog() const
{
    auto length = Get(GL_INFO_LOG_LENGTH);
    std::string info_log(length, 0);
    glGetShaderInfoLog(m_Name, length, &length, info_log.data());
    info_log.resize(length);
    return info_log;
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

bool DesktopTown::GLShader::CompileAndCheck() const
{
    Compile();

    if (Get(GL_COMPILE_STATUS))
        return true;

    const auto label = GetLabel();
    const auto info_log = GetInfoLog();
    Error(
        GL_DEBUG_TYPE_ERROR,
        0x0005,
        GL_DEBUG_SEVERITY_HIGH,
        "Failed to compile shader '{}' ({}): {}",
        label,
        m_Name,
        info_log);
    return false;
}

DesktopTown::GLShader DesktopTown::GLShader::Create(const GLenum stage)
{
    return GLShader(glCreateShader(stage));
}
