#pragma once

namespace antarans {
    void __cdecl Antaran_Invasion_Check_();

    void __cdecl Restock_Antaran_Ship_(int16_t ship_type_idx);

    void __cdecl Kill_Antaran_Defensive_Ship_(int16_t ship_idx);

    void __cdecl Kill_Antaran_Offensive_Ship_(int16_t ship_idx);

    int32_t __cdecl Delta_Stardate_();
    int16_t __cdecl Antaran_Delay_();
    inline int16_t __cdecl Delay_() {
        return Antaran_Delay_();
    }
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_();
    int16_t __cdecl Antarans_Offensive_Resource_Bonus_();
    int16_t __cdecl Next_Antaran_Ship_Size_();

    bool __cdecl Antaran_Defensive_Force_At_Maximum_();

    void __cdecl Increment_Antaran_Resource_Level_();

    int16_t __cdecl No_Small_Ships_After_N_Turns_();

    int16_t __cdecl No_Medium_Ships_After_N_Turns_();

    extern uint8_t _antaran_ship_cost[9];
    extern uint8_t _n_max_antaran_off_ships[9];
    extern const char s_offensive_00558598[12];
    extern uint8_t _offensive_titan_ship_built;

    void __cdecl Build_Antaran_Ships_(short *resources, short *ships, uint8_t *max_ships, char is_offensive);
    void __cdecl Build_Antaran_Offensive_Ships_();

    void __cdecl Build_List_Of_Stars_By_Player_(short player_idx, short* star_list, short* list_count);

    short __cdecl Get_Antaran_Target_Star_(short player_idx);

    extern const char s_defensive_0055858c[12];
    extern uint8_t _n_max_antaran_def_ships[9];
    void __cdecl Build_Antaran_Defensive_Ships_();

    extern const char s_2d_fmt[5];
    extern const char s_ships_by_size[31];
    extern const char s_ships_deployed_by_size[31];

    bool __cdecl Deploy_Antaran_Ships_();

    int16_t __cdecl Antaran_Total_Resources_();

    int16_t __cdecl Antaran_Offensive_Force_();

    int16_t __cdecl Antaran_Defensive_Force_();

    bool __cdecl Antarans_Should_Attack_();
}