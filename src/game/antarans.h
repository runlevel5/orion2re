#pragma once

namespace antarans {
    inline void __cdecl Antaran_Invasion_Check_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b3690 });
        fn();
    }

    int32_t __cdecl Delta_Stardate_();
    int16_t __cdecl Delay_();
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_();
    int16_t __cdecl Next_Antaran_Ship_Size_();

    inline int16_t __cdecl Antarans_Offensive_Resource_Bonus_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b40d0});
        return fn();
    }

    inline bool __cdecl Antaran_Defensive_Force_At_Maximum_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b4130});
        return fn();
    }

    void __cdecl Increment_Antaran_Resource_Level_();

    inline int16_t __cdecl No_Small_Ships_After_N_Turns_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b4230});
        return fn();
    }

    inline int16_t __cdecl No_Medium_Ships_After_N_Turns_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b4260});
        return fn();
    }

    inline int8_t (&_antaran_ship_cost)[9] = *reinterpret_cast<int8_t (*)[9]>(std::uintptr_t{0x00558560});
    inline int8_t (&_n_max_antaran_off_ships)[9] = *reinterpret_cast<int8_t (*)[9]>(std::uintptr_t{0x00558570});
    inline const char* s_offensive_00558598 = reinterpret_cast<const char*>(std::uintptr_t{0x00558598});
    inline uint8_t& _offensive_titan_ship_built = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CD84});

    void __cdecl Build_Antaran_Ships_(short *resources, short *ships, int8_t *max_ships, char is_offensive);
    void __cdecl Build_Antaran_Offensive_Ships_();

    void __cdecl Build_List_Of_Stars_By_Player_(short player_idx, short* star_list, short* list_count);

    short __cdecl Get_Antaran_Target_Star_(short player_idx);

    inline const char* s_defensive_0055858c = reinterpret_cast<const char*>(std::uintptr_t{0x0055858C});
    inline int8_t (&_n_max_antaran_def_ships)[9] = *reinterpret_cast<int8_t (*)[9]>(std::uintptr_t{0x00558580});
    void __cdecl Build_Antaran_Defensive_Ships_();

    inline const char* s_2d_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00558690});
    inline const char* s_ships_by_size = reinterpret_cast<const char*>(std::uintptr_t{0x0055871C});
    inline const char* s_ships_deployed_by_size = reinterpret_cast<const char*>(std::uintptr_t{0x005586FC});

    bool __cdecl Deploy_Antaran_Ships_();
}