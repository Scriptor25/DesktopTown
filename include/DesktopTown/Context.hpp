#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

namespace DesktopTown
{
    class Context
    {
    public:
        Context();
        virtual ~Context();

        void Start();
        void Stop() const;

        [[nodiscard]] GLFWwindow* GetWindow() const;
        void GetSize(int& width, int& height) const;

    protected:
        virtual void OnStart();
        virtual void OnUpdate();
        virtual void OnStop();

    private:
        GLFWwindow* m_Window;
    };
}
