#include <DesktopTown/FileUtil.hpp>
#include <DesktopTown/Material.hpp>
#include <DesktopTown/GL/GLShader.hpp>

DesktopTown::Material::Material(const std::vector<Shader>& shaders)
    : m_Program(GLProgram::Create())
{
    for (const auto& [stage_, filename_] : shaders)
    {
        const auto source = ReadFileText(filename_);
        const auto shader = GLShader::Create(stage_);
        shader.SetSource(source);
        if (shader.CompileAndCheck())
            m_Program.Attach(shader);
    }

    (void)m_Program.LinkAndCheck();
    (void)m_Program.ValidateAndCheck();

    m_Program.DetachAll();
}

DesktopTown::Material::Material(GLProgram&& program)
    : m_Program(std::move(program))
{
}
