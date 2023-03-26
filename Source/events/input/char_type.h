#pragma once

#include "event.h"

namespace sraphy
{
    struct char_type final : public Event<char_type>
    {
        char_type() = default;
        explicit char_type(uint32_t p_Unicode) : unicode{p_Unicode} {}

        uint32_t unicode{};
    };
}
