
#include "editor.h"

#include <imgui.h>

#include "event_bus.h"
#include "window_manager.h"
#include "events/events.h"
#include "gfx_context.h"
#include "gui.h"

namespace sraphy
{
    static void RaiseEvents()
    {
        app_update{};
        app_late_update{};
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
        std::unique_ptr<Editor>        editor         = std::make_unique<Editor>();

        WindowManager::AppWindowRequestFocus();
        InitializeGfxContext(WindowManager::AppWindowHandle());

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
            
            gui::Start();

            EventBus::RaiseEvent<app_update>(delta_time);

            gui::End();

            EventBus::RaiseEvent<app_late_update>(delta_time);
        }

        gui::Shutdown();

        return 0;
    }

    Editor::Editor()
    {
        SUB_TO_EVENT_MEM_FUN(app_update, update);
    }
    
    Editor::~Editor()
    {
    }

    void Editor::update(app_update& e)
    {
        ImGui::Begin("Game");
        ImGui::End();
    }
} // namespace sraphy
