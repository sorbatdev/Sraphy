#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window_properties.h"
#include "events/app/app_late_update.h"
#include "events/window/window_focus.h"

namespace sraphy
{
    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();
    public:
        static int64_t NewWindow(window_properties& p_Props);
        static void    CloseWindow(int64_t p_Guid);
        static void    CloseWindow(GLFWwindow* p_WinHandle);

        static int64_t     AppWindowGuid();
        static GLFWwindow* AppWindowHandle();
        static bool        AppWindowShouldClose();
        static void        AppWindowRequestFocus();

        static window_properties GetWindowProperties(int64_t p_Guid);
        static GLFWwindow*       GetWindowHandle(int64_t p_Guid);
        static int64_t           GetWindowGuid(GLFWwindow* p_Handle);

        static GLFWwindow* GetFocusedWindow();

        static void RequestFocus(int64_t p_Guid);
        static bool GetWindowShouldClose(int64_t p_Guid);
    private:
        void update(app_late_update& e);
        void on_focus(window_focus& e);

        static std::size_t find_index_with_guid(int64_t p_Guid);
        static std::size_t find_index_with_handle(GLFWwindow* p_Window);
    private:
        static WindowManager* s_Instance;

        std::vector<int64_t>           m_WindowGuids;
        std::vector<GLFWwindow*>       m_WindowHandles;
        std::vector<window_properties> m_WindowProps;

        std::size_t m_ActiveWindowIndex{};
    };
} // namespace sraphy
