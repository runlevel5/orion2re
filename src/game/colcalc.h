#pragma once
#include <cstdint>

struct s_player;
struct s_colony;
struct s_star_data;

namespace COLCALC {
    inline int8_t (&_minerals_per_mine)[8] =
        *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x0054F160});

    inline int8_t* _morale_lookup_table = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F178});
    inline int8_t* _non_capital_morale_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F180});
    inline int8_t* _barracks_morale_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F188});

    inline const char*& _government_morale_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AE0});
    inline const char*& _mixed_races_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AE4});
    inline const char*& _unification_has_no_morale_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B00});
    inline const char*& _no_capital_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B10});
    inline const char*& _no_barracks_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B38});
    inline const char*& _total_morale_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571BC8});
    inline const char*& _leader_bonus_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571BE0});

    int32_t __cdecl Mixed_Race_Morale_Penalty_(s_colony* colony);

    int __cdecl Colony_Officer_(s_colony* colony);

    short __cdecl Colony_Race_Pop_Limit_(s_colony* colony, int16_t player_idx);

    inline int16_t __cdecl Owned_Officer_Level_(int officer_idx) {
        using Fn = int16_t(__cdecl*)(int);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0043e780})(officer_idx);
    }

    int16_t __cdecl Colony_Industry_Per_Worker_(s_colony* colony, int16_t player_idx, void* out);
    int16_t __cdecl Colony_Food2_Per_Farmer_(s_colony* colony, int16_t player_idx, void* out);
    int16_t __cdecl Colony_Research_Per_Scientist_(s_colony* colony, int16_t player_idx, void* out);

    void __cdecl Twiddle_Initial_Homeworlds_();
    void __cdecl Twiddle_Initial_Homeworld_(s_colony* colony);

    void __cdecl Pre_Import_Computing_(s_colony* colony);

    void __cdecl Pass_Out_Imports_(int16_t owner, s_colony* colony);

    void __cdecl Recalculate_Colony_(s_colony* colony);

    void __cdecl Compute_All_Star_Colony_Stuff_();
    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* star);

    void __cdecl Init_Surrenders_();

    inline void __cdecl No_Op1_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004FBEF0})();
    }

    inline int32_t __cdecl Player_Research_Cost_(s_player* plr, uint16_t field_idx) {
        using Fn = int32_t(__cdecl*)(s_player*, uint16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f73e0})(plr, field_idx);
    }

    int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int32_t accumulated_total, int32_t produced, int32_t cost);

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr);

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr);

    inline int __cdecl Get_First_Player_Colony_Idx(int player_idx) {
        using Fn = int(__cdecl*)(int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fc110 });
        return fn(player_idx);
    }

    inline uint8_t __cdecl Monster_At_Star_(int star_idx) {
        using Fn = uint8_t(__cdecl*)(int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fc700 });
        return fn(star_idx);
    }

    inline void __cdecl All_Colony_Calculations_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004f91d0 });
        fn();
    }

    void __cdecl Colony_Pop_Grows_(s_colony* colony);

    inline void __cdecl Colony_Specialty_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f8d00});
        fn(colony);
    }

    void __cdecl Update_Colony_Pop_Specialty_(s_colony* colony);

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony);

    void __cdecl Colony_Morale_(s_colony* colony, char* debug);

    inline void __cdecl Colony_Industry_Maintenance_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f6ed0})(colony);
    }

    inline void __cdecl Colony_Industry_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f4520})(colony, unused);
    }

    inline void __cdecl Colony_Food_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f37f0})(colony, unused);
    }

    inline void __cdecl Colony_Food_Maintenance_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f6d50})(colony);
    }

    inline void __cdecl Colony_Replicators_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f7030})(colony);
    }

    inline void __cdecl Colony_Research_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f4fb0})(colony, unused);
    }

    inline void __cdecl Colony_Industry_To_Tax_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f72a0})(colony);
    }

    inline void __cdecl Colony_BC_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f5520})(colony, unused);
    }

    inline void __cdecl Colony_BC_Maintenance_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f7310})(colony);
    }

    bool __cdecl Colony_Is_Blockaded(s_colony* colony);

    inline int16_t __cdecl Size_And_Climate_Race_Pop_Limit_(int8_t size, uint32_t climate, int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int8_t, uint32_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004f5b90 });
        return fn(size, climate, player_idx);
    }
}