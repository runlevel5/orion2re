#pragma once
#include <cstdint>

namespace FIELDSAV {
    inline void __cdecl Save_Field_Stats_(int16_t* save_buf) {
        using Fn = void(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487750});
        fn(save_buf);
    }
    inline void __cdecl Restore_Field_Stats_(int16_t* save_buf) {
        using Fn = void(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004877E0});
        fn(save_buf);
    }
}