#pragma once

namespace fields {
    inline void Assign_Update_Function_(void (*func)()) {
        using Fn = void(*)(void (*)());
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00523E00 });
        fn(func);
    }
}