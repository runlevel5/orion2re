#pragma once

namespace LOADER {
    inline void Load_Pictures_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004B3140 });
        fn();
    }
}