#include <DesktopTown/GL/GLObject.hpp>

DesktopTown::GLObject::GLObject(const GLenum type, const GLuint name)
    : m_Type(type), m_Name(name)
{
}

GLuint DesktopTown::GLObject::GetName() const
{
    return m_Name;
}

std::string DesktopTown::GLObject::GetLabel() const
{
    GLsizei length;
    glGetObjectLabel(m_Type, m_Name, 0, &length, nullptr);

    std::string label(length, 0);
    glGetObjectLabel(m_Type, m_Name, length, &length, label.data());

    return label;
}

void DesktopTown::GLObject::SetLabel(const std::string& label) const
{
    glObjectLabel(m_Type, m_Name, static_cast<GLsizei>(label.size()), label.c_str());
}

void DesktopTown::GLObject::RemoveLabel() const
{
    glObjectLabel(m_Type, m_Name, 0, nullptr);
}
