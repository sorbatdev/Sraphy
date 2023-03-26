#pragma once

namespace sraphy
{
    template<typename T>
    int64_t TypeHash()
    {
        std::string name = typeid(T).name();
        return static_cast<int64_t>(std::hash<std::string>()(name));
    }

    int64_t CreateGuid();

} // namespace sraphy
