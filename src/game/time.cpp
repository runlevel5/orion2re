#include "pch.h"

namespace time_ {

int32_t __cdecl time_() {
    int32_t cVar1 = clock();
    uint32_t scaled = (uint32_t)cVar1 * 1000u;
    return (int32_t)scaled / 1000;
}

}