#pragma once

#define GLFW_INCLUDE_NONE

#include <unordered_map>
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace DesktopTown
{
    struct ButtonState
    {
        bool Press() const;
        bool Release() const;
        bool Repeat() const;

        void Update(bool next);

        bool Previous, Current;
    };

    class Context
    {
    public:
        Context();
        virtual ~Context();

        void Start();
        void Stop() const;

        [[nodiscard]] GLFWwindow *GetWindow() const;
        void GetSize(int &width, int &height) const;
        void GetCursor(float &xpos, float &ypos) const;

        bool IsMouseButtonDown(int button) const;
        bool IsMouseButtonUp(int button) const;
        bool IsMouseButton(int button) const;
        bool IsKeyDown(int key) const;
        bool IsKeyUp(int key) const;
        int GetKey(int key) const;

        void AddFocus(void *handle);
        void RemoveFocus(void *handle);

        void Invalidate();
        const glm::mat4 &GetProjection();

    protected:
        virtual void OnStart();
        virtual void OnUpdate();
        virtual void OnStop();

    private:
        GLFWwindow *m_Window;

        bool m_Dirty = true;
        glm::mat4 m_Projection;

        std::unordered_set<void *> m_WantFocus;

        std::unordered_map<int, ButtonState> m_MouseStateMap;
        std::unordered_map<int, ButtonState> m_KeyStateMap;
    };
}
