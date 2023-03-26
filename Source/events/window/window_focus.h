#pragma once

#include "event.h"

namespace sraphy
{
    struct window_focus final : public Event<window_focus>
    {
        window_focus() = default;
        explicit window_focus(int32_t p_IsFocused) : isFocused{p_IsFocused} {}

        int32_t isFocused;
    };
}
