#pragma once

#include "event.h"

namespace sraphy
{
    struct mouse_scroll final : public Event<mouse_scroll>
    {
        mouse_scroll() = default;
        explicit mouse_scroll(float p_XOffset, float p_YOffset) : xOffset{p_XOffset}, yOffset{p_YOffset} {}

        float xOffset{};
        float yOffset{};
    };
}
