#pragma once
#include <cstdint>

namespace COLMOVE {
    extern int16_t _cluster_colony_n;
    extern uint8_t _g_hacky_dont_draw_cluster_flag;

    void __cdecl Draw_Cluster_(int16_t x, int16_t y);

    int16_t __cdecl Get_Num_Unemployed_(int16_t colony_idx);

    int16_t __cdecl Get_First_Unemployed_(int16_t colony_idx);

    void __cdecl Clear_Cluster_();

    void __cdecl Get_Cluster_(int16_t colony_idx, int16_t start_pop_idx);

    bool __cdecl Pops_Identical_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4);

    void __cdecl Send_Cluster_(int16_t colony_idx, int16_t requested_job);

    void __cdecl Colony_Pop_String_(int16_t colony_idx, int16_t pop_idx, char* output_buffer);

    uint8_t __cdecl Give_Colonist_New_Job_(int16_t colony_idx, int16_t pop_idx, int16_t new_job);
}