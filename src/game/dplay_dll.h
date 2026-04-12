#pragma once
#include <cstdint>

namespace DPLAY_DLL {
    // Suggested name: DPLAY_DLL::DirectPlayCreate
    inline HRESULT __stdcall Ordinal_1(GUID* lpGUID, IDirectPlay** lplpDP, void* pUnkOuter) {
        using Fn = HRESULT(__stdcall*)(GUID*, IDirectPlay**, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0053a8d0});
        return fn(lpGUID, lplpDP, pUnkOuter);
    }

    // Suggested name: DPLAY_DLL::DirectPlayEnumerate
    inline int32_t __stdcall Ordinal_2(void* callback, HWND context) {
        using Fn = int32_t(__stdcall*)(void*, HWND);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0053a8d6});
        return fn(callback, context);
    }
}