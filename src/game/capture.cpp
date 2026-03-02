#include "pch.h"

namespace capture {
    void Release_Version_() {
        _release_flag = 1;
    }

    int16_t __cdecl Check_Release_Version_() {
        return _release_flag;
    }
}