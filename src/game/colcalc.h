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

    inline int8_t* _difficulty_bc_lookup_table = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F1A8});
    inline char*& _government_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571AD4});
    inline char*& _special_income_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B08});
    inline char*& _taxed_from_industry_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B30});
    inline char*& _colony_debug_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B40});
    inline char*& _morale_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B54});
    inline char*& _taxes_collected_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B7C});
    inline char*& _trade_goods_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BD4});
    inline char*& _total_income_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BDC});
    inline char*& _morale_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BEC});

    inline const char* const s_T_15_65 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8A4}); // "T+15,+65."
    inline const char* const s_0 = reinterpret_cast<const char*>(std::uintptr_t{0x0055110C}); // "0"
    inline const char* const s_fmt_s_00551130 = reinterpret_cast<const char*>(std::uintptr_t{0x00551130}); // "%s"
    inline const char* const s_fmt_plus_d_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8DC}); // "%+d %s"
    inline const char* const s_newline_0055136c = reinterpret_cast<const char*>(std::uintptr_t{0x0055136C}); // "\n"

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

    int __cdecl Player_Research_Cost_(s_player* plr, int16_t tech_field);

    int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int32_t accumulated_total, int32_t produced, int32_t cost);

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr);

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr);

    short __cdecl Get_First_Player_Colony_Idx(int player_idx);

    inline bool __cdecl Monster_At_Star_(int star_idx);

    void __cdecl All_Colony_Calculations_();

    void __cdecl Colony_Pop_Grows_(s_colony* colony);

    void __cdecl Colony_Specialty_(s_colony* colony);

    void __cdecl Update_Colony_Pop_Specialty_(s_colony* colony);

    inline void __cdecl Update_Player_Stats_(s_player* player) {
        using Fn = void(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f7e60});
        fn(player);
    }

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony);

    void __cdecl Colony_Morale_(s_colony* colony, char* debug);

    void __cdecl Colony_Industry_Maintenance_(s_colony* colony);

    inline void __cdecl Colony_Industry_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f4520})(colony, unused);
    }

    inline void __cdecl Colony_Food_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f37f0})(colony, unused);
    }

    void __cdecl Colony_Food_Maintenance_(s_colony* colony);

    void __cdecl Colony_Replicators_(s_colony* colony);

    inline void __cdecl Colony_Research_Production_(s_colony* colony, void* unused) {
        using Fn = void(__cdecl*)(s_colony*, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004f4fb0})(colony, unused);
    }

    void __cdecl Colony_Industry_To_Tax_(s_colony* colony);

    void __cdecl Colony_BC_Production_(s_colony* colony, char* debug_buf);

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