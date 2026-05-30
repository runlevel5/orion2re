#pragma once

namespace time_ {
    using Fn = int32_t(*)();
    extern Fn _time;

    extern Fn _time2;

    int32_t __cdecl time_();
}