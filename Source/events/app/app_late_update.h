#pragma once

#include "event.h"

namespace sraphy
{
    struct app_late_update final : Event<app_late_update>
    {
        app_late_update() = default;
        app_late_update(float p_DeltaTime) : deltaTime{ p_DeltaTime }
        {};

        float deltaTime;
    };
} // namespace sraphy
