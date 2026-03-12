#pragma once

namespace antarans {
    void __cdecl Antaran_Invasion_Check_();

    inline void __cdecl Restock_Antaran_Ship_(int16_t size) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x004B4290)(size);
    }

    inline void __cdecl Kill_Antaran_Defensive_Ship_(int32_t size) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b4170});
        fn(size);
    }

    inline void __cdecl Kill_Antaran_Offensive_Ship_(int32_t size) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b41c0});
        fn(size);
    }

    int32_t __cdecl Delta_Stardate_();
    int16_t __cdecl Delay_();
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_();
    int16_t __cdecl Antarans_Offensive_Resource_Bonus_();
    int16_t __cdecl Next_Antaran_Ship_Size_();

    bool __cdecl Antaran_Defensive_Force_At_Maximum_();

    void __cdecl Increment_Antaran_Resource_Level_();

    int16_t __cdecl No_Small_Ships_After_N_Turns_();

    int16_t __cdecl No_Medium_Ships_After_N_Turns_();

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

    int16_t __cdecl Antaran_Total_Resources_();

    int16_t __cdecl Antaran_Offensive_Force_();

    int16_t __cdecl Antaran_Defensive_Force_();

    bool __cdecl Antarans_Should_Attack_();
}