#pragma once

namespace diplomac {
    inline int16_t* _starting_relations = reinterpret_cast<int16_t*>(std::uintptr_t{0x00556B40});

    inline int16_t (&_personality_peace_duration)[12] = 
        *reinterpret_cast<int16_t(*)[12]>(std::uintptr_t{0x00556CC8});

    void Init_Diplomatic_Relations_();

    inline void __cdecl Change_Relations_(int16_t delta, int16_t player1, int16_t player2, int16_t type, int16_t star_id, int16_t unused) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004513f0});
        fn(delta, player1, player2, type, star_id, unused);
    }

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

    void Determine_First_Contacts_();

    void Determine_Diplomacy_Messages_();

    inline void __cdecl Limit_Treaty_Modifiers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004539D0});
        fn();
    }

    inline void __cdecl Determine_Bad_Message_(int16_t i, int16_t j, int incident) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00451F40});
        fn(i, j, incident);
    }

    inline void __cdecl Determine_Good_Message_(int16_t i, int16_t j, int incident) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00452CF0});
        fn(i, j, incident);
    }

    inline void __cdecl Peace_Proposal_(int16_t i, int16_t j, int16_t param_3) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00452420});
        fn(i, j, param_3);
    }

    inline void __cdecl Required_Alliances_(int16_t i, int16_t j) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00452BE0});
        fn(i, j);
    }

    inline void __cdecl NPC_Required_Alliances_(int16_t i, int16_t j) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004EB6C0});
        fn(i, j);
    }

    inline void __cdecl Unprovoked_Good_Messages_(int16_t i, int16_t j) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00453100});
        fn(i, j);
    }

    void __cdecl End_Of_Turn_Diplomacy_Adjustments_();

    inline void __cdecl NPC_Sneak_Attacks_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004573d0});
        fn();
    }

    int16_t __cdecl Get_Player_Diplomacy_Personality_(int16_t player_idx, int16_t target_idx);

    inline void __cdecl Diplomacy_Broken_Contacts_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00455590 });
        fn();
    }
}