#include "deps.h"

namespace timer {
    /**
     * @address 0052b220
     */
    void __cdecl Mark_Time_() {
        _time_mark = GetTickCount();
    }
}