#include <string>
#include <DesktopTown/Context.hpp>
#include <GL/glew.h>

#include "DesktopTown/GL.hpp"

static void on_error(const int error_code, const char* description)
{
    fprintf(stderr, "[GLFW 0x%08X] %s\r\n", error_code, description);
}

static void APIENTRY on_debug(
    const GLenum /*source*/,
    const GLenum /*type*/,
    const GLuint id,
    const GLenum /*severity*/,
    const GLsizei length,
    const GLchar* message,
    const void* /*user_param*/)
{
    fprintf(stderr, "[OpenGL 0x%08X] %.*s\r\n", id, length, message);
}

DesktopTown::Context::Context(ContextInfo&& info)
    : m_Info(info)
{
    glfwSetErrorCallback(on_error);
    glfwInit();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_Window = glfwCreateWindow(1, 1, "Desktop Town", nullptr, nullptr);
    glfwSetWindowUserPointer(m_Window, this);

    {
        int xpos, ypos, width, height;
        const auto monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);
        glfwSetWindowMonitor(m_Window, nullptr, xpos, ypos, width, height, GLFW_DONT_CARE);
    }

    glfwHideTaskbarIcon(m_Window);

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    glewInit();

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(on_debug, this);

    glClearColor(0.1f, 0.02f, 0.0f, 0.0f);

    Start();

    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();

        Update();

        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render NPCs, buildings, etc.
        // render text, info boxes, buttons, etc.
        // ...

        glfwSwapBuffers(m_Window);
    }

    Stop();
}

DesktopTown::Context::~Context()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void DesktopTown::Context::Start()
{
    // load shaders, models, textures, etc.
    // load save file
    // ...
    m_Info.OnStart(*this);
}

void DesktopTown::Context::Update()
{
    // update NPCs, animations, etc.
    // ...
    m_Info.OnUpdate(*this);
}

void DesktopTown::Context::Stop()
{
    m_Info.OnStop(*this);
}
