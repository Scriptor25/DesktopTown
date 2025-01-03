#pragma once

#define GLFW_INCLUDE_NONE

#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace DesktopTown
{
    class Context;
    class FontContext;

    void glfwHideTaskbarIcon(GLFWwindow* window);

    template <typename... Args>
    void Error(const GLenum type, const GLuint id, const GLenum severity, const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, type, id, severity, -1, message.c_str());
    }

    std::vector<char> ReadFile(
        const std::string& filename,
        std::ios::openmode mode = {});
    void WriteFile(
        const std::string& filename,
        const std::vector<char>& data,
        std::ios::openmode mode = {});

    std::string ReadFileText(
        const std::string& filename,
        std::ios::openmode mode = {});
}
