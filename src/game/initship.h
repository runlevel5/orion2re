#pragma once
#include <cstdint>

struct s_ship_data;
struct s_ship_design;

namespace INITSHIP {
    void __cdecl Repair_Ships_At_Colonies_();

    inline void __cdecl Repair_Ship_Full_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00483050});
        fn(ship_idx);
    }

    void __cdecl Init_Ship_Designs_(int16_t player_idx);
    
    void __cdecl Get_Ship_Combat_Bonuses_(int16_t ship_idx, int16_t* ship_defense, int16_t* ship_missile_defense, int16_t* ship_attack);

    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx);

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Player_Strategic_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Automatic_Design_Equipment_(int16_t player_idx);

    void __cdecl Update_Equipment_On_Ships_Being_Built_(int16_t player_idx);

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx);

    int16_t __cdecl Calc_Player_FTL_Speed_(int16_t player_idx);

    int16_t __cdecl Best_Armor_(int16_t player_idx);

    int16_t __cdecl Best_Computer_(int16_t player_idx);

    int16_t __cdecl Best_Ship_Shield_(int16_t player_idx);

    inline int16_t __cdecl Best_Beam_Weapon_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481420});
        return fn(player_idx);
    }

    inline int16_t __cdecl Best_Secondary_Weapon_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481630});
        return fn(player_idx);
    }

    inline int16_t __cdecl Best_Missile_Weapon_NT_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481800});
        return fn(player_idx);
    }

    int16_t __cdecl Get_FTL_Speed_(int16_t player_idx, int16_t ftl_type);

    inline int16_t __cdecl Get_Design_Structure_(s_ship_design design) {
        using Fn = int16_t(__cdecl*)(s_ship_design);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00483120});
        return fn(design);
    }

    int16_t __cdecl Get_Structure_(int16_t hull_index, bool is_fort, int16_t armor_tech);

    int16_t __cdecl Get_Ship_Structure_(int16_t ship_idx);

    int16_t __cdecl Get_Ship_Armor_Hits_(int16_t ship_idx);

    void __cdecl Auto_Strategic_Design_Ship_(int16_t player_idx, int16_t design_idx, s_ship_design* design);

    void __cdecl Load_Small_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Medium_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Large_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Huge_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Titan_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Antaran_Ship_Design_(s_ship_design* design, int16_t hull_size);

    inline void __cdecl Load_Small_Antaran_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047f300});
        fn(ship_idx);
    }

    inline void __cdecl Load_Medium_Antaran_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047f6e0});
        fn(ship_idx);
    }

    inline void __cdecl Load_Large_Antaran_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047fb50});
        fn(ship_idx);
    }

    inline void __cdecl Load_Huge_Antaran_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004800d0});
        fn(ship_idx);
    }

    inline void __cdecl Load_Titan_Antaran_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00480720});
        fn(ship_idx);
    }

    inline void __cdecl Load_Guardian_Ship_Design_(s_ship_design* design) {
        using Fn = void(__cdecl*)(s_ship_design*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00482da0})(design);
    }

    inline void __cdecl Load_Amoeba_Ship_Design_(s_ship_design* design, uint8_t owner) {
        using Fn = void(__cdecl*)(s_ship_design*, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004829f0})(design, owner);
    }

    inline void __cdecl Load_Crystal_Ship_Design_(s_ship_design* design, uint8_t owner) {
        using Fn = void(__cdecl*)(s_ship_design*, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00482c60})(design, owner);
    }

    inline void __cdecl Load_Dragon_Ship_Design_(s_ship_design* design, uint8_t owner) {
        using Fn = void(__cdecl*)(s_ship_design*, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00482780})(design, owner);
    }

    inline void __cdecl Load_Eel_Ship_Design_(s_ship_design* design, uint8_t owner) {
        using Fn = void(__cdecl*)(s_ship_design*, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00482b30})(design, owner);
    }

    inline void __cdecl Load_Hydra_Ship_Design_(s_ship_design* design, uint8_t owner) {
        using Fn = void(__cdecl*)(s_ship_design*, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004828d0})(design, owner);
    }

    void __cdecl Load_Colony_Ship_Design_(int16_t player_idx, s_ship_design* design);

    inline char* s_Colony_Ship_Localized = reinterpret_cast<char*>(std::uintptr_t{0x00558E28});

    void __cdecl Load_Doom_Star_Ship_Design_(int16_t owner_idx, s_ship_design* design);

    inline void __cdecl Update_Strategic_Design_Weapons_Specials_(s_ship_design* design, int16_t player_idx, int16_t design_idx) {
        using Fn = void(__cdecl*)(s_ship_design*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004823D0});
        fn(design, player_idx, design_idx);
    }

    inline void __cdecl Update_Strategic_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004826D0})(ship_idx);
    }

    inline int16_t __cdecl Best_Point_Defense_Weapon_(int16_t owner_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00481510})(owner_idx);
    }

    inline int16_t __cdecl Best_Bomb_Weapon_(int16_t owner_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004818F0})(owner_idx);
    }

    int16_t __cdecl Cost_Of_Strategic_Design_(int16_t govt_type, struct s_ship_design* design);

    inline void __cdecl Get_Design_Combat_Bonuses_(s_ship_design *design, int16_t* bonus1, int16_t* bonus2, int16_t* bonus3) {
        // ABI is weird when passed by value, so just call in assembly
        /*
        using Fn = void(__cdecl*)(s_ship_design, int16_t*, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0047ee30 })(design, bonus1, bonus2, bonus3);
        */
        __asm {
            mov eax, bonus3
            push eax
            mov eax, bonus2
            push eax
            mov eax, bonus1
            push eax

            sub esp, 0x64
            mov edi, esp
            mov esi, design
            mov ecx, 0x18
            rep movsd
            movsw
            movsb

            mov eax, 0x0047ee30
            call eax
            add esp, 0x70
        }
    }
}