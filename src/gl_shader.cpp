#include <DesktopTown/GL.hpp>

DesktopTown::GLShader::GLShader(const GLenum type)
    : ID(glCreateShader(type))
{
}

DesktopTown::GLShader::~GLShader()
{
    glDeleteShader(ID);
}

bool DesktopTown::GLShader::IsValid() const
{
    return glIsShader(ID);
}

GLint DesktopTown::GLShader::GetParam(const GLenum pname) const
{
    GLint param;
    glGetShaderiv(ID, pname, &param);
    return param;
}

std::string DesktopTown::GLShader::GetInfoLog() const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLchar buf[1024];
    glGetShaderInfoLog(ID, BUF_SIZE, &length, buf);
    return {buf, static_cast<std::string::size_type>(length)};
}

std::string DesktopTown::GLShader::GetSource() const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLchar buf[1024];
    glGetShaderSource(ID, BUF_SIZE, &length, buf);
    return {buf, static_cast<std::string::size_type>(length)};
}

void DesktopTown::GLShader::SetBinary(const GLBinary& binary) const
{
    const auto buffer = binary.Binary.data();
    glShaderBinary(1, &ID, binary.Format, &buffer, 1);
}

void DesktopTown::GLShader::SetSource(const std::string& source) const
{
    const auto buffer = source.c_str();
    glShaderSource(ID, 1, &buffer, nullptr);
}

void DesktopTown::GLShader::Compile() const
{
    glCompileShader(ID);
}
