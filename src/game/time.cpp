#include "pch.h"

namespace time_ {

Fn _time;

Fn _time2;

int32_t __cdecl time_() {
    int32_t cVar1 = clock();
    int32_t scaled = (int32_t)((uint32_t)cVar1 * 1000u);
    return scaled / 1000;
}

}