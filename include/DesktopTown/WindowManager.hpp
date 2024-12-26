#pragma once

#include <vector>
#include <DesktopTown/DesktopTown.hpp>

namespace DesktopTown
{
    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();

    private:
        GLFWwindow* m_Window;
    };
}
