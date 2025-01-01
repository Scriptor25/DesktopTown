#pragma once

#include <DesktopTown/DesktopTown.hpp>

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
