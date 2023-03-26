#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window_properties.h"

namespace sraphy
{
    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();
    public:
        int64_t NewWindow(window_properties& p_Props);
        void    CloseWindow(int64_t p_Guid);

        int64_t     AppWindowGuid();
        GLFWwindow* AppWindowHandle();
        bool        AppWindowShouldClose();
        void        AppWindowRequestFocus();


        window_properties const& GetWindowProperties(int64_t p_Guid);
        GLFWwindow*              GetWindowHandle(int64_t p_Guid);

        void RequestFocus(int64_t p_Guid);
        bool GetWindowShouldClose(int64_t p_Guid);
    private:
        std::size_t find_index_with_guid(int64_t p_Guid);
    private:
        static WindowManager* s_Instance;

        std::vector<int64_t>           m_WindowGuids;
        std::vector<GLFWwindow*>       m_WindowHandles;
        std::vector<window_properties> m_WindowProps;
    };
} // namespace sraphy
