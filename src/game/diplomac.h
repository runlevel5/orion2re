#pragma once

namespace diplomac {
    inline int16_t* _starting_relations = reinterpret_cast<int16_t*>(std::uintptr_t{0x00556B40});

    void Init_Diplomatic_Relations_();

    inline void __cdecl Declare_War(int16_t attacker_idx, int16_t victim_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00453F20 });
        fn(attacker_idx, victim_idx);
    }

    inline void __cdecl Declare_Peace_(int16_t player_1_idx, int16_t player_2_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00455410 });
        fn(player_1_idx, player_2_idx);
    }

    inline void __cdecl Break_Treaties_(int16_t initiator_idx, int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00454290 });
        fn(initiator_idx, target_idx);
    }

    inline void __cdecl Update_Diplomatic_Status_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00453cb0});
        fn();
    }

    inline void __cdecl NoOp2() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00453ca0});
        fn();
    }

    void Clear_Diplomacy_Messages_();

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

    inline void __cdecl NPC_Sneak_Attacks_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004573d0});
        fn();
    }
}