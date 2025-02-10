#include <DesktopTown/FileUtil.hpp>
#include <DesktopTown/Material.hpp>

DesktopTown::Material::Material(std::filesystem::path binary_path, std::vector<ShaderInfo> shader_infos)
    : m_BinaryPath(std::move(binary_path)),
      m_ShaderInfos(std::move(shader_infos))
{
}

DesktopTown::Material::Material(Material &&other) noexcept
    : m_Program(std::move(other.m_Program)),
      m_BinaryPath(std::move(other.m_BinaryPath)),
      m_ShaderInfos(std::move(other.m_ShaderInfos))
{
}

DesktopTown::Material &DesktopTown::Material::operator=(Material &&other) noexcept
{
    m_Program = std::move(other.m_Program);
    m_BinaryPath = std::move(other.m_BinaryPath);
    m_ShaderInfos = std::move(other.m_ShaderInfos);
    return *this;
}

DesktopTown::GLProgram *DesktopTown::Material::operator->()
{
    return &m_Program;
}

const DesktopTown::GLProgram *DesktopTown::Material::operator->() const
{
    return &m_Program;
}

DesktopTown::GLProgram &DesktopTown::Material::operator*()
{
    return m_Program;
}

const DesktopTown::GLProgram &DesktopTown::Material::operator*() const
{
    return m_Program;
}

bool DesktopTown::Material::Load()
{
    m_Program = GLProgram::Create();

    auto raw_data = ReadFile(m_BinaryPath, std::ios::binary);

    const auto binary_time = exists(m_BinaryPath)
                                 ? last_write_time(m_BinaryPath)
                                 : std::filesystem::file_time_type();
    auto need_recompile = raw_data.empty();
    if (!need_recompile)
        for (const auto &shader_info: m_ShaderInfos)
            if (shader_info.LastWriteTime() > binary_time)
            {
                need_recompile = true;
                break;
            }

    if (need_recompile)
    {
        for (auto &shader_info: m_ShaderInfos)
        {
            if (shader_info.Load())
                return true;
            m_Program.Attach(*shader_info);
        }

        if (m_Program.LinkAndCheck())
            return true;
        if (m_Program.ValidateAndCheck())
            return true;

        m_Program.DetachAll();

        const auto [format_, data_] = m_Program.GetBinary();

        raw_data.resize(4 + data_.size());
        *reinterpret_cast<GLenum *>(raw_data.data()) = format_;
        for (unsigned i = 0; i < data_.size(); ++i)
            raw_data[i + 4] = data_[i];

        return WriteFile(m_BinaryPath, raw_data, std::ios::binary);
    }

    const auto format = *reinterpret_cast<GLenum *>(raw_data.data());
    std::vector data(raw_data.begin() + 4, raw_data.end());
    const GLBinary binary
    {
        format,
        std::move(data),
    };

    return m_Program.SetBinaryAndCheck(binary);
}
