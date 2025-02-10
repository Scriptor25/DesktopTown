#pragma once

#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLVertexArray final : public GLObject
    {
        explicit GLVertexArray(GLuint name);

    public:
        GLVertexArray();
        ~GLVertexArray() override;

        GLVertexArray(const GLVertexArray &) = delete;
        GLVertexArray &operator=(const GLVertexArray &) = delete;
        GLVertexArray(GLVertexArray &&other) noexcept;
        GLVertexArray &operator=(GLVertexArray &&other) noexcept;

        [[nodiscard]] GLDefer Bind() const;
        void VertexBuffer(GLuint binding_index, const GLBuffer &buffer, GLintptr offset, GLsizei stride) const;
        void EnableAttrib(GLuint index) const;
        void AttribFormat(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint offset) const;
        void AttribBinding(GLuint attrib_index, GLuint binding_index) const;

        static GLVertexArray Create();
    };
}
