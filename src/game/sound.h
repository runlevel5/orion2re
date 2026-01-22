#pragma once

namespace sound {
    inline void*& _soundBuffer =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0055B520 });

    inline uint32_t& _soundBufferSize =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B524 });

    inline void __cdecl Audio_Clear_Cache_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E4A0 });
        fn();
    }

    void __cdecl Set_Audio_Buffer_(void* buffer);
}