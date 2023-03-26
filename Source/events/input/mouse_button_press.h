#pragma once

#include "event.h"

namespace sraphy
{
    struct mouse_button_press final : public Event<mouse_button_press>
    {
        mouse_button_press() = default;
        explicit mouse_button_press(int32_t p_Button) : button{p_Button} {}

        int32_t button{};
    };
}
