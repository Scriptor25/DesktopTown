#include <DesktopTown/GL.hpp>

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

DesktopTown::GLPipeline DesktopTown::GLPipeline::Create()
{
    GLuint name;
    glCreateProgramPipelines(1, &name);
    return GLPipeline(name);
}
