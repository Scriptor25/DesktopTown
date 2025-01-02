#include <DesktopTown/GL.hpp>

DesktopTown::GLObject::GLObject(const GLenum type, const GLuint name)
    : m_Type(type), m_Name(name)
{
}

GLuint DesktopTown::GLObject::GetName() const
{
    return m_Name;
}

void DesktopTown::GLObject::SetLabel(const std::string& label) const
{
    glObjectLabel(m_Type, m_Name, label.size(), label.c_str());
}

void DesktopTown::GLObject::RemoveLabel() const
{
    glObjectLabel(m_Type, m_Name, 0, nullptr);
}
