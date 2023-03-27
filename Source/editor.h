#pragma once

#include <imgui.h>

#include "events/app/app_update.h"
#include "gfx/framebuffer.h"
#include "events/app/imgui_update.h"

namespace sraphy
{
    int32_t Run();

    class App
    {
    public:
        App();
        ~App();
    private:
        void update(app_update& e);
        void gui_update(imgui_update& e);

        void draw_preview_window();
        void draw_node_editor();
    private:
        float m_AspectRatio{};

        FrameBuffer m_MainFB;
        float       m_MainFBAspectRatioBefore{};
        ImVec2      m_MainFBDimensions{};
        ImVec2      m_MainFBUVCoords1{};
        ImVec2      m_MainFBUVCoords2{};

        FrameBuffer m_NodeEditorFB;
        ImVec2      m_NodeEditorFBDimensions{};
    };
}
