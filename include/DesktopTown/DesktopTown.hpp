#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace DesktopTown
{
    class Context;
    class FontContext;

    void glfwHideTaskbarIcon(GLFWwindow* window);
}
