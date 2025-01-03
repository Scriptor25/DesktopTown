#pragma once

#include <map>
#include <vector>
#include <DesktopTown/DesktopTown.hpp>

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

    struct GLBinary
    {
        GLenum Format;
        std::vector<char> Data;
    };

    class GLObject
    {
    protected:
        GLObject(GLenum type, GLuint name);
        virtual ~GLObject() = default;

        GLenum m_Type;
        GLuint m_Name;

    public:
        [[nodiscard]] GLuint GetName() const;

        virtual void SetLabel(const std::string& label) const;
        virtual void RemoveLabel() const;
        virtual std::string GetLabel() const;
    };

    class GLBuffer;
    class GLFramebuffer;
    class GLPipeline;
    class GLProgram;
    class GLQuery;
    class GLRenderbuffer;
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
            glNamedBufferSubData(m_Name, offset * sizeof(T), data.size() * sizeof(T), data.data());
        }

        template <typename T>
        void ClearData(const GLenum internalformat, const GLenum format, const GLenum type, const T& data) const
        {
            glClearNamedBufferData(m_Name, internalformat, format, type, &data);
        }

        template <typename T>
        void ClearSubData(
            const GLenum internalformat,
            const GLintptr offset,
            const GLsizeiptr size,
            const GLenum format,
            const GLenum type,
            const T& data) const
        {
            glClearNamedBufferSubData(
                m_Name,
                internalformat,
                offset * sizeof(T),
                size * sizeof(T),
                format,
                type,
                &data);
        }

        void CopySubData(
            const GLBuffer& write_buffer,
            GLintptr read_offset,
            GLintptr write_offset,
            GLsizeiptr size) const;

        template <typename T>
        void Storage(const GLsizeiptr count, const GLbitfield flags) const
        {
            glNamedBufferStorage(m_Name, count * sizeof(T), nullptr, flags);
        }

        template <typename T>
        void Storage(const std::vector<T>& data, const GLbitfield flags) const
        {
            glNamedBufferStorage(m_Name, data.size() * sizeof(T), data.data(), flags);
        }

        [[nodiscard]] void* Map(GLbitfield access) const;
        [[nodiscard]] void* MapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) const;
        [[nodiscard]] bool Unmap() const;

        static GLBuffer Create();
    };

    class GLFramebuffer final : public GLObject
    {
        explicit GLFramebuffer(GLuint name);

    public:
        GLFramebuffer();
        ~GLFramebuffer() override;

        GLFramebuffer(const GLFramebuffer&) = delete;
        GLFramebuffer& operator=(const GLFramebuffer&) = delete;
        GLFramebuffer(GLFramebuffer&& other) noexcept;
        GLFramebuffer& operator=(GLFramebuffer&& other) noexcept;

        static GLFramebuffer Create();
    };

    class GLPipeline final : public GLObject
    {
        explicit GLPipeline(GLuint name);

    public:
        GLPipeline();
        ~GLPipeline() override;

        GLPipeline(const GLPipeline&) = delete;
        GLPipeline& operator=(const GLPipeline&) = delete;
        GLPipeline(GLPipeline&& other) noexcept;
        GLPipeline& operator=(GLPipeline&& other) noexcept;

        static GLPipeline Create();
    };

    class GLProgram final : public GLObject
    {
        explicit GLProgram(GLuint name);

        std::map<std::string, GLint> m_Locations;

    public:
        GLProgram();
        ~GLProgram() override;

        GLProgram(const GLProgram&) = delete;
        GLProgram& operator=(const GLProgram&) = delete;
        GLProgram(GLProgram&& other) noexcept;
        GLProgram& operator=(GLProgram&& other) noexcept;

        GLint Get(GLenum pname) const;
        std::string GetInfoLog() const;
        void Attach(const GLShader& shader) const;
        void Detach(const GLShader& shader) const;
        void Link();
        bool LinkAndCheck();
        void Validate() const;
        bool ValidateAndCheck() const;
        void Use() const;
        void BindAttribLocation(GLuint index, const std::string& name) const;
        void BindFragDataLocation(GLuint color_number, const std::string& name) const;
        GLint GetUniformLocation(const std::string& name);
        std::vector<GLuint> GetAttachedShaders() const;
        void DetachAll() const;
        GLBinary GetBinary() const;
        void SetBinary(const GLBinary& binary) const;
        bool SetBinaryAndCheck(const GLBinary& binary) const;

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
                    const T* value)
            >(pfn);

            fn(m_Name, location, count, transpose, value);
        }

        static GLProgram Create();
    };

    class GLQuery final : public GLObject
    {
        explicit GLQuery(GLuint name);

        GLenum m_Target{};

    public:
        GLQuery();
        ~GLQuery() override;

        GLQuery(const GLQuery&) = delete;
        GLQuery& operator=(const GLQuery&) = delete;
        GLQuery(GLQuery&& other) noexcept;
        GLQuery& operator=(GLQuery&& other) noexcept;

        void Begin(GLenum target);
        void End();

        static GLQuery Create();
    };

    class GLRenderbuffer final : public GLObject
    {
        explicit GLRenderbuffer(GLuint name);

    public:
        GLRenderbuffer();
        ~GLRenderbuffer() override;

        GLRenderbuffer(const GLRenderbuffer&) = delete;
        GLRenderbuffer& operator=(const GLRenderbuffer&) = delete;
        GLRenderbuffer(GLRenderbuffer&& other) noexcept;
        GLRenderbuffer& operator=(GLRenderbuffer&& other) noexcept;

        static GLRenderbuffer Create();
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

        GLint Get(GLenum pname) const;
        std::string GetInfoLog() const;

        void SetSource(const std::string& string) const;
        void Compile() const;

        bool CompileAndCheck() const;

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
            const void* pixels) const;
        void SubImage(
            GLint level,
            GLint xoffset,
            GLint yoffset,
            GLsizei width,
            GLsizei height,
            GLenum format,
            GLenum type,
            const void* pixels) const;
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
            const void* pixels) const;
        GLTexture View(
            GLenum target,
            GLenum internalformat,
            GLuint min_level,
            GLuint num_levels,
            GLuint min_layer,
            GLuint num_layers) const;

        static GLTexture Create(GLenum target);
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
        void VertexBuffer(GLuint binding_index, const GLBuffer& buffer, GLintptr offset, GLsizei stride) const;
        void EnableAttrib(GLuint index) const;
        void AttribFormat(GLuint attrib_index, GLint size, GLenum type, GLboolean normalized, GLuint offset) const;
        void AttribBinding(GLuint attrib_index, GLuint binding_index) const;

        static GLVertexArray Create();
    };
}
