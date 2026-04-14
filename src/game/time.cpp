#include "pch.h"

namespace time_ {

int32_t __cdecl time_() {
    int32_t cVar1 = _clock();
    return (cVar1 * 1000) / 1000;
}

}