#pragma once

#include "event.h"

namespace sraphy
{
    struct window_maximize final : public Event<window_maximize>
    {
        window_maximize() = default;
        explicit window_maximize(int32_t p_Maximized) : maximized{p_Maximized} {}

        int32_t maximized{};
    };
}
