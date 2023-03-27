
#include "editor.h"

#include "event_bus.h"
#include "window_manager.h"
#include "events/events.h"
#include "gfx/gfx_context.h"
#include "gui.h"

namespace sraphy
{
    static void RaiseEvents()
    {
        app_update{};
        app_late_update{};
        imgui_update{};
        char_type{};
        key_press{};
        key_release{};
        mouse_button_press{};
        mouse_button_release{};
        mouse_move{};
        mouse_scroll{};
        window_close{};
        window_focus{};
        window_framebuffer_size{};
        window_iconify{};
        window_maximize{};
        window_position{};
        window_resize{};
    }

    int32_t Run()
    {
        RaiseEvents();

        std::unique_ptr<EventBus>      event_bus      = std::make_unique<EventBus>();

        std::unique_ptr<WindowManager> window_manager = std::make_unique<WindowManager>();
        WindowManager::AppWindowRequestFocus();
        InitializeGfxContext(WindowManager::AppWindowHandle());

        std::unique_ptr<App>        editor         = std::make_unique<App>();

        using AppClock = std::chrono::high_resolution_clock;

        auto start = AppClock::now();
        auto end   = AppClock::now();
        float delta_time{};
        
        gui::Setup();

        glClearColor(0.45f, 0.55f, 0.6f, 1.0f);
        while (!WindowManager::AppWindowShouldClose())
        {

            end        = AppClock::now();
            delta_time = (end - start).count() * 0.001f * 0.001f;
            start      = AppClock::now();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            EventBus::RaiseEvent<app_update>(delta_time);

            gui::Start();
            EventBus::RaiseEvent<imgui_update>(delta_time);
            gui::End();

            EventBus::RaiseEvent<app_late_update>(delta_time);
        }

        gui::Shutdown();

        return 0;
    }

    App::App()
        : m_MainFBAspectRatioBefore{ 0.0f }
        , m_MainFBUVCoords1{ ImVec2{0.0f, 1.0f} }
        , m_MainFBUVCoords2{ ImVec2{1.0f, 0.0f} }
    {
        SUB_TO_EVENT_MEM_FUN(app_update, update);
        SUB_TO_EVENT_MEM_FUN(imgui_update, gui_update);

        window_properties props = WindowManager::GetWindowProperties(WindowManager::AppWindowGuid());

        m_MainFBDimensions.x = props.width;
        m_MainFBDimensions.y = props.height;
        m_AspectRatio = m_MainFBDimensions.x / m_MainFBDimensions.y;
        m_MainFB.Initialize(m_MainFBDimensions.x, m_MainFBDimensions.y);
        m_MainFB.Bind();
        glViewport(0, 0, m_MainFBDimensions.x, m_MainFBDimensions.y);
        m_MainFB.Unbind();
    }
    
    App::~App()
    {
    }

    void App::update(app_update& e)
    {
        m_MainFB.Clear();
    }
    
    void App::gui_update(imgui_update& e)
    {
        auto const* viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(viewport);

        draw_preview_window();
        draw_node_editor();
    }
    
    void App::draw_preview_window()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        {
            ImGui::Begin("Preview");
            {
                ImVec2 drawable_area_dimensions = ImGui::GetContentRegionAvail();
                float  drawable_area_aspect_ratio = drawable_area_dimensions.x / drawable_area_dimensions.y;

                if (m_MainFBAspectRatioBefore != drawable_area_aspect_ratio)
                {

                    if (m_AspectRatio >= drawable_area_aspect_ratio)
                    {
                        m_MainFBDimensions.x = drawable_area_dimensions.x;
                        m_MainFBDimensions.y = drawable_area_dimensions.x / m_AspectRatio;
                    }
                    else
                    {
                        m_MainFBDimensions.x = drawable_area_dimensions.y * m_AspectRatio;
                        m_MainFBDimensions.y = drawable_area_dimensions.y;
                    }

                    m_MainFBAspectRatioBefore = drawable_area_aspect_ratio;
                }

                auto framebuffer_texture_id = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(m_MainFB.GetFrameBufferTextureHandle()));

                ImVec2 window_pos = ImGui::GetWindowPos();

                // Center the the preview
                ImGui::SetCursorPos(ImVec2((drawable_area_dimensions.x - m_MainFBDimensions.x) / 2, (drawable_area_dimensions.y - m_MainFBDimensions.y) / 2));

                ImGui::Image(
                    framebuffer_texture_id,
                    m_MainFBDimensions,
                    m_MainFBUVCoords1,
                    m_MainFBUVCoords2
                );
            }
            ImGui::End();
        }
        ImGui::PopStyleVar();
    }
    
    void App::draw_node_editor()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        {
            ImGui::Begin("Editor");
            {
                if (ImGui::BeginPopupContextWindow("Deneme"))
                {
                    // Add the context menu items like New node etc.
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        }
        ImGui::PopStyleVar();
    }
} // namespace sraphy
