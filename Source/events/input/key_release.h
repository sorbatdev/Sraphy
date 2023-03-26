#pragma once

#include "event.h"

namespace sraphy
{
    struct key_release final : public Event<key_release>
    {
        key_release() = default;
        explicit key_release(int32_t p_Key) : key{p_Key} {}
        
        int32_t key{};
    };
}
