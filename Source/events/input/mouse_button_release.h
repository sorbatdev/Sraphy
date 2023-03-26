#pragma once

#include "event.h"

namespace sraphy
{
    struct mouse_button_release final : public Event<mouse_button_release>
    {
        mouse_button_release() = default;
        explicit mouse_button_release(int32_t p_Button) : button{p_Button} {}

        int32_t button{};
    };
}
