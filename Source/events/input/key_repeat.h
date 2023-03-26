#pragma once

#include "event.h"

namespace sraphy
{
	struct key_repeat final : public Event<key_repeat>
	{
		key_repeat() = default;
        explicit key_repeat(int32 p_Key, int32 p_RepatCount) : key{p_Key}, repeatCount{p_RepatCount} {}

        int32 key{};
        int32 repeatCount{};
	};
}
