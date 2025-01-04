#include <DesktopTown/GL/GLQuery.hpp>

DesktopTown::GLQuery::GLQuery(const GLuint name)
    : GLObject(GL_QUERY, name)
{
}

DesktopTown::GLQuery::GLQuery()
    : GLObject(GL_QUERY, 0)
{
}

DesktopTown::GLQuery::~GLQuery()
{
    glDeleteQueries(1, &m_Name);
}

DesktopTown::GLQuery::GLQuery(GLQuery&& other) noexcept
    : GLObject(GL_QUERY, other.m_Name)
{
    other.m_Name = 0;
}

DesktopTown::GLQuery& DesktopTown::GLQuery::operator=(GLQuery&& other) noexcept
{
    std::swap(m_Name, other.m_Name);
    return *this;
}

void DesktopTown::GLQuery::Measure(const GLenum target, const std::function<void()>& callback) const
{
    glBeginQuery(target, m_Name);
    callback();
    glEndQuery(target);
}

DesktopTown::GLQuery DesktopTown::GLQuery::Create()
{
    GLuint name;
    glGenQueries(1, &name);
    return GLQuery(name);
}
