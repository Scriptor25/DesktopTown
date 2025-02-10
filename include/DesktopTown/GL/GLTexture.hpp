#pragma once

#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLTexture final : public GLObject
    {
        explicit GLTexture(GLuint name);

    public:
        GLTexture();
        ~GLTexture() override;

        GLTexture(const GLTexture &) = delete;
        GLTexture &operator=(const GLTexture &) = delete;
        GLTexture(GLTexture &&other) noexcept;
        GLTexture &operator=(GLTexture &&other) noexcept;

        [[nodiscard]] GLDefer Bind(GLenum target) const;
        void BindUnit(GLuint unit) const;
        void SetParameter(GLenum pname, GLint param) const;
        void Storage(GLsizei levels, GLenum internalformat, GLsizei width) const;
        void Storage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) const;
        void Storage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) const;
        void SubImage(
            GLint level,
            GLint xoffset,
            GLsizei width,
            GLenum format,
            GLenum type,
            const void *pixels) const;
        void SubImage(
            GLint level,
            GLint xoffset,
            GLint yoffset,
            GLsizei width,
            GLsizei height,
            GLenum format,
            GLenum type,
            const void *pixels) const;
        void SubImage(
            GLint level,
            GLint xoffset,
            GLint yoffset,
            GLint zoffset,
            GLsizei width,
            GLsizei height,
            GLsizei depth,
            GLenum format,
            GLenum type,
            const void *pixels) const;
        [[nodiscard]] GLTexture View(
            GLenum target,
            GLenum internalformat,
            GLuint min_level,
            GLuint num_levels,
            GLuint min_layer,
            GLuint num_layers) const;

        static GLTexture Create(GLenum target);
    };
}
