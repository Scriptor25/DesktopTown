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

GLint DesktopTown::GLProgram::Get(const GLenum pname) const
{
    GLint param;
    glGetProgramiv(m_Name, pname, &param);
    return param;
}

std::string DesktopTown::GLProgram::GetInfoLog() const
{
    auto length = Get(GL_INFO_LOG_LENGTH);
    std::string info_log(length, 0);
    glGetProgramInfoLog(m_Name, length, &length, info_log.data());
    info_log.resize(length);
    return info_log;
}

void DesktopTown::GLProgram::Attach(const GLShader& shader) const
{
    glAttachShader(m_Name, shader.GetName());
}

void DesktopTown::GLProgram::Detach(const GLShader& shader) const
{
    glDetachShader(m_Name, shader.GetName());
}

void DesktopTown::GLProgram::Link()
{
    m_Locations.clear();
    glLinkProgram(m_Name);
}

bool DesktopTown::GLProgram::LinkAndCheck()
{
    Link();

    if (Get(GL_LINK_STATUS))
        return true;

    const auto label = GetLabel();
    const auto info_log = GetInfoLog();
    Error(
        GL_DEBUG_TYPE_ERROR,
        0x0003,
        GL_DEBUG_SEVERITY_HIGH,
        "Failed to link program '{}' ({}): {}",
        label,
        m_Name,
        info_log);
    return false;
}

void DesktopTown::GLProgram::Validate() const
{
    glValidateProgram(m_Name);
}

bool DesktopTown::GLProgram::ValidateAndCheck() const
{
    Validate();

    if (Get(GL_VALIDATE_STATUS))
        return true;

    const auto label = GetLabel();
    const auto info_log = GetInfoLog();
    Error(
        GL_DEBUG_TYPE_ERROR,
        0x0004,
        GL_DEBUG_SEVERITY_HIGH,
        "Failed to validate program '{}' ({}): {}",
        label,
        m_Name,
        info_log);
    return false;
}

void DesktopTown::GLProgram::Use() const
{
    glUseProgram(m_Name);
}

void DesktopTown::GLProgram::BindAttribLocation(const GLuint index, const std::string& name) const
{
    glBindAttribLocation(m_Name, index, name.c_str());
}

void DesktopTown::GLProgram::BindFragDataLocation(const GLuint color_number, const std::string& name) const
{
    glBindFragDataLocation(m_Name, color_number, name.c_str());
}

GLint DesktopTown::GLProgram::GetUniformLocation(const std::string& name)
{
    if (m_Locations.contains(name))
        return m_Locations.at(name);

    const auto location = glGetUniformLocation(m_Name, name.c_str());
    if (location < 0)
    {
        Error(
            GL_DEBUG_TYPE_ERROR,
            0x0001,
            GL_DEBUG_SEVERITY_MEDIUM,
            "No uniform with name '{}'",
            name);
    }

    return m_Locations[name] = location;
}

std::vector<GLuint> DesktopTown::GLProgram::GetAttachedShaders() const
{
    auto count = Get(GL_ATTACHED_SHADERS);
    std::vector<GLuint> shaders(count);
    glGetAttachedShaders(m_Name, count, &count, shaders.data());
    shaders.resize(count);
    return shaders;
}

void DesktopTown::GLProgram::DetachAll() const
{
    for (const auto& shader : GetAttachedShaders())
        glDetachShader(m_Name, shader);
}

DesktopTown::GLBinary DesktopTown::GLProgram::GetBinary() const
{
    auto length = Get(GL_PROGRAM_BINARY_LENGTH);
    GLenum format;
    std::vector<char> data(length);
    glGetProgramBinary(m_Name, length, &length, &format, data.data());
    data.resize(length);
    return {format, std::move(data)};
}

void DesktopTown::GLProgram::SetBinary(const GLBinary& binary) const
{
    glProgramBinary(m_Name, binary.Format, binary.Data.data(), binary.Data.size());
}

bool DesktopTown::GLProgram::SetBinaryAndCheck(const GLBinary& binary) const
{
    SetBinary(binary);

    if (Get(GL_LINK_STATUS))
        return true;

    const auto label = GetLabel();
    const auto info_log = GetInfoLog();
    Error(
        GL_DEBUG_TYPE_ERROR,
        0x0006,
        GL_DEBUG_SEVERITY_MEDIUM,
        "Failed to set binary for program '{}' ({}): {}",
        label,
        m_Name,
        info_log);
    return false;
}

DesktopTown::GLProgram DesktopTown::GLProgram::Create()
{
    return GLProgram(glCreateProgram());
}
