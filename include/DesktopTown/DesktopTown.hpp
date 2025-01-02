#pragma once

#define GLFW_INCLUDE_NONE

#include <format>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

namespace DesktopTown
{
    class Context;
    class FontContext;

    void glfwHideTaskbarIcon(GLFWwindow* window);

    template <typename... Args>
    void Error(const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(args...));
        std::cerr << "[Error] " << message << std::endl;
    }
}
