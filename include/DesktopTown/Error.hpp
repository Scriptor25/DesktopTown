#pragma once

#include <format>
#include <string>
#include <GL/glew.h>

namespace DesktopTown
{
    template <typename... Args>
    void Error(const GLenum type, const GLuint id, const GLenum severity, const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, type, id, severity, -1, message.c_str());
    }
}
