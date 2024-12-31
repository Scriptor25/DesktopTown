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

    class GLShader
    {
        friend class GLProgram;

    public:
        explicit GLShader(GLenum type);
        ~GLShader();

        [[nodiscard]] bool IsValid() const;

        GLint GetParam(GLenum pname) const;

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

    private:
        GLuint ID;
    };

    class GLProgram
    {
        friend class GLPipelines;

    public:
        GLProgram();
        ~GLProgram();

        [[nodiscard]] bool IsValid() const;

        GLint GetParam(GLenum pname) const;

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetProgramiv(ID, pname, params.data());
            return params;
        }

        GLint GetStageParam(GLenum type, GLenum pname) const;

        template <unsigned N>
        std::array<GLint, N> GetStageParams(const GLenum type, const GLenum pname) const
        {
            std::array<GLint, N> params;
            glGetProgramStageiv(ID, type, pname, params.data());
            return params;
        }

        [[nodiscard]] std::string GetInfoLog() const;

        GLint GetInterfaceParam(GLenum interface, GLenum pname) const;

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

    private:
        GLuint ID;
    };

    class GLPipelines
    {
    public:
        explicit GLPipelines(GLsizei n = 1);
        ~GLPipelines();

        [[nodiscard]] bool IsValid(unsigned index = 0u) const;
        GLint GetParam(GLenum pname, unsigned index = 0u) const;

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname, const unsigned index = 0u) const
        {
            std::array<GLint, N> params;
            glGetProgramPipelineiv(IDs.at(index), pname, params.data());
            return params;
        }

        [[nodiscard]] std::string GetInfoLog(unsigned index = 0u) const;

        void ActiveShaderProgram(const GLProgram& program, unsigned index = 0u) const;
        void UseProgramStages(GLbitfield stages, const GLProgram& program, unsigned index = 0u) const;
        void Validate(unsigned index = 0u) const;
        void Bind(unsigned index = 0u) const;

    private:
        std::vector<GLuint> IDs;
    };

    class GLVertexArrays
    {
    public:
        static void EnableVertexAttrib(GLuint index);
        static void DisableVertexAttrib(GLuint index);
        static void VertexAttribPointer(
            GLuint index,
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
            const void* pointer);
        static void VertexAttribPointer(
            GLuint index,
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
            unsigned offset);

        explicit GLVertexArrays(GLsizei n = 1);
        ~GLVertexArrays();

        void Bind(unsigned index = 0u) const;

    private:
        std::vector<GLuint> IDs;
    };

    class GLBuffers
    {
    public:
        template <typename T>
        static void Data(const GLenum target, const std::vector<T>& data, const GLenum usage)
        {
            glBufferData(target, data.size() * sizeof(T), data.data(), usage);
        }

        template <typename T>
        static void Data(const GLenum target, const unsigned n, const GLenum usage)
        {
            glBufferData(target, n * sizeof(T), nullptr, usage);
        }

        explicit GLBuffers(GLsizei n = 1);
        ~GLBuffers();

        void Bind(GLenum target, unsigned index = 0u) const;

    private:
        std::vector<GLuint> IDs;
    };

    class GLTextures
    {
    public:
        explicit GLTextures(GLsizei n = 1);
        ~GLTextures();

        void Bind(GLenum target, GLuint index = 0u) const;

    private:
        std::vector<GLuint> IDs;
    };
}
