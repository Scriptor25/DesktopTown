#pragma once

#include <DesktopTown/GL.hpp>
#include <DesktopTown/GL/GLObject.hpp>

namespace DesktopTown
{
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

        GLDefer Bind() const;
        void UseStages(GLbitfield stages, const GLProgram& program) const;
        void Validate() const;

        static GLPipeline Create();
    };
}
