#include <DesktopTown/GL.hpp>

DesktopTown::GLProgram::GLProgram()
    : ID(glCreateProgram())
{
}

DesktopTown::GLProgram::~GLProgram()
{
    glDeleteProgram(ID);
}

bool DesktopTown::GLProgram::IsValid() const
{
    return glIsProgram(ID);
}

std::string DesktopTown::GLProgram::GetInfoLog() const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLchar buf[1024];
    glGetProgramInfoLog(ID, BUF_SIZE, &length, buf);
    return {buf, static_cast<std::string::size_type>(length)};
}

std::vector<GLint> DesktopTown::GLProgram::GetResources(
    const GLenum interface,
    const GLuint index,
    const std::vector<GLenum>& props) const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLint buf[BUF_SIZE];
    glGetProgramResourceiv(
        ID,
        interface,
        index,
        static_cast<GLsizei>(props.size()),
        props.data(),
        BUF_SIZE,
        &length,
        buf);
    return {buf, buf + length};
}

GLuint DesktopTown::GLProgram::GetResourceIndex(const GLenum interface, const std::string& name) const
{
    return glGetProgramResourceIndex(ID, interface, name.c_str());
}

GLint DesktopTown::GLProgram::GetResourceLocation(const GLenum interface, const std::string& name) const
{
    return glGetProgramResourceLocation(ID, interface, name.c_str());
}

GLint DesktopTown::GLProgram::GetResourceLocationIndex(const GLenum interface, const std::string& name) const
{
    return glGetProgramResourceLocationIndex(ID, interface, name.c_str());
}

std::string DesktopTown::GLProgram::GetResourceName(const GLenum interface, const GLuint index) const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLchar buf[BUF_SIZE];
    glGetProgramResourceName(ID, interface, index, BUF_SIZE, &length, buf);
    return {buf, static_cast<std::string::size_type>(length)};
}

DesktopTown::GLBinary DesktopTown::GLProgram::GetBinary() const
{
    constexpr GLsizei BUF_SIZE = 1024 * 10;
    GLsizei length;
    GLenum format;
    char data[BUF_SIZE];
    glGetProgramBinary(ID, BUF_SIZE, &length, &format, &data);
    return {format, {data, data + length}};
}

std::vector<GLuint> DesktopTown::GLProgram::GetAttached() const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLuint buf[BUF_SIZE];
    glGetAttachedShaders(ID, BUF_SIZE, &length, buf);
    return {buf, buf + length};
}

GLint DesktopTown::GLProgram::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(ID, name.c_str());
}

GLint DesktopTown::GLProgram::GetSubroutineUniformLocation(const GLenum type, const std::string& name) const
{
    return glGetSubroutineUniformLocation(ID, type, name.c_str());
}

void DesktopTown::GLProgram::SetBinary(const GLBinary& binary) const
{
    glProgramBinary(ID, binary.Format, binary.Binary.data(), static_cast<GLsizei>(binary.Binary.size()));
}

void DesktopTown::GLProgram::SetParam(const GLenum pname, const GLint value) const
{
    glProgramParameteri(ID, pname, value);
}

void DesktopTown::GLProgram::Attach(const GLShader& shader) const
{
    glAttachShader(ID, shader.ID);
}

void DesktopTown::GLProgram::Detach(const GLShader& shader) const
{
    glDetachShader(ID, shader.ID);
}

void DesktopTown::GLProgram::ShaderStorageBlockBinding(const GLuint index, const GLuint binding) const
{
    glShaderStorageBlockBinding(ID, index, binding);
}

void DesktopTown::GLProgram::Link() const
{
    glLinkProgram(ID);
}

void DesktopTown::GLProgram::Validate() const
{
    glValidateProgram(ID);
}

void DesktopTown::GLProgram::Use() const
{
    glUseProgram(ID);
}
