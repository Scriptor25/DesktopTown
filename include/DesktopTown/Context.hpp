#pragma once

#include <DesktopTown/DesktopTown.hpp>

namespace DesktopTown
{
    class Context;

    struct ContextInfo
    {
        void (*OnStart)(Context& ctx);
        void (*OnUpdate)(Context& ctx);
        void (*OnStop)(Context& ctx);
    };

    class Context
    {
    public:
        explicit Context(ContextInfo&& info);
        ~Context();

    private:
        void Start();
        void Update();
        void Stop();

        ContextInfo m_Info;
        GLFWwindow* m_Window;
    };
}
