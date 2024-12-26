#include <DesktopTown/DesktopTown.hpp>

#ifdef _WIN32 || __WIN32 || __WIN32__

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void DesktopTown::glfwHideTaskbarIcon(GLFWwindow* window)
{
    const auto hwnd = glfwGetWin32Window(window);

    auto style = GetWindowLong(hwnd, GWL_EXSTYLE);
    style &= ~WS_EX_APPWINDOW;
    style |= WS_EX_TOOLWINDOW;

    SetWindowLong(hwnd, GWL_EXSTYLE, style);
}

#else

void DesktopTown::glfwHideTaskbarIcon(GLFWwindow* window)
{
}

#endif
