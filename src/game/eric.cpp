#include "deps.h"

namespace ERIC {
    void __cdecl Eric_Init_() {
        COLONY::Init_Colonies_();
        EVENTS::Init_Events_();
    }
}