
#include "editor.h"

#include "event_bus.h"
#include "window_manager.h"
#include "events/events.h"
#include "gfx_context.h"

namespace sraphy
{
    static void RaiseEvents()
    {
        app_update{};
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

        window_manager->AppWindowRequestFocus();
        InitializeGfxContext(window_manager->AppWindowHandle());

        using AppClock = std::chrono::high_resolution_clock;

        auto start = AppClock::now();
        auto end   = AppClock::now();
        auto delta_time = end - start;

        while (!window_manager->AppWindowShouldClose())
        {
            end = AppClock::now();
            delta_time = end - start;
            start = AppClock::now();

            EventBus::RaiseEvent<app_update>(static_cast<float>(delta_time.count()) * 0.001f * 0.001f);
            glfwPollEvents();
            glfwSwapBuffers(window_manager->AppWindowHandle());
        }

        return 0;
    }

    Editor::Editor()
    {
        SUB_TO_EVENT_MEM_FUN(app_update, Update);
    }
    
    Editor::~Editor()
    {
    }

    void Editor::Update(app_update& e)
    {
        LOG_INFO("delta: {}\n", e.deltaTime);
    }
} // namespace sraphy
