#pragma once

#include "utils.h"

namespace sraphy
{
    template<typename T>
    int64_t g_UniqueEventId = -1;

    class BaseEvent
    {
    public:
        virtual ~BaseEvent() = default;
    protected:
        virtual int64_t id() = 0;
    };

    template<typename T>
    class Event : public BaseEvent
    {
    public:
        Event()
        {
            if (g_UniqueEventId<T> == -1)
            {
                g_UniqueEventId<T> = TypeHash<T>();
            }
        }
    private:
        int64_t id() override { return g_UniqueEventId<T>; }//never going to be accessed, just for vtable
    };
} // namespace sraphy
