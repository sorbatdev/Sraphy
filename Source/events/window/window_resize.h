#pragma once

#include "event.h"

namespace sraphy
{
    struct window_resize final : public Event<window_resize>
    {
        window_resize() = default;
        explicit window_resize(int32_t p_Width, int32_t p_Height) : width{p_Width}, height{p_Height} {}

        int32_t width{};
        int32_t height{};
    };
}
