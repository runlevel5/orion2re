#include "pch.h"

namespace time_ {

int32_t __cdecl time_() {
    int32_t cVar1 = clock();
    int32_t scaled = (int32_t)((uint32_t)cVar1 * 1000u);
    return scaled / 1000;
}

}