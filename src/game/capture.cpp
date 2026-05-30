#include "pch.h"

namespace capture {
    uint16_t _release_flag;

    void Release_Version_() {
        _release_flag = 1;
    }

    uint16_t __cdecl Check_Release_Version_() {
        return _release_flag;
    }

    void __cdecl Screen_Flic_Capture_() {
    }
}