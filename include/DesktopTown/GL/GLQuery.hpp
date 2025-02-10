#pragma once

#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLQuery final : public GLObject
    {
        explicit GLQuery(GLuint name);

    public:
        GLQuery();
        ~GLQuery() override;

        GLQuery(const GLQuery &) = delete;
        GLQuery &operator=(const GLQuery &) = delete;
        GLQuery(GLQuery &&other) noexcept;
        GLQuery &operator=(GLQuery &&other) noexcept;

        void Measure(GLenum target, const std::function<void()> &callback) const;

        static GLQuery Create();
    };
}
