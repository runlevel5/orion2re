#pragma once

namespace fields {
    inline void Assign_Update_Function_(void (*func)()) {
        using Fn = void(*)(void (*)());
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00523E00 });
        fn(func);
    }

    inline void __cdecl Deactivate_Auto_Function_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051ff00 })();
    }

    inline void __cdecl Init_Fields_State_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00521080 })();
    }
}