#pragma once

#include "event.h"

namespace sraphy
{
    struct window_framebuffer_size final : public Event<window_framebuffer_size>
    {
        window_framebuffer_size() = default;
        explicit window_framebuffer_size(int32_t p_Width, int32_t p_Height) : width{p_Width}, height{p_Height} {}

        int32_t width{};
        int32_t height{};
    };
}
