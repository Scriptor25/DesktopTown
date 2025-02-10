#pragma once

#include <filesystem>
#include <DesktopTown/GL/GLProgram.hpp>
#include <DesktopTown/GL/GLShader.hpp>

namespace DesktopTown
{
    class ShaderInfo
    {
    public:
        ShaderInfo() = default;
        ShaderInfo(std::filesystem::path binary_path, std::filesystem::path source_path, GLenum stage);

        ShaderInfo(const ShaderInfo &) = delete;
        ShaderInfo &operator=(const ShaderInfo &) = delete;
        ShaderInfo(ShaderInfo &&other) noexcept;
        ShaderInfo &operator=(ShaderInfo &&other) noexcept;

        GLShader *operator->();
        const GLShader *operator->() const;

        GLShader &operator*();
        const GLShader &operator*() const;

        [[nodiscard]] bool Load();

        [[nodiscard]] std::filesystem::file_time_type LastWriteTime() const;

    private:
        GLShader m_Shader;

        std::filesystem::path m_BinaryPath;
        std::filesystem::path m_SourcePath;
        GLenum m_Stage{};
    };

    class Material
    {
    public:
        Material() = default;
        Material(std::filesystem::path binary_path, std::vector<ShaderInfo> shader_infos);

        Material(const Material &) = delete;
        Material &operator=(const Material &) = delete;
        Material(Material &&other) noexcept;
        Material &operator=(Material &&other) noexcept;

        GLProgram *operator->();
        const GLProgram *operator->() const;

        GLProgram &operator*();
        const GLProgram &operator*() const;

        [[nodiscard]] bool Load();

    private:
        GLProgram m_Program;

        std::filesystem::path m_BinaryPath;
        std::vector<ShaderInfo> m_ShaderInfos;
    };
}
