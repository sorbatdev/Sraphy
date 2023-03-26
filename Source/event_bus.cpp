
#include "event_bus.h"

namespace sraphy
{
    EventBus* EventBus::s_Instance;

    EventBus::EventBus()
    {
        s_Instance = this;
    }
    
    EventBus::~EventBus()
    {
        s_Instance = nullptr;
    }
}