#pragma once

namespace CONTACT {
    inline void __cdecl Compute_Contacts_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00499a20});
        fn();
    }
}