#pragma once

namespace diplomac {
    inline int16_t* _starting_relations = reinterpret_cast<int16_t*>(std::uintptr_t{0x00556B40});

    void Init_Diplomatic_Relations_();

    inline void __cdecl Clear_Diplomacy_Messages_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00453750 });
        fn();
    }

    inline void __cdecl Diplomacy_Growth_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00450cb0 });
        fn();
    }

    inline void __cdecl Determine_First_Contacts_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00453a80 });
        fn();
    }

    inline void __cdecl Determine_Diplomacy_Messages_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00451a90 });
        fn();
    }

    inline void __cdecl End_Of_Turn_Diplomacy_Adjustments_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00450910 });
        fn();
    }
}