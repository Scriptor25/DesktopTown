#pragma once

#include <DesktopTown/GL/GLProgram.hpp>

namespace DesktopTown
{
    struct Shader
    {
        GLenum Stage;
        std::string Filename;
    };

    class Material
    {
    public:
        explicit Material(const std::vector<Shader>& shaders);
        explicit Material(GLProgram&& program);

    private:
        GLProgram m_Program;
    };
}
