#pragma once

#include "event.h"

namespace sraphy
{
    struct window_focus final : public Event<window_focus>
    {
        window_focus() = default;
        explicit window_focus(void* p_WinHandle, int32_t p_IsFocused)
            : winHandle{p_WinHandle}, isFocused{p_IsFocused}
        {}

        void* winHandle{};
        int32_t isFocused{};
    };
}
