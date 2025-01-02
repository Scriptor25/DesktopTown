#pragma once

#include <map>
#include <string>
#include <vector>
#include <DesktopTown/DesktopTown.hpp>
#include <GL/glew.h>

namespace DesktopTown
{
    enum GLUniform
    {
        Uniform_Float1,
        Uniform_Float2,
        Uniform_Float3,
        Uniform_Float4,
        Uniform_Int1,
        Uniform_Int2,
        Uniform_Int3,
        Uniform_Int4,
        Uniform_UInt1,
        Uniform_UInt2,
        Uniform_UInt3,
        Uniform_UInt4,
        Uniform_Matrix2x2,
        Uniform_Matrix2x3,
        Uniform_Matrix2x4,
        Uniform_Matrix3x2,
        Uniform_Matrix3x3,
        Uniform_Matrix3x4,
        Uniform_Matrix4x2,
        Uniform_Matrix4x3,
        Uniform_Matrix4x4,
    };

    class GLObject
    {
    protected:
        GLObject(GLenum type, GLuint name);
        virtual ~GLObject() = default;

        GLenum m_Type;
        GLuint m_Name;

    public:
        GLuint GetName() const;

        virtual void SetLabel(const std::string& label) const;
        virtual void RemoveLabel() const;
    };

    class GLBuffer;
    class GLProgram;
    class GLShader;
    class GLTexture;
    class GLVertexArray;

    class GLBuffer final : public GLObject
    {
        explicit GLBuffer(GLuint name);

    public:
        GLBuffer();
        ~GLBuffer() override;

        GLBuffer(const GLBuffer&) = delete;
        GLBuffer& operator=(const GLBuffer&) = delete;
        GLBuffer(GLBuffer&& other) noexcept;
        GLBuffer& operator=(GLBuffer&& other) noexcept;

        void Bind(GLenum target) const;

        template <typename T>
        void Data(const GLsizeiptr count, const GLenum usage) const
        {
            glNamedBufferData(m_Name, count * sizeof(T), nullptr, usage);
        }

        template <typename T>
        void Data(const std::vector<T>& data, const GLenum usage) const
        {
            glNamedBufferData(m_Name, data.size() * sizeof(T), data.data(), usage);
        }

        template <typename T>
        void SubData(const GLintptr offset, const std::vector<T>& data) const
        {
            glNamedBufferSubData(m_Name, offset, data.size() * sizeof(T), data.data());
        }

        static GLBuffer Create();
    };

    class GLProgram final : public GLObject
    {
        explicit GLProgram(GLuint name);

    public:
        GLProgram();
        ~GLProgram() override;

        GLProgram(const GLProgram&) = delete;
        GLProgram& operator=(const GLProgram&) = delete;
        GLProgram(GLProgram&& other) noexcept;
        GLProgram& operator=(GLProgram&& other) noexcept;

        void Attach(const GLShader& shader) const;
        void Detach(const GLShader& shader) const;
        void Link() const;
        void Use() const;
        void Validate() const;

        template <GLUniform U, typename... T>
        void SetUniform(const std::string& name, T... values)
        {
            static const std::map<GLUniform, void*> PFNS
            {
                {Uniform_Float1, reinterpret_cast<void*>(glProgramUniform1f)},
                {Uniform_Float2, reinterpret_cast<void*>(glProgramUniform2f)},
                {Uniform_Float3, reinterpret_cast<void*>(glProgramUniform3f)},
                {Uniform_Float4, reinterpret_cast<void*>(glProgramUniform4f)},
                {Uniform_Int1, reinterpret_cast<void*>(glProgramUniform1i)},
                {Uniform_Int2, reinterpret_cast<void*>(glProgramUniform2i)},
                {Uniform_Int3, reinterpret_cast<void*>(glProgramUniform3i)},
                {Uniform_Int4, reinterpret_cast<void*>(glProgramUniform4i)},
                {Uniform_UInt1, reinterpret_cast<void*>(glProgramUniform1ui)},
                {Uniform_UInt2, reinterpret_cast<void*>(glProgramUniform2ui)},
                {Uniform_UInt3, reinterpret_cast<void*>(glProgramUniform3ui)},
                {Uniform_UInt4, reinterpret_cast<void*>(glProgramUniform4ui)},
            };

            const auto location = glGetUniformLocation(m_Name, name.c_str());
            if (location < 0)
            {
                Error("no uniform with name '{}' in program", name);
                return;
            }

            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<
                void(*)(
                    GLuint program,
                    GLint location,
                    T... values)
            >(pfn);

            fn(m_Name, location, values...);
        }

        template <GLUniform U, typename T>
        void SetUniformMatrix(const std::string& name, const GLsizei count, const GLboolean transpose, const T* value)
        {
            static const std::map<GLUniform, void*> PFNS
            {
                {Uniform_Matrix2x2, reinterpret_cast<void*>(glProgramUniformMatrix2fv)},
                {Uniform_Matrix2x3, reinterpret_cast<void*>(glProgramUniformMatrix2x3fv)},
                {Uniform_Matrix2x4, reinterpret_cast<void*>(glProgramUniformMatrix2x4fv)},
                {Uniform_Matrix3x2, reinterpret_cast<void*>(glProgramUniformMatrix3x2fv)},
                {Uniform_Matrix3x3, reinterpret_cast<void*>(glProgramUniformMatrix3fv)},
                {Uniform_Matrix3x4, reinterpret_cast<void*>(glProgramUniformMatrix3x4fv)},
                {Uniform_Matrix4x2, reinterpret_cast<void*>(glProgramUniformMatrix4x2fv)},
                {Uniform_Matrix4x3, reinterpret_cast<void*>(glProgramUniformMatrix4x3fv)},
                {Uniform_Matrix4x4, reinterpret_cast<void*>(glProgramUniformMatrix4fv)},
            };

            const auto location = glGetUniformLocation(m_Name, name.c_str());
            if (location < 0)
            {
                Error("no uniform with name '{}' in program", name);
                return;
            }

            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<
                void(*)(
                    GLuint program,
                    GLint location,
                    GLsizei count,
                    GLboolean transpose,
                    const T* value)
            >(pfn);

            fn(m_Name, location, count, transpose, value);
        }

        static GLProgram Create();
    };

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

        void SetSource(const std::string& string) const;
        void Compile() const;

        static GLShader Create(GLenum stage);
    };

    class GLTexture final : public GLObject
    {
        explicit GLTexture(GLuint name);

    public:
        GLTexture();
        ~GLTexture() override;

        GLTexture(const GLTexture&) = delete;
        GLTexture& operator=(const GLTexture&) = delete;
        GLTexture(GLTexture&& other) noexcept;
        GLTexture& operator=(GLTexture&& other) noexcept;

        void Bind(GLenum target) const;
        void SetParameter(GLenum pname, GLint param) const;
        void Image2D(
            GLenum target,
            GLint level,
            GLint internalformat,
            GLsizei width,
            GLsizei height,
            GLenum format,
            GLenum type,
            const void* pixels) const;
        void SubImage2D(
            GLint level,
            GLint xoffset,
            GLint yoffset,
            GLsizei width,
            GLsizei height,
            GLenum format,
            GLenum type,
            const void* pixels) const;

        static GLTexture Create();
    };

    class GLVertexArray final : public GLObject
    {
        explicit GLVertexArray(GLuint name);

    public:
        GLVertexArray();
        ~GLVertexArray() override;

        GLVertexArray(const GLVertexArray&) = delete;
        GLVertexArray& operator=(const GLVertexArray&) = delete;
        GLVertexArray(GLVertexArray&& other) noexcept;
        GLVertexArray& operator=(GLVertexArray&& other) noexcept;

        void Bind() const;
        void EnableAttrib(GLuint index) const;
        void AttribPointer(
            GLuint index,
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
            unsigned offset) const;

        static GLVertexArray Create();
    };
}
