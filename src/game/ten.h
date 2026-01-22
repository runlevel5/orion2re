#pragma once

namespace ten {
    // Suggested name: ten::Initialize_Internal_Systems
    inline void init() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D7D30 });
        fn();
    }
}