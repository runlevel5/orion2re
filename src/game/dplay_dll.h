#pragma once
#include <cstdint>

namespace DPLAY_DLL {
    // Suggested name: DPLAY_DLL::DirectPlayEnumerate
    inline int32_t __stdcall Ordinal_2(void* callback, HWND context) {
        using Fn = int32_t(__stdcall*)(void*, HWND);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0053a8d6});
        return fn(callback, context);
    }
}