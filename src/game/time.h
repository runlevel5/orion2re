#pragma once

namespace time_ {
    using Fn = int32_t(*)();
    inline Fn& _time = *reinterpret_cast<Fn*>(std::uintptr_t{0x005897F0});

    inline Fn& _time2 = *reinterpret_cast<Fn*>(std::uintptr_t{0x00589A10});

    int32_t __cdecl time_();
}