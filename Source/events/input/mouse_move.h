#pragma once

#include "event.h"

namespace sraphy
{
    struct mouse_move final : public Event<mouse_move>
    {
        mouse_move() = default;
        explicit mouse_move(float p_X, float p_Y) : x{p_X}, y{p_Y} {}

        float x{};
        float y{};
    };
}
