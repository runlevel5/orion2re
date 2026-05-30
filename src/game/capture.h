#pragma once
#include <cstdint>

namespace capture {
    extern uint16_t _release_flag;

    void Release_Version_();

    void __cdecl Screen_Flic_Capture_();

    uint16_t __cdecl Check_Release_Version_();
}