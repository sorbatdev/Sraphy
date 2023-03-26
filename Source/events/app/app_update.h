#pragma once

#include "event.h"

namespace sraphy
{
    struct app_update final : Event<app_update>
    {
        app_update() = default;
        app_update(float p_DeltaTime) : deltaTime{ p_DeltaTime } {};

        float deltaTime;
    };
} // namespace sraphy
