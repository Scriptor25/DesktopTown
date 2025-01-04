#pragma once

#include <string>
#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLShader final : public GLObject
    {
        explicit GLShader(GLuint name);

    public:
        GLShader();
        ~GLShader() override;

        GLShader(const GLShader&) = delete;
        GLShader& operator=(const GLShader&) = delete;
        GLShader(GLShader&& other) noexcept;
        GLShader& operator=(GLShader&& other) noexcept;

        [[nodiscard]] GLint Get(GLenum pname) const;
        [[nodiscard]] std::string GetInfoLog() const;

        void SetSource(const std::string& string) const;
        void Compile() const;

        [[nodiscard]] bool CompileAndCheck() const;

        static GLShader Create(GLenum stage);
    };
}
