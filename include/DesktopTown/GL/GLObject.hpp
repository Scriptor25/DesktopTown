#pragma once

#include <string>
#include <DesktopTown/GL.hpp>

namespace DesktopTown
{
    class GLObject
    {
    protected:
        GLObject(GLenum type, GLuint name);
        virtual ~GLObject() = default;

        GLenum m_Type;
        GLuint m_Name;

    public:
        [[nodiscard]] GLuint GetName() const;
        [[nodiscard]] virtual std::string GetLabel() const;

        virtual void SetLabel(const std::string &label) const;
        virtual void RemoveLabel() const;
    };
}
