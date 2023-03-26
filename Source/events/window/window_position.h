#pragma once

#include "event.h"

namespace sraphy
{
    struct window_position final : public Event<window_position>
    {
        window_position() = default;
        explicit window_position(int32_t p_X, int32_t p_Y) : x{p_X}, y{p_Y} {}

        int32_t x{};
        int32_t y{};
    };
}
