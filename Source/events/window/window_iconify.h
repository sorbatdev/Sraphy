#pragma once

#include "event.h"

namespace sraphy
{
    struct window_iconify final : public Event<window_iconify>
    {
        window_iconify() = default;
        window_iconify(int32_t p_Iconified) : iconified{p_Iconified} {}

        int32_t iconified{};
    };
}
