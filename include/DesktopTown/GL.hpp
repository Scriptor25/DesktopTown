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

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname) const
        {
            GLint params[N];
            glGetShaderiv(ID, pname, params);
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

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname) const
        {
            GLint params[N];
            glGetProgramiv(ID, pname, params);
            return params;
        }

        template <unsigned N>
        std::array<GLint, N> GetStageParams(const GLenum type, const GLenum pname) const
        {
            GLint params[N];
            glGetProgramStageiv(ID, type, pname, params);
            return params;
        }

        [[nodiscard]] std::string GetInfoLog() const;

        template <unsigned N>
        std::array<GLint, N> GetInterfaceParams(const GLenum interface, const GLenum pname) const
        {
            GLint params[N];
            glGetProgramInterfaceiv(ID, interface, pname, params);
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
                {UNIFORM_FLOAT_1, (void*)glProgramUniform1f},
                {UNIFORM_FLOAT_2, (void*)glProgramUniform2f},
                {UNIFORM_FLOAT_3, (void*)glProgramUniform3f},
                {UNIFORM_FLOAT_4, (void*)glProgramUniform4f},
                {UNIFORM_INT_1, (void*)glProgramUniform1i},
                {UNIFORM_INT_2, (void*)glProgramUniform2i},
                {UNIFORM_INT_3, (void*)glProgramUniform3i},
                {UNIFORM_INT_4, (void*)glProgramUniform4i},
                {UNIFORM_UINT_1, (void*)glProgramUniform1ui},
                {UNIFORM_UINT_2, (void*)glProgramUniform2ui},
                {UNIFORM_UINT_3, (void*)glProgramUniform3ui},
                {UNIFORM_UINT_4, (void*)glProgramUniform4ui},
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
                {UNIFORM_FLOAT_1, (void*)glProgramUniform1fv},
                {UNIFORM_FLOAT_2, (void*)glProgramUniform2fv},
                {UNIFORM_FLOAT_3, (void*)glProgramUniform3fv},
                {UNIFORM_FLOAT_4, (void*)glProgramUniform4fv},
                {UNIFORM_INT_1, (void*)glProgramUniform1iv},
                {UNIFORM_INT_2, (void*)glProgramUniform2iv},
                {UNIFORM_INT_3, (void*)glProgramUniform3iv},
                {UNIFORM_INT_4, (void*)glProgramUniform4iv},
                {UNIFORM_UINT_1, (void*)glProgramUniform1uiv},
                {UNIFORM_UINT_2, (void*)glProgramUniform2uiv},
                {UNIFORM_UINT_3, (void*)glProgramUniform3uiv},
                {UNIFORM_UINT_4, (void*)glProgramUniform4uiv},
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
                {UNIFORM_MATRIX_2_2, (void*)glProgramUniformMatrix2fv},
                {UNIFORM_MATRIX_2_3, (void*)glProgramUniformMatrix2x3fv},
                {UNIFORM_MATRIX_2_4, (void*)glProgramUniformMatrix2x4fv},
                {UNIFORM_MATRIX_3_2, (void*)glProgramUniformMatrix3x2fv},
                {UNIFORM_MATRIX_3_3, (void*)glProgramUniformMatrix3fv},
                {UNIFORM_MATRIX_3_4, (void*)glProgramUniformMatrix3x4fv},
                {UNIFORM_MATRIX_4_2, (void*)glProgramUniformMatrix4x2fv},
                {UNIFORM_MATRIX_4_3, (void*)glProgramUniformMatrix4x3fv},
                {UNIFORM_MATRIX_4_4, (void*)glProgramUniformMatrix4fv},
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

        template <unsigned N>
        std::array<GLint, N> GetParams(const GLenum pname, const unsigned index = 0u) const
        {
            GLint params[N];
            glGetProgramPipelineiv(IDs.at(index), pname, params);
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
}
