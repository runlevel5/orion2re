#pragma once

namespace AIDATA {
    inline int16_t(&_g_base_planet_values)[8][360] = *reinterpret_cast<int16_t(*)[8][360]>(std::uintptr_t{ 0x0056ed28 });
    inline int16_t(*&_ship_strengths)[500][8] = *reinterpret_cast<int16_t(**)[500][8]>(std::uintptr_t{ 0x005953D4 });
    inline int16_t(*&_ship_strengths_in_nebula)[500][8] = *reinterpret_cast<int16_t(**)[500][8]>(std::uintptr_t{ 0x005953AC });
    inline int32_t(*&_ship_bomb_strengths)[500][2] = *reinterpret_cast<int32_t(**)[500][2]>(std::uintptr_t{ 0x005953D8 });
    inline int16_t(*&_planet_stage_weights)[360][8] = *reinterpret_cast<int16_t(**)[360][8]>(std::uintptr_t{ 0x0056ED20 });
    inline int32_t(*&_base_star_stage_weights)[72][8] = *reinterpret_cast<int32_t(**)[72][8]>(std::uintptr_t{ 0x0059539C });
    inline int32_t(*&_star_stage_weights)[72][8] = *reinterpret_cast<int32_t(**)[72][8]>(std::uintptr_t{ 0x005953E4 });
    inline int8_t(*&_n_turns_from_to)[72][72][8] = *reinterpret_cast<int8_t(**)[72][72][8]>(std::uintptr_t{ 0x005953D0 });
    inline int32_t(*&_n_turns_from_to_intermediate)[72][72][8] = *reinterpret_cast<int32_t(**)[72][72][8]>(std::uintptr_t{ 0x00571A28 });
    inline int8_t(*&_ship_n_turns_to)[500][72] = *reinterpret_cast<int8_t(**)[500][72]>(std::uintptr_t{ 0x005953A8 });
    inline int16_t(*&_ship_n_turns_to_intermediate)[500][72] = *reinterpret_cast<int16_t(**)[500][72]>(std::uintptr_t{ 0x00571A2C });
    inline int32_t(*&_possible_ship_strengths)[72][8][8][10] = *reinterpret_cast<int32_t(**)[72][8][8][10]>(std::uintptr_t{ 0x005953B0 });
    inline s_ai_colony_info(*&_ai_colony_info)[250] = *reinterpret_cast<s_ai_colony_info(**)[250]>(std::uintptr_t{ 0x005953E8 });
    inline s_ai_player_info(*&_ai_player_info)[8] = *reinterpret_cast<s_ai_player_info(**)[8]>(std::uintptr_t{ 0x005953B8 });
    inline s_ai_star_info(*&_ai_star_info)[72] = *reinterpret_cast<s_ai_star_info(**)[72]>(std::uintptr_t{ 0x005953C8 });
    inline uint8_t(*&_star_in_range_of_player)[72] = *reinterpret_cast<uint8_t(**)[72]>(std::uintptr_t{ 0x005953BC });
    inline uint8_t(*&_star_in_extended_range_of_player)[72] = *reinterpret_cast<uint8_t(**)[72]>(std::uintptr_t{ 0x005953EC });
    inline int32_t(*&_enroute_ship_strengths)[72][8][8][10] = *reinterpret_cast<int32_t(**)[72][8][8][10]>(std::uintptr_t{ 0x005953F0 });
    inline int8_t(*&_enroute_ships)[72][8][10] = *reinterpret_cast<int8_t(**)[72][8][10]>(std::uintptr_t{ 0x005953B4 });
    inline int8_t(*&_enroute_human_ships)[72][10] = *reinterpret_cast<int8_t(**)[72][10]>(std::uintptr_t{ 0x005953DC });
    inline int8_t(*&_enroute_nonstealthy_ships)[72][8] = *reinterpret_cast<int8_t(**)[72][8]>(std::uintptr_t{ 0x00595398 });
    inline int16_t(*&_all_ships)[500] = *reinterpret_cast<int16_t(**)[500]>(std::uintptr_t{ 0x005953C0 });
    inline int16_t(*&_i_all_combat_ships)[8][73] = *reinterpret_cast<int16_t(**)[8][73]>(std::uintptr_t{ 0x005953A0 });
    inline s_ai_tech_info*& _aitech = *reinterpret_cast<s_ai_tech_info**>(std::uintptr_t{ 0x005953CC });
    inline s_ai_refit_player(*&_airefit)[8] = *reinterpret_cast<s_ai_refit_player(**)[8]>(std::uintptr_t{ 0x005953A4 });
    inline int32_t(*&_ship_refit_desirabilities)[500] = *reinterpret_cast<int32_t(**)[500]>(std::uintptr_t{ 0x005953C4 });
    inline int16_t(*&_base_planet_values)[8][360] = *reinterpret_cast<int16_t(**)[8][360]>(std::uintptr_t{ 0x0055A6B4 });
    inline uint16_t& _n_all_ships = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005953E0 });

    inline int16_t __cdecl Colony_Dominant_Player_(char* owner) {
        using Fn = int16_t(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004efb70});
        return fn(owner);
    }

    inline void __cdecl Compute_Star_Player_Range_Info_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f0230});
        fn();
    }

    inline void __cdecl Compute_Base_Planet_Values_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004efc30});
        fn();
    }

    inline void __cdecl Compute_Contextual_Planet_Values_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004efd60});
        fn();
    }

    inline int32_t __cdecl Refit_Desirability_(s_ship_data* ship) {
        using Fn = int32_t(__cdecl*)(s_ship_data*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f0070});
        return fn(ship);
    }

    inline short __cdecl Player_N_Turns_From_Star_To_Star_With_Intermediate_(int16_t player_idx, int16_t star1, int16_t star2, int16_t* inter_ptr, bool fast) {
        using Fn = short(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004eda30});
        return fn(player_idx, star1, star2, inter_ptr, fast);
    }

    inline int __cdecl airefit_cmp_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x004eda10 })(a, b);
    }

    inline int __cdecl sort_ships_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x004effa0 })(a, b);
    }

    inline void __cdecl Deallocate_AI_Data_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004f05a0 });
        fn();
    }

    void __cdecl Compute_AI_Data_();
}