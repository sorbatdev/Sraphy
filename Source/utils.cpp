#include "utils.h"

namespace sraphy
{
    namespace
    {
        static std::random_device s_RngDevice{};
        static std::mt19937_64    s_RngEngine(s_RngDevice());
        static std::uniform_int_distribution<int64_t> s_UniDist(-INT64_MIN);
    }

    int64_t CreateGuid()
    {
        return s_UniDist(s_RngEngine);
    }
} // namespace sraphy
