#pragma once

#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLRenderbuffer final : public GLObject
    {
        explicit GLRenderbuffer(GLuint name);

    public:
        GLRenderbuffer();
        ~GLRenderbuffer() override;

        GLRenderbuffer(const GLRenderbuffer &) = delete;
        GLRenderbuffer &operator=(const GLRenderbuffer &) = delete;
        GLRenderbuffer(GLRenderbuffer &&other) noexcept;
        GLRenderbuffer &operator=(GLRenderbuffer &&other) noexcept;

        static GLRenderbuffer Create();
    };
}
