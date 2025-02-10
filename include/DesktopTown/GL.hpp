#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace DesktopTown
{
    class GLDefer
    {
    public:
        explicit GLDefer(std::function<void()> &&defer);
        ~GLDefer();

        GLDefer(const GLDefer &) = delete;
        GLDefer &operator=(const GLDefer &) = delete;
        GLDefer(GLDefer &&other) noexcept;
        GLDefer &operator=(GLDefer &&other) noexcept;

    private:
        std::function<void()> m_Defer;
    };

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

    class GLBuffer;
    class GLFramebuffer;
    class GLObject;
    class GLPipeline;
    class GLProgram;
    class GLQuery;
    class GLRenderbuffer;
    class GLShader;
    class GLTexture;
    class GLVertexArray;
}
