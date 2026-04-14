#pragma once

namespace time_ {
    // TYPE MISMATCH: was uint32_t&
    using Fn = int32_t(__cdecl*)();
    inline Fn& _time = *reinterpret_cast<Fn*>(std::uintptr_t{0x005897F0});

    inline Fn& _time2 = *reinterpret_cast<Fn*>(std::uintptr_t{0x00589A10});

    int32_t __cdecl time_();
}