#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>

namespace DesktopTown
{
    struct GLBinary
    {
        GLenum Format;
        std::vector<char> Binary;
    };

    enum GLUniform
    {
        UNIFORM_FLOAT_1,
        UNIFORM_FLOAT_2,
        UNIFORM_FLOAT_3,
        UNIFORM_FLOAT_4,
        UNIFORM_INT_1,
        UNIFORM_INT_2,
        UNIFORM_INT_3,
        UNIFORM_INT_4,
        UNIFORM_UINT_1,
        UNIFORM_UINT_2,
        UNIFORM_UINT_3,
        UNIFORM_UINT_4,
        UNIFORM_MATRIX_2_2,
        UNIFORM_MATRIX_2_3,
        UNIFORM_MATRIX_2_4,
        UNIFORM_MATRIX_3_2,
        UNIFORM_MATRIX_3_3,
        UNIFORM_MATRIX_3_4,
        UNIFORM_MATRIX_4_2,
        UNIFORM_MATRIX_4_3,
        UNIFORM_MATRIX_4_4,
    };

    class GLObject
    {
    public:
        GLObject(const GLObject&) = delete;
        GLObject& operator=(const GLObject&) = delete;
        virtual ~GLObject() = default;

    protected:
        explicit GLObject(GLuint id);

        GLuint ID;
    };

    class GLShader final : public GLObject
    {
        friend class GLProgram;

        explicit GLShader(GLuint id);

    public:
        GLShader();
        GLShader(GLShader&&) noexcept;
        GLShader& operator=(GLShader&&) noexcept;
        ~GLShader() override;

        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] GLint GetParam(GLenum pname) const;

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetShaderiv(ID, pname, params.data());
            return params;
        }

        [[nodiscard]] std::string GetInfoLog() const;
        [[nodiscard]] std::string GetSource() const;

        void SetBinary(const GLBinary& binary) const;
        void SetSource(const std::string& source) const;
        void Compile() const;

        static GLShader Create(GLenum type);
    };

    class GLProgram final : public GLObject
    {
        friend class GLPipeline;

        explicit GLProgram(GLuint id);

    public:
        GLProgram();
        GLProgram(GLProgram&&) noexcept;
        GLProgram& operator=(GLProgram&&) noexcept;
        ~GLProgram() override;

        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] GLint GetParam(GLenum pname) const;

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetProgramiv(ID, pname, params.data());
            return params;
        }

        [[nodiscard]] GLint GetStageParam(GLenum type, GLenum pname) const;

        template <unsigned N>
        std::array<GLint, N> GetStageParams(const GLenum type, const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetProgramStageiv(ID, type, pname, params.data());
            return params;
        }

        [[nodiscard]] std::string GetInfoLog() const;
        [[nodiscard]] GLint GetInterfaceParam(GLenum interface, GLenum pname) const;

        template <unsigned N>
        std::array<GLint, N> GetInterfaceParams(const GLenum interface, const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetProgramInterfaceiv(ID, interface, pname, params.data());
            return params;
        }

        [[nodiscard]] std::vector<GLint> GetResources(
            GLenum interface,
            GLuint index,
            const std::vector<GLenum>& props) const;

        [[nodiscard]] GLuint GetResourceIndex(GLenum interface, const std::string& name) const;
        [[nodiscard]] GLint GetResourceLocation(GLenum interface, const std::string& name) const;
        [[nodiscard]] GLint GetResourceLocationIndex(GLenum interface, const std::string& name) const;
        [[nodiscard]] std::string GetResourceName(GLenum interface, GLuint index) const;

        [[nodiscard]] GLBinary GetBinary() const;

        [[nodiscard]] std::vector<GLuint> GetAttached() const;

        [[nodiscard]] GLint GetUniformLocation(const std::string& name) const;
        [[nodiscard]] GLint GetSubroutineUniformLocation(GLenum type, const std::string& name) const;

        template <GLUniform U, typename... T>
        void SetUniform(const std::string& name, T... value) const
        {
            static const std::map<GLUniform, void*> PFNS
            {
                {UNIFORM_FLOAT_1, reinterpret_cast<void*>(glProgramUniform1f)},
                {UNIFORM_FLOAT_2, reinterpret_cast<void*>(glProgramUniform2f)},
                {UNIFORM_FLOAT_3, reinterpret_cast<void*>(glProgramUniform3f)},
                {UNIFORM_FLOAT_4, reinterpret_cast<void*>(glProgramUniform4f)},
                {UNIFORM_INT_1, reinterpret_cast<void*>(glProgramUniform1i)},
                {UNIFORM_INT_2, reinterpret_cast<void*>(glProgramUniform2i)},
                {UNIFORM_INT_3, reinterpret_cast<void*>(glProgramUniform3i)},
                {UNIFORM_INT_4, reinterpret_cast<void*>(glProgramUniform4i)},
                {UNIFORM_UINT_1, reinterpret_cast<void*>(glProgramUniform1ui)},
                {UNIFORM_UINT_2, reinterpret_cast<void*>(glProgramUniform2ui)},
                {UNIFORM_UINT_3, reinterpret_cast<void*>(glProgramUniform3ui)},
                {UNIFORM_UINT_4, reinterpret_cast<void*>(glProgramUniform4ui)},
            };
            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<void (*)(GLuint, GLint, T...)>(pfn);
            const auto location = GetUniformLocation(name);
            fn(ID, location, value...);
        }

        template <GLUniform U, typename T>
        void SetUniform(const std::string& name, GLsizei count, const T* value) const
        {
            static const std::map<GLUniform, void*> PFNS
            {
                {UNIFORM_FLOAT_1, reinterpret_cast<void*>(glProgramUniform1fv)},
                {UNIFORM_FLOAT_2, reinterpret_cast<void*>(glProgramUniform2fv)},
                {UNIFORM_FLOAT_3, reinterpret_cast<void*>(glProgramUniform3fv)},
                {UNIFORM_FLOAT_4, reinterpret_cast<void*>(glProgramUniform4fv)},
                {UNIFORM_INT_1, reinterpret_cast<void*>(glProgramUniform1iv)},
                {UNIFORM_INT_2, reinterpret_cast<void*>(glProgramUniform2iv)},
                {UNIFORM_INT_3, reinterpret_cast<void*>(glProgramUniform3iv)},
                {UNIFORM_INT_4, reinterpret_cast<void*>(glProgramUniform4iv)},
                {UNIFORM_UINT_1, reinterpret_cast<void*>(glProgramUniform1uiv)},
                {UNIFORM_UINT_2, reinterpret_cast<void*>(glProgramUniform2uiv)},
                {UNIFORM_UINT_3, reinterpret_cast<void*>(glProgramUniform3uiv)},
                {UNIFORM_UINT_4, reinterpret_cast<void*>(glProgramUniform4uiv)},
            };
            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<void (*)(GLuint, GLint, GLsizei, const T*)>(pfn);
            const auto location = GetUniformLocation(name);
            fn(ID, location, count, value);
        }

        template <GLUniform U, typename T>
        void SetUniformMatrix(const std::string& name, GLsizei count, GLboolean transpose, const T* value) const
        {
            static const std::map<GLUniform, void*> PFNS
            {
                {UNIFORM_MATRIX_2_2, reinterpret_cast<void*>(glProgramUniformMatrix2fv)},
                {UNIFORM_MATRIX_2_3, reinterpret_cast<void*>(glProgramUniformMatrix2x3fv)},
                {UNIFORM_MATRIX_2_4, reinterpret_cast<void*>(glProgramUniformMatrix2x4fv)},
                {UNIFORM_MATRIX_3_2, reinterpret_cast<void*>(glProgramUniformMatrix3x2fv)},
                {UNIFORM_MATRIX_3_3, reinterpret_cast<void*>(glProgramUniformMatrix3fv)},
                {UNIFORM_MATRIX_3_4, reinterpret_cast<void*>(glProgramUniformMatrix3x4fv)},
                {UNIFORM_MATRIX_4_2, reinterpret_cast<void*>(glProgramUniformMatrix4x2fv)},
                {UNIFORM_MATRIX_4_3, reinterpret_cast<void*>(glProgramUniformMatrix4x3fv)},
                {UNIFORM_MATRIX_4_4, reinterpret_cast<void*>(glProgramUniformMatrix4fv)},
            };
            const auto pfn = PFNS.at(U);
            const auto fn = reinterpret_cast<void (*)(GLuint, GLint, GLsizei, GLboolean, const T*)>(pfn);
            const auto location = GetUniformLocation(name);
            fn(ID, location, count, transpose, value);
        }

        void SetBinary(const GLBinary& binary) const;
        void SetParam(GLenum pname, GLint value) const;
        void Attach(const GLShader& shader) const;
        void Detach(const GLShader& shader) const;
        void ShaderStorageBlockBinding(GLuint index, GLuint binding) const;
        void Link() const;
        void Validate() const;
        void Use() const;

        static GLProgram Create();
    };

    class GLPipeline final : public GLObject
    {
        explicit GLPipeline(GLuint id);

    public:
        GLPipeline();
        GLPipeline(GLPipeline&&) noexcept;
        GLPipeline& operator=(GLPipeline&&) noexcept;
        ~GLPipeline() override;

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetProgramPipelineiv(ID, pname, params.data());
            return params;
        }

        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] GLint GetParam(GLenum pname) const;
        [[nodiscard]] std::string GetInfoLog() const;

        void ActiveShaderProgram(const GLProgram& program) const;
        void UseProgramStages(GLbitfield stages, const GLProgram& program) const;
        void Validate() const;
        void Bind() const;

        static GLPipeline Create();
    };

    class GLVertexArray final : public GLObject
    {
        explicit GLVertexArray(GLuint id);

    public:
        GLVertexArray();
        GLVertexArray(GLVertexArray&&) noexcept;
        GLVertexArray& operator=(GLVertexArray&&) noexcept;
        ~GLVertexArray() override;

        void Bind() const;
        void Stride(GLsizei stride);

        template <typename T>
        void VertexAttrib(const GLint size, const GLenum type, const GLboolean normalized)
        {
            glEnableVertexArrayAttrib(ID, m_Index);
            glVertexAttribPointer(m_Index, size, type, normalized, m_Stride, reinterpret_cast<const void*>(m_Offset));
            ++m_Index;
            m_Offset += size * sizeof(T);
        }

        static GLVertexArray Create();

    private:
        GLsizei m_Stride{};
        GLuint m_Index{};
        unsigned m_Offset{};
    };

    class GLBuffer final : public GLObject
    {
        explicit GLBuffer(GLuint id);

    public:
        GLBuffer();
        GLBuffer(GLBuffer&&) noexcept;
        GLBuffer& operator=(GLBuffer&&) noexcept;
        ~GLBuffer() override;

        void Bind(GLenum target);

        template <typename T>
        void Data(const std::vector<T>& data, const GLenum usage) const
        {
            glBufferData(m_Target, data.size() * sizeof(T), data.data(), usage);
        }

        template <typename T>
        void Data(const unsigned n, const GLenum usage) const
        {
            glBufferData(m_Target, n * sizeof(T), nullptr, usage);
        }

        template <typename T>
        void SubData(const unsigned offset, const std::vector<T>& data) const
        {
            glBufferSubData(m_Target, offset, data.size() * sizeof(T), data.data());
        }

        static GLBuffer Create();

    private:
        GLenum m_Target{};
    };

    class GLTexture final : public GLObject
    {
        explicit GLTexture(GLuint id);

    public:
        GLTexture();
        GLTexture(GLTexture&&) noexcept;
        GLTexture& operator=(GLTexture&&) noexcept;
        ~GLTexture() override;

        [[nodiscard]] GLint LevelParam(GLint level, GLenum pname) const;

        void Bind(GLenum target);
        void Param(GLenum pname, GLint param) const;
        void Image2D(
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

    private:
        GLenum m_Target{};
    };
}
