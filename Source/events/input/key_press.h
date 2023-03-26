#pragma once

#include "event.h"

namespace sraphy
{
    struct key_press final : public Event<key_press>
    {
        key_press() = default;
        explicit key_press(int32_t p_Key) : key{p_Key} {}

        int32_t key{};
    };
}
