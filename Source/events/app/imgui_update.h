#pragma once

#include "event.h"

namespace sraphy
{
    struct imgui_update final : Event<imgui_update>
    {
        imgui_update() = default;
        explicit imgui_update(float p_DeltaTime) : deltaTime{ p_DeltaTime } {}

        float deltaTime;
    };
} // namespace sraphy
