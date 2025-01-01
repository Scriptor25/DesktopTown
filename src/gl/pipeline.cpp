#include <DesktopTown/GL.hpp>

DesktopTown::GLPipeline::GLPipeline(const GLuint id)
    : GLObject(id)
{
}

DesktopTown::GLPipeline::GLPipeline()
    : GLObject(0)
{
}

DesktopTown::GLPipeline::GLPipeline(GLPipeline&& other) noexcept
    : GLObject(other.ID)
{
    other.ID = 0;
}

DesktopTown::GLPipeline& DesktopTown::GLPipeline::operator=(GLPipeline&& other) noexcept
{
    ID = other.ID;
    other.ID = 0;
    return *this;
}

DesktopTown::GLPipeline::~GLPipeline()
{
    glDeleteProgramPipelines(1, &ID);
}

bool DesktopTown::GLPipeline::IsValid() const
{
    return glIsProgramPipeline(ID);
}

GLint DesktopTown::GLPipeline::GetParam(const GLenum pname) const
{
    GLint param;
    glGetProgramPipelineiv(ID, pname, &param);
    return param;
}

std::string DesktopTown::GLPipeline::GetInfoLog() const
{
    constexpr GLsizei BUF_SIZE = 1024;
    GLsizei length;
    GLchar buf[1024];
    glGetProgramPipelineInfoLog(ID, BUF_SIZE, &length, buf);
    return {buf, static_cast<std::string::size_type>(length)};
}

void DesktopTown::GLPipeline::ActiveShaderProgram(const GLProgram& program) const
{
    glActiveShaderProgram(ID, program.ID);
}

void DesktopTown::GLPipeline::UseProgramStages(const GLbitfield stages, const GLProgram& program) const
{
    glUseProgramStages(ID, stages, program.ID);
}

void DesktopTown::GLPipeline::Validate() const
{
    glValidateProgramPipeline(ID);
}

void DesktopTown::GLPipeline::Bind() const
{
    glBindProgramPipeline(ID);
}

DesktopTown::GLPipeline DesktopTown::GLPipeline::Create()
{
    GLuint id;
    glGenProgramPipelines(1, &id);
    return GLPipeline(id);
}
