#include <DesktopTown/FileUtil.hpp>
#include <DesktopTown/Material.hpp>

DesktopTown::ShaderInfo::ShaderInfo(
    std::filesystem::path binary_path,
    std::filesystem::path source_path,
    const GLenum stage)
    : m_BinaryPath(std::move(binary_path)),
      m_SourcePath(std::move(source_path)),
      m_Stage(stage)
{
}

DesktopTown::ShaderInfo::ShaderInfo(ShaderInfo &&other) noexcept
    : m_Shader(std::move(other.m_Shader)),
      m_BinaryPath(std::move(other.m_BinaryPath)),
      m_SourcePath(std::move(other.m_SourcePath)),
      m_Stage(other.m_Stage)
{
}

DesktopTown::ShaderInfo &DesktopTown::ShaderInfo::operator=(ShaderInfo &&other) noexcept
{
    m_Shader = std::move(other.m_Shader);
    m_BinaryPath = std::move(other.m_BinaryPath);
    m_SourcePath = std::move(other.m_SourcePath);
    m_Stage = other.m_Stage;
    return *this;
}

DesktopTown::GLShader *DesktopTown::ShaderInfo::operator->()
{
    return &m_Shader;
}

const DesktopTown::GLShader *DesktopTown::ShaderInfo::operator->() const
{
    return &m_Shader;
}

DesktopTown::GLShader &DesktopTown::ShaderInfo::operator*()
{
    return m_Shader;
}

const DesktopTown::GLShader &DesktopTown::ShaderInfo::operator*() const
{
    return m_Shader;
}

bool DesktopTown::ShaderInfo::Load()
{
    m_Shader = GLShader::Create(m_Stage);

    auto raw_data = ReadFile(m_BinaryPath, std::ios::binary);
    if (raw_data.empty())
    {
        const auto source = ReadFileText(m_SourcePath);
        m_Shader.SetSource(source);
        return m_Shader.CompileAndCheck();
    }

    const auto format = *reinterpret_cast<GLenum *>(raw_data.data());
    std::vector data(raw_data.begin() + 4, raw_data.end());
    const GLBinary binary
    {
        format,
        std::move(data),
    };

    m_Shader.SetBinary(binary);
    m_Shader.Specialize("main");
    return false;
}

std::filesystem::file_time_type DesktopTown::ShaderInfo::LastWriteTime() const
{
    return exists(m_SourcePath)
               ? last_write_time(m_SourcePath)
               : std::filesystem::file_time_type();
}
