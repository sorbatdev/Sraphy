#pragma once

namespace sraphy
{
    struct window_properties
    {
        std::size_t order{}; // set internally

        int32_t        width{};
        int32_t        height{};
        CharArray<256> title{};

        bool decorated{};
        bool fullscreen{};
        bool maximized{};

        bool    vsync{};
        int32_t refreshRate{};
    };

    bool operator==(window_properties const& p_Lhs, window_properties const& p_Rhs);
} // namespace sraphy
