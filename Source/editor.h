#pragma once

#include "events/app/app_update.h"

namespace sraphy
{
    int32_t Run();

    class Editor
    {
    public:
        Editor();
        ~Editor();
    private:
        void update(app_update& e);
    };
}
