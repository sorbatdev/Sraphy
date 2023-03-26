#include "window_properties.h"

namespace sraphy
{
    bool operator==(window_properties const& p_Lhs, window_properties const& p_Rhs)
    {
        return p_Lhs.order == p_Rhs.order;
    }
} // namespace sraphy