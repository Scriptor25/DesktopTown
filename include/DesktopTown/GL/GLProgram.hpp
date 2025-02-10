#pragma once

#include <map>
#include <string>
#include <vector>
#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
    class GLProgram final : public GLObject
    {
        explicit GLProgram(GLuint name);

        std::map<std::string, GLint> m_Locations;

    public:
        GLProgram();
        ~GLProgram() override;

        GLProgram(const GLProgram &) = delete;
        GLProgram &operator=(const GLProgram &) = delete;
        GLProgram(GLProgram &&other) noexcept;
        GLProgram &operator=(GLProgram &&other) noexcept;

        [[nodiscard]] GLint Get(GLenum pname) const;
        [[nodiscard]] std::string GetInfoLog() const;
        [[nodiscard]] bool ValidateAndCheck() const;
        [[nodiscard]] std::vector<GLuint> GetAttachedShaders() const;
        [[nodiscard]] GLBinary GetBinary() const;
        [[nodiscard]] bool SetBinaryAndCheck(const GLBinary &binary) const;

        void Attach(const GLShader &shader) const;
        void Detach(const GLShader &shader) const;
        void Link();
        bool LinkAndCheck();
        void Validate() const;
        [[nodiscard]] GLDefer Bind() const;
        void BindAttribLocation(GLuint index, const std::string &name) const;
        void BindFragDataLocation(GLuint color_number, const std::string &name) const;
        GLint GetUniformLocation(const std::string &name);
        void DetachAll() const;
        void SetBinary(const GLBinary &binary) const;
        void SetSeparable(bool separable) const;

        template<GLUniform U, typename... T>
        void SetUniform(const std::string &name, T... values)
        {
            static const std::map<GLUniform, void *> PFNS
            {
                {Uniform_Float1, reinterpret_cast<void *>(glProgramUniform1f)},
                {Uniform_Float2, reinterpret_cast<void *>(glProgramUniform2f)},
                {Uniform_Float3, reinterpret_cast<void *>(glProgramUniform3f)},
                {Uniform_Float4, reinterpret_cast<void *>(glProgramUniform4f)},
                {Uniform_Int1, reinterpret_cast<void *>(glProgramUniform1i)},
                {Uniform_Int2, reinterpret_cast<void *>(glProgramUniform2i)},
                {Uniform_Int3, reinterpret_cast<void *>(glProgramUniform3i)},
                {Uniform_Int4, reinterpret_cast<void *>(glProgramUniform4i)},
                {Uniform_UInt1, reinterpret_cast<void *>(glProgramUniform1ui)},
                {Uniform_UInt2, reinterpret_cast<void *>(glProgramUniform2ui)},
                {Uniform_UInt3, reinterpret_cast<void *>(glProgramUniform3ui)},
                {Uniform_UInt4, reinterpret_cast<void *>(glProgramUniform4ui)},
            };

            const auto location = GetUniformLocation(name);
            if (location < 0)
                return;

            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<
                void(*)(
                    GLuint program,
                    GLint location,
                    T... values)
            >(pfn);

            fn(m_Name, location, values...);
        }

        template<GLUniform U, typename T>
        void SetUniformMatrix(const std::string &name, const GLsizei count, const GLboolean transpose, const T *value)
        {
            static const std::map<GLUniform, void *> PFNS
            {
                {Uniform_Matrix2x2, reinterpret_cast<void *>(glProgramUniformMatrix2fv)},
                {Uniform_Matrix2x3, reinterpret_cast<void *>(glProgramUniformMatrix2x3fv)},
                {Uniform_Matrix2x4, reinterpret_cast<void *>(glProgramUniformMatrix2x4fv)},
                {Uniform_Matrix3x2, reinterpret_cast<void *>(glProgramUniformMatrix3x2fv)},
                {Uniform_Matrix3x3, reinterpret_cast<void *>(glProgramUniformMatrix3fv)},
                {Uniform_Matrix3x4, reinterpret_cast<void *>(glProgramUniformMatrix3x4fv)},
                {Uniform_Matrix4x2, reinterpret_cast<void *>(glProgramUniformMatrix4x2fv)},
                {Uniform_Matrix4x3, reinterpret_cast<void *>(glProgramUniformMatrix4x3fv)},
                {Uniform_Matrix4x4, reinterpret_cast<void *>(glProgramUniformMatrix4fv)},
            };

            const auto location = GetUniformLocation(name);
            if (location < 0)
                return;

            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<
                void(*)(
                    GLuint program,
                    GLint location,
                    GLsizei count,
                    GLboolean transpose,
                    const T *value)
            >(pfn);

            fn(m_Name, location, count, transpose, value);
        }

        static GLProgram Create();
    };
}
