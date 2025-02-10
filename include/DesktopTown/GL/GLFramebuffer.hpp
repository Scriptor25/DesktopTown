#pragma once

#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLFramebuffer final : public GLObject
    {
        explicit GLFramebuffer(GLuint name);

    public:
        GLFramebuffer();
        ~GLFramebuffer() override;

        GLFramebuffer(const GLFramebuffer &) = delete;
        GLFramebuffer &operator=(const GLFramebuffer &) = delete;
        GLFramebuffer(GLFramebuffer &&other) noexcept;
        GLFramebuffer &operator=(GLFramebuffer &&other) noexcept;

        static GLFramebuffer Create();
    };
}
