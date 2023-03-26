#pragma once

#include "event.h"
#include "delegate.h"

namespace sraphy
{
    template<class T>
    concept EngineEvent = std::is_base_of_v<sraphy::Event<T>, T>;

    class EventBus
    {
    public:
        EventBus();
        ~EventBus();

        template<EngineEvent EventType, typename... Args>
        static void RaiseEvent(Args&&... p_Args);
        
        template<EngineEvent EventType>
        static void SubscribeToEvent(Delegate<void(EventType&)> p_Callback);

        template<EngineEvent EventType>
        static void UnsubscribeFromEvent(Delegate<void(EventType&)> p_Callback);

    private:
        static EventBus* s_Instance;

        std::unordered_map<int64_t, std::vector<Delegate<void(BaseEvent&)>>> m_Callbacks;
    };

    template<EngineEvent EventType, typename... Args>
    inline void EventBus::RaiseEvent(Args&&... p_Args)
    {
        auto& callbacks = s_Instance->m_Callbacks;
        if (callbacks.contains(g_UniqueEventId<EventType>))
        {
            EventType e(std::forward<Args>(p_Args)...);
            int64_t id = g_UniqueEventId<EventType>;
            for (auto const& subscriber_function : callbacks[id])
            {
                std::invoke(subscriber_function, e);
            }
        }
    }

    template<EngineEvent EventType>
    inline void EventBus::SubscribeToEvent(Delegate<void(EventType&)> p_Callback)
    {
        auto& callbacks  = s_Instance->m_Callbacks;

        int64_t id = g_UniqueEventId<EventType>;
        callbacks.try_emplace(id).first->second.push_back(std::move(*std::launder(reinterpret_cast<Delegate<void(BaseEvent&)>*>(&p_Callback))));
    }

    template<EngineEvent EventType>
    inline void EventBus::UnsubscribeFromEvent(Delegate<void(EventType&)> p_Callback)
    {
        int64_t id      = g_UniqueEventId<EventType>;
        auto& callbacks  = s_Instance->m_Callbacks;
        auto& event_list = callbacks.at(id);

        // Check for the same delegate in the subscribers, then delete it
        event_list.erase(
            std::remove(
                event_list.begin(),
                event_list.end(),
                *std::launder(reinterpret_cast<Delegate<void(BaseEvent&)>*>(&p_Callback))
            ),
            event_list.end()
        );
    }
} // namespace sraphy

#define SUB_TO_EVENT_MEM_FUN(EventType, MemFuncName) \
    ::sraphy::EventBus::SubscribeToEvent( \
        ::sraphy::Delegate<void(EventType&)>::template From< \
            std::remove_cvref_t<decltype(*this)>, \
            &std::remove_cvref_t<decltype(*this)>::MemFuncName \
        >(this) \
    )

#define SUB_TO_EVENT_FREE_FUN(EventType, FuncPtr) \
    ::sraphy::EventBus::SubscribeToEvent( \
        ::sraphy::Delegate<void(EventType&)>::template From<FuncPtr>() \
    )

#define UNSUB_FROM_EVENT_MEM_FUN(EventType, MemFuncName) \
    ::sraphy::EventBus::UnsubscribeFromEvent( \
        ::sraphy::Delegate<void(EventType&)>::template From< \
            std::remove_cvref_t<decltype(*this)>, \
            &std::remove_cvref_t<decltype(*this)>::MemFuncName \
        >(this) \
    )

#define UNSUB_FROM_EVENT_FREE_FUN(EventType, FuncPtr) \
    ::sraphy::EventBus::UnsubscribeFromEvent(          \
            ::sraphy::Delegate<void(EventType&)>::template From<FuncPtr>() \
    )
