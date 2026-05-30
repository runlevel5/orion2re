#pragma once
#include <cstdint>

struct s_hof;

namespace SCORE {
    extern s_hof _hof;
    extern char _default_names[10][20];
    extern const char LBX_0055a430[10];

    extern const char s_wb[3]; // "wb"
    extern const char s_HOF_M2[7]; // "HOF.M2"

    extern s_hiscore* _hi;

    extern const char s__d__d_005523a4[6]; // "%d.%d"

    extern const char* PTR_s_SCORE_LBX_0055a440;

    extern int16_t _entry_to_flash;

    extern char _difficulty_string[5][15];

    int16_t __cdecl HOF_Version_Number_();

    void __cdecl Create_Default_HOF_File_();

    int8_t __cdecl Read_HOF_File_();

    void __cdecl Load_Hall_Of_Fame_Screen_();

    void __cdecl Init_Hall_Of_Fame_Screen_();

    int __cdecl qsort_hof_scores_descending_(const void* a, const void* b);

    int __cdecl qsort_hof_scores_ascending_(const void* param_1, const void* param_2);

    void __cdecl Get_Hall_Of_Fame_Colors_();

    void __cdecl Draw_Hall_Of_Fame_Screen_();

    int16_t __cdecl Hi_Score_Screen_(int16_t player_idx);

    void __cdecl Sort_HOF_Scores_(int16_t sort_list[10][2]);

    void __cdecl Move_Sorted_HOF_Data_(int16_t sort_list[10][2]);

    void __cdecl Write_HOF_File_();

    void __cdecl Draw_Time_Score_();

    void __cdecl Print_Score_String_(int16_t x, int16_t y, int16_t score, const char* label);

    void __cdecl Draw_Population_Score_();

    void __cdecl Draw_Generic_Score_(int16_t value, const char* label, void (__cdecl *callback)(), int width);

    void __cdecl Draw_Eliminated_Race_Pictures_();

    void __cdecl Draw_Elimination_Score_();

    void __cdecl Draw_Captured_Colonies_Score_();

    void __cdecl Draw_Cap_Pop_();

    void __cdecl Draw_Technology_Score_();

    void __cdecl Draw_Orion_Score_();

    void __cdecl Draw_Antares_Score_();

    void __cdecl Draw_Council_Score_(void);

    void __cdecl Draw_Total_Score_();

    void __cdecl Draw_Pop_(uint8_t is_captured, int16_t max_pop, int16_t current_pop);

    void __cdecl Draw_Reg_Pop_();

    void __cdecl Draw_Hi_Score_Screen_();

    void __cdecl Load_Hi_Score_Screen_();

    void __cdecl Get_Hi_Score_Colors_();

    void __cdecl Add_Hi_Score_Screen_Fields_();

    void __cdecl Init_Hi_Score_(int16_t player_idx);

    int16_t __cdecl Get_Total_Population_(int16_t player_idx);

    void __cdecl Set_Hi_Score_Pop_Icon_Ids_(int16_t player_idx);

    void __cdecl Get_From_Tmp_Pop_(s_hiscore_tmp_pop* tmp_pop_list, uint16_t race_idx, int16_t count, int16_t* out_colony_idx, int16_t* out_pop_idx);

    int __cdecl ascending_qsort_uchar_(const void* a, const void* b);

    void __cdecl Build_Captured_Colonists_Array_(int16_t player_idx);

    void __cdecl End_Of_Game_Hi_Score_(int16_t player_idx);

    void __cdecl Hall_Of_Fame_Screen_(int16_t param_1);

    int16_t __cdecl Do_Calculate_Hi_Score_(int16_t player_idx);

    void __cdecl Calculate_Hi_Scores_(int16_t player_idx);

    int16_t __cdecl Get_Elimination_Score_(int16_t player_idx);

    int16_t __cdecl Get_Technology_Score_(int16_t player_idx);

    void __cdecl Get_Technology_(int16_t player_idx, int16_t* tech_count, int16_t* hyper_tech_count);

    int8_t __cdecl Captured_Antares_(int16_t player_idx);

    int16_t __cdecl Get_Antares_Score_(int16_t player_idx);

    int8_t __cdecl Captured_Orion_(int16_t player_idx);

    int16_t __cdecl Get_Orion_Score_(int16_t player_idx);

    int32_t __cdecl Get_Captured_Colonist_Score_(int16_t player_idx);

    int16_t __cdecl Get_Population_Score_(int16_t player_idx);

    int16_t __cdecl Get_Time_Score_();

    void __cdecl nullsub();

    int8_t __cdecl Won_Council_Vote_(int16_t player_idx);

    int16_t __cdecl Get_Won_Council_Score_(int16_t player_idx);

    int16_t __cdecl Player_Score_(int16_t player_idx);
}