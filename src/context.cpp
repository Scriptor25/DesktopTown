#include <iostream>
#include <string>
#include <DesktopTown/Context.hpp>
#include <DesktopTown/Error.hpp>
#include <DesktopTown/GL.hpp>
#include <DesktopTown/WindowUtil.hpp>
#include <glm/ext.hpp>

static void on_error(const int error_code, const char *description)
{
    fprintf(stderr, "[GLFW 0x%08X] %s\r\n", error_code, description);
}

static void APIENTRY on_debug(
    const GLenum /*source*/,
    const GLenum /*type*/,
    const GLuint id,
    const GLenum severity,
    const GLsizei length,
    const GLchar *message,
    const void * /*user_param*/)
{
    const char *level;
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            level = "ERROR  ";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            level = "WARNING";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            level = "VERBOSE";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            level = "INFO   ";
            break;
        default:
            level = "UNKNOWN";
            break;
    }

    fprintf(stderr, "[OpenGL %s 0x%08X] %.*s\r\n", level, id, length, message);
    fflush(stderr);

    if (severity == GL_DEBUG_SEVERITY_HIGH)
        __debugbreak();
}

static void on_framebuffer_size(GLFWwindow *window, int, int)
{
    static_cast<DesktopTown::Context *>(glfwGetWindowUserPointer(window))->Invalidate();
}

bool DesktopTown::ButtonState::Press() const
{
    return !Previous && Current;
}

bool DesktopTown::ButtonState::Release() const
{
    return Previous && !Current;
}

bool DesktopTown::ButtonState::Repeat() const
{
    return Previous && Current;
}

void DesktopTown::ButtonState::Update(const bool next)
{
    Previous = Current;
    Current = next;
}

DesktopTown::Context::Context()
{
    glfwSetErrorCallback(on_error);
    glfwInit();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

    glfwSetFramebufferSizeCallback(m_Window, on_framebuffer_size);

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(on_debug, this);

    // glClearColor(0.1f, 0.02f, 0.0f, 0.0f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    glfwHideWindow(m_Window);
    glfwShowWindow(m_Window);
}

DesktopTown::Context::~Context()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void DesktopTown::Context::Start()
{
    OnStart();

    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();

        int width, height;
        GetSize(width, height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        OnUpdate();

        glfwSwapBuffers(m_Window);

        if (m_WantFocus.empty())
        {
            glfwSetWindowAttrib(m_Window, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        }
        else
        {
            glfwSetWindowAttrib(m_Window, GLFW_MOUSE_PASSTHROUGH, GLFW_FALSE);
            glfwFocusWindow(m_Window);
        }

        for (auto i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; ++i)
            m_KeyStateMap[i].Update(glfwGetKey(m_Window, i) == GLFW_PRESS);
        for (auto i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST; ++i)
            m_MouseStateMap[i].Update(glfwGetMouseButton(m_Window, i) == GLFW_PRESS);
    }

    OnStop();
}

void DesktopTown::Context::Stop() const
{
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

GLFWwindow *DesktopTown::Context::GetWindow() const
{
    return m_Window;
}

void DesktopTown::Context::GetSize(int &width, int &height) const
{
    glfwGetFramebufferSize(m_Window, &width, &height);
}

void DesktopTown::Context::GetCursor(float &xpos, float &ypos) const
{
    int width, height;
    GetSize(width, height);

    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    xpos = static_cast<float>(x);
    ypos = static_cast<float>(height - y - 1);
}

bool DesktopTown::Context::IsMouseButtonDown(const int button) const
{
    return m_MouseStateMap.at(button).Press();
}

bool DesktopTown::Context::IsMouseButtonUp(const int button) const
{
    return m_MouseStateMap.at(button).Release();
}

bool DesktopTown::Context::IsMouseButton(const int button) const
{
    return m_MouseStateMap.at(button).Repeat();
}

bool DesktopTown::Context::IsKeyDown(const int key) const
{
    return m_KeyStateMap.at(key).Press();
}

bool DesktopTown::Context::IsKeyUp(const int key) const
{
    return m_KeyStateMap.at(key).Release();
}

int DesktopTown::Context::GetKey(const int key) const
{
    return m_KeyStateMap.at(key).Repeat();
}

void DesktopTown::Context::AddFocus(void *handle)
{
    m_WantFocus.emplace(handle);
}

void DesktopTown::Context::RemoveFocus(void *handle)
{
    m_WantFocus.erase(handle);
}

void DesktopTown::Context::Invalidate()
{
    m_Dirty = true;
}

const glm::mat4 &DesktopTown::Context::GetProjection()
{
    if (m_Dirty)
    {
        m_Dirty = false;

        int width, height;
        GetSize(width, height);
        const auto float_width = static_cast<float>(width);
        const auto float_height = static_cast<float>(height);

        m_Projection = glm::mat4(1.f);
        m_Projection = glm::ortho(0.f, float_width, 0.f, float_height);
    }

    return m_Projection;
}

void DesktopTown::Context::OnStart()
{
}

void DesktopTown::Context::OnUpdate()
{
}

void DesktopTown::Context::OnStop()
{
}
