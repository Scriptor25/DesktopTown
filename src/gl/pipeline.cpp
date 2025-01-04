#include <DesktopTown/GL/GLPipeline.hpp>
#include <DesktopTown/GL/GLProgram.hpp>

DesktopTown::GLPipeline::GLPipeline(const GLuint name)
    : GLObject(GL_PROGRAM_PIPELINE, name)
{
}

DesktopTown::GLPipeline::GLPipeline()
    : GLObject(GL_PROGRAM_PIPELINE, 0)
{
}

DesktopTown::GLPipeline::~GLPipeline()
{
    glDeleteProgramPipelines(1, &m_Name);
}

DesktopTown::GLPipeline::GLPipeline(GLPipeline&& other) noexcept
    : GLObject(GL_PROGRAM_PIPELINE, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLPipeline& DesktopTown::GLPipeline::operator=(GLPipeline&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

DesktopTown::GLDefer DesktopTown::GLPipeline::Bind() const
{
    glBindProgramPipeline(m_Name);
    return GLDefer([]
    {
        glBindProgramPipeline(0);
    });
}

void DesktopTown::GLPipeline::UseStages(const GLbitfield stages, const GLProgram& program) const
{
    glUseProgramStages(m_Name, stages, program.GetName());
}

void DesktopTown::GLPipeline::Validate() const
{
    glValidateProgramPipeline(m_Name);
}

DesktopTown::GLPipeline DesktopTown::GLPipeline::Create()
{
    GLuint name;
    glCreateProgramPipelines(1, &name);
    return GLPipeline(name);
}
