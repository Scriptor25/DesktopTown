#pragma once

#include <format>
#include <string_view>
#include <GL/glew.h>

namespace DesktopTown
{
    template<typename... Args>
    void Message(
        const GLenum type,
        const GLuint id,
        const GLenum severity,
        const std::string_view &format,
        Args &&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, type, id, severity, -1, message.c_str());
    }

    template<typename... Args>
    void Error(const std::string_view &format, Args &&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(
            GL_DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE_ERROR,
            0,
            GL_DEBUG_SEVERITY_HIGH,
            -1,
            message.c_str());
    }

    template<typename... Args>
    void Warning(const std::string_view &format, Args &&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(
            GL_DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE_ERROR,
            0,
            GL_DEBUG_SEVERITY_MEDIUM,
            -1,
            message.c_str());
    }

    template<typename... Args>
    void Verbose(const std::string_view &format, Args &&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(
            GL_DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE_ERROR,
            0,
            GL_DEBUG_SEVERITY_LOW,
            -1,
            message.c_str());
    }

    template<typename... Args>
    void Info(const std::string_view &format, Args &&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(
            GL_DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE_ERROR,
            0,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            -1,
            message.c_str());
    }
}
