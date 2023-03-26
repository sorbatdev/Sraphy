
#include "window_manager.h"

#include "events/events.h"
#include "utils.h"
#include "char_array.h"

namespace sraphy
{
    namespace
    {
        static void GLFWErrorCallback(int p_ErrorCode, const char* p_Description)
        {
            switch (p_ErrorCode)
            {
            case GLFW_INVALID_ENUM:
                LOG_WARN("GLFW received an invalid enum to it's function! Desc: {0}", p_Description);
                break;
            case GLFW_INVALID_VALUE:
                LOG_WARN("GLFW received an invalid value to it's function! Desc: {0}", p_Description);
                break;
            case GLFW_OUT_OF_MEMORY:
                LOG_ERROR("A memory allocation failed within GLFW or the operating system! Desc: {0}", p_Description);
                break;
            case GLFW_API_UNAVAILABLE:
                LOG_ERROR("GLFW could not find support for the requested API on the system! Desc: {0}", p_Description);
                break;
            case GLFW_FORMAT_UNAVAILABLE:
                LOG_ERROR("The requested pixel format is not supported! Desc: {0}", p_Description);
                break;
            }
        }

        static void WindowCloseCallback(GLFWwindow* p_Window)
        {
            // Raise the event with the window handle so we know which window
            // to close.
            EventBus::RaiseEvent<window_close>(p_Window);

            // Main window closing needs to happen at the end of the app. Not
            // by cakking destroywindow but glfwTerminate.
            if (p_Window != WindowManager::AppWindowHandle())
                WindowManager::CloseWindow(p_Window);
        }

        static void WindowFocusCallback(GLFWwindow* p_Window, int32_t p_Focused)
        {
            if (p_Window != glfwGetCurrentContext())
            {
                glfwMakeContextCurrent(p_Window);
            }
            EventBus::RaiseEvent<window_focus>(p_Window, p_Focused);
        }

        static void WindowSizeCallback(GLFWwindow* /*p_Window*/, int32_t p_Width, int32_t p_Height)
        {
            EventBus::RaiseEvent<window_resize>(p_Width, p_Height);
        }

        static void WindowIconifyCallback(GLFWwindow* /*p_Window*/, int32_t p_Iconified)
        {
            EventBus::RaiseEvent<window_iconify>(p_Iconified);
        }

        static void WindowMaximizeCallback(GLFWwindow* /*p_Window*/, int32_t p_Maximized)
        {
            EventBus::RaiseEvent<window_maximize>(p_Maximized);
        }

        static void FramebufferSizeCallback(GLFWwindow* /*p_Window*/, int32_t p_Width, int32_t p_Height)
        {
            EventBus::RaiseEvent<window_framebuffer_size>();
            glViewport(0, 0, p_Width, p_Height);
        }

        static void KeyCallback(
            GLFWwindow* /*p_Window*/,
            int32_t p_Key,
            int32_t /*p_Scancode*/,
            int32_t p_Action,
            int32_t /*p_Mods*/
        )
        {
            switch (p_Action)
            {
            case GLFW_PRESS:
                EventBus::RaiseEvent<key_press>(p_Key);
                break;
            case GLFW_RELEASE:
                EventBus::RaiseEvent<key_release>(p_Key);
                break;
            }
        }

        static void CharCallback(GLFWwindow* /*p_Window*/, uint32_t p_Unicode)
        {
            EventBus::RaiseEvent<char_type>(p_Unicode);
        }

        static void MouseButtonCallback(
            GLFWwindow* /*p_Window*/,
            int32_t p_Button,
            int32_t p_Action,
            int32_t /*p_Mods*/
        )
        {
            switch (p_Action)
            {
            case GLFW_PRESS:
                EventBus::RaiseEvent<mouse_button_press>(p_Button);
                break;
            case GLFW_RELEASE:
                EventBus::RaiseEvent<mouse_button_release>(p_Button);
                break;
            }
        }

        static void ScrollCallback(
            GLFWwindow* /*p_Window*/,
            double p_XOffset,
            double p_YOffset)
        {
            EventBus::RaiseEvent<mouse_scroll>(static_cast<float>(p_XOffset), static_cast<float>(p_YOffset));
        }

        static void CursorPosCallback(GLFWwindow* /*p_Window*/, double p_XOffset, double p_YOffset)
        {
            EventBus::RaiseEvent<mouse_move>(static_cast<float>(p_XOffset), static_cast<float>(p_YOffset));
        }
    }

    WindowManager* WindowManager::s_Instance;

    WindowManager::WindowManager()
    {
        SUB_TO_EVENT_MEM_FUN(app_late_update, update);
        SUB_TO_EVENT_MEM_FUN(window_focus, on_focus);

        if (!glfwInit())
        {
            LOG_ERROR("GLFW could not be initialized.");
        }

        s_Instance = this;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        GLFWvidmode const* mode = glfwGetVideoMode(monitor);
        
        using namespace std::string_view_literals;
        window_properties win_props{
            .order = 0, // the first window, so it's order is 0
            .width = mode->width,
            .height = mode->height,
            .title = "Sraphy"sv,
            .decorated = true,
            .fullscreen = false,
            .maximized = true,
            .vsync = true,
            .refreshRate = mode->refreshRate
        };
        NewWindow(win_props);
    }

    WindowManager::~WindowManager()
    {
        glfwTerminate();
        s_Instance = nullptr;
    }

    int64_t WindowManager::NewWindow(window_properties& p_Props)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_DECORATED, p_Props.decorated);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        GLFWvidmode const* mode = glfwGetVideoMode(monitor);

        p_Props.order = s_Instance->m_WindowGuids.size();

        int64_t guid = CreateGuid();
        s_Instance->m_WindowGuids.push_back(guid);
        s_Instance->m_WindowProps.push_back(p_Props);

        auto* win_handle = glfwCreateWindow(p_Props.width, p_Props.height, p_Props.title.c_str(), nullptr, nullptr);
        s_Instance->m_WindowHandles.push_back(win_handle);

        if(p_Props.maximized)
            glfwMaximizeWindow(win_handle);

        glfwSetWindowUserPointer(win_handle, &p_Props);
        int32_t pos_x, pos_y;
        glfwGetWindowPos(win_handle, &pos_x, &pos_y);

        if (p_Props.fullscreen)
        {
            int32_t refresh_rate = p_Props.vsync ? mode->refreshRate : p_Props.refreshRate;

            glfwSetWindowMonitor(win_handle, monitor, pos_x, pos_y, mode->width, mode->height, refresh_rate);
        }
        else
        {
            int32_t window_width, window_height;
            glfwGetWindowSize(win_handle, &window_width, &window_height);
            p_Props.width = window_width;
            p_Props.height = window_height;
        }

        glfwSwapInterval(p_Props.vsync);

        glfwSetWindowCloseCallback(win_handle, WindowCloseCallback);
        glfwSetWindowFocusCallback(win_handle, WindowFocusCallback);
        glfwSetWindowSizeCallback(win_handle, WindowSizeCallback);
        glfwSetWindowIconifyCallback(win_handle, WindowIconifyCallback);
        glfwSetWindowMaximizeCallback(win_handle, WindowMaximizeCallback);
        glfwSetFramebufferSizeCallback(win_handle, FramebufferSizeCallback);

        glfwSetKeyCallback(win_handle, KeyCallback);
        glfwSetCharCallback(win_handle, CharCallback);

        glfwSetMouseButtonCallback(win_handle, MouseButtonCallback);
        glfwSetScrollCallback(win_handle, ScrollCallback);
        glfwSetCursorPosCallback(win_handle, CursorPosCallback);

        return guid;
    }

    void WindowManager::CloseWindow(int64_t p_Guid)
    {
        auto index = find_index_with_guid(p_Guid);

        if (index == s_Instance->m_WindowGuids.size())
        {
            LOG_ERROR("[WindowManager::CloseWindow] Could not find a window associated with the guid: {}", p_Guid);
            return;
        }

        glfwDestroyWindow(s_Instance->m_WindowHandles[index]);

        auto prop   = s_Instance->m_WindowProps[index];
        auto* handle = s_Instance->m_WindowHandles[index];

        std::erase(s_Instance->m_WindowGuids, p_Guid);
        std::erase(s_Instance->m_WindowHandles, handle);
        std::erase(s_Instance->m_WindowProps, prop);
    }

    void WindowManager::CloseWindow(GLFWwindow* p_WinHandle)
    {
        auto index = find_index_with_handle(p_WinHandle);

        if (index == s_Instance->m_WindowHandles.size())
        {
            LOG_ERROR("[WindowManager::CloseWindow] Could not find a window associated with the given handle!");
            return;
        }

        glfwDestroyWindow(p_WinHandle);

        auto guid = s_Instance->m_WindowGuids[index];
        auto prop = s_Instance->m_WindowProps[index];

        std::erase(s_Instance->m_WindowGuids, guid);
        std::erase(s_Instance->m_WindowHandles, p_WinHandle);
        std::erase(s_Instance->m_WindowProps, prop);
    }

    int64_t WindowManager::AppWindowGuid()
    {
        return s_Instance->m_WindowGuids[0];
    }

    GLFWwindow* WindowManager::AppWindowHandle()
    {
        return s_Instance->m_WindowHandles[0];
    }

    bool WindowManager::AppWindowShouldClose()
    {
        return glfwWindowShouldClose(s_Instance->m_WindowHandles[0]);
    }

    void WindowManager::AppWindowRequestFocus()
    {
        s_Instance->m_ActiveWindowIndex = 0;
        glfwMakeContextCurrent(s_Instance->m_WindowHandles[0]);
    }

    window_properties WindowManager::GetWindowProperties(int64_t p_Guid)
    {
        auto index = find_index_with_guid(p_Guid);

        if (index == s_Instance->m_WindowGuids.size())
        {
            LOG_ERROR("[WindowManager::GetWindowShouldClose] Could not find a window associated with the guid: {}", p_Guid);
            return {};
        }

        return s_Instance->m_WindowProps[index];
    }

    GLFWwindow* WindowManager::GetWindowHandle(int64_t p_Guid)
    {
        auto index = find_index_with_guid(p_Guid);

        if (index == s_Instance->m_WindowGuids.size())
        {
            LOG_ERROR("[WindowManager::GetWindowShouldClose] Could not find a window associated with the guid: {}", p_Guid);
            return nullptr;
        }

        return s_Instance->m_WindowHandles[index];
    }

    int64_t WindowManager::GetWindowGuid(GLFWwindow* p_Handle)
    {
        auto index = find_index_with_handle(p_Handle);

        if (index == s_Instance->m_WindowHandles.size())
        {
            LOG_ERROR("[WindowManager::on_focus] The Window Manager does not own the window to focus on it.");
            return INT_MIN;
        }

        return s_Instance->m_WindowGuids[index];
    }

    GLFWwindow* WindowManager::GetFocusedWindow()
    {
        return s_Instance->m_WindowHandles[s_Instance->m_ActiveWindowIndex];
    }

    void WindowManager::RequestFocus(int64_t p_Guid)
    {
        auto index = find_index_with_guid(p_Guid);

        if (index == s_Instance->m_WindowGuids.size())
        {
            LOG_ERROR("[WindowManager::GetWindowShouldClose] Could not find a window associated with the guid: {}", p_Guid);
            return;
        }

        s_Instance->m_ActiveWindowIndex = index;
        glfwMakeContextCurrent(s_Instance->m_WindowHandles[index]);
    }

    bool WindowManager::GetWindowShouldClose(int64_t p_Guid)
    {
        auto index = find_index_with_guid(p_Guid);

        if (index == s_Instance->m_WindowGuids.size())
        {
            LOG_ERROR("[WindowManager::GetWindowShouldClose] Could not find a window associated with the guid: {}", p_Guid);
            return true;
        }

        return glfwWindowShouldClose(s_Instance->m_WindowHandles[index]);
    }

    void WindowManager::update(app_late_update& e)
    {
        glfwPollEvents();
        glfwSwapBuffers(m_WindowHandles[m_ActiveWindowIndex]);
    }

    void WindowManager::on_focus(window_focus& e)
    {
        auto index = find_index_with_handle(static_cast<GLFWwindow*>(e.winHandle));

        if (index == m_WindowHandles.size())
        {
            LOG_ERROR("[WindowManager::on_focus] The Window Manager does not own the window to focus on it.");
            return;
        }

        m_ActiveWindowIndex = index;
    }

    std::size_t WindowManager::find_index_with_guid(int64_t p_Guid)
    {
        std::size_t i{};
        for (; i < s_Instance->m_WindowGuids.size(); ++i)
        {
            if (s_Instance->m_WindowGuids[i] == p_Guid)
            {
                break;
            }
        }

        return i;
    }

    std::size_t WindowManager::find_index_with_handle(GLFWwindow* p_Window)
    {
        std::size_t i{};
        for (; i < s_Instance->m_WindowHandles.size(); ++i)
        {
            if (s_Instance->m_WindowHandles[i] == p_Window)
            {
                break;
            }
        }

        return i;
    }

} // namespace sraphy
