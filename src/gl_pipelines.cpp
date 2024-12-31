#include <DesktopTown/GL.hpp>

DesktopTown::GLPipelines::GLPipelines(const GLsizei n)
    : IDs(n)
{
    glGenProgramPipelines(n, IDs.data());
}

DesktopTown::GLPipelines::~GLPipelines()
{
    glDeleteProgramPipelines(static_cast<GLsizei>(IDs.size()), IDs.data());
}

bool DesktopTown::GLPipelines::IsValid(const unsigned index) const
{
    return glIsProgramPipeline(IDs.at(index));
}

GLint DesktopTown::GLPipelines::GetParam(const GLenum pname, const unsigned index) const
{
    GLint param;
    glGetProgramPipelineiv(IDs[index], pname, &param);
    return param;
}

std::string DesktopTown::GLPipelines::GetInfoLog(const unsigned index) const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLchar buf[1024];
    glGetProgramPipelineInfoLog(IDs.at(index), BUF_SIZE, &length, buf);
    return {buf, static_cast<std::string::size_type>(length)};
}

void DesktopTown::GLPipelines::ActiveShaderProgram(const GLProgram& program, const unsigned index) const
{
    glActiveShaderProgram(IDs.at(index), program.ID);
}

void DesktopTown::GLPipelines::UseProgramStages(
    const GLbitfield stages,
    const GLProgram& program,
    const unsigned index) const
{
    glUseProgramStages(IDs.at(index), stages, program.ID);
}

void DesktopTown::GLPipelines::Validate(const unsigned index) const
{
    glValidateProgramPipeline(IDs.at(index));
}

void DesktopTown::GLPipelines::Bind(const unsigned index) const
{
    glBindProgramPipeline(IDs.at(index));
}
