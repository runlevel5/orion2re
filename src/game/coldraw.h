#pragma once
#include <cstdint>

struct s_animation_header;
struct s_coldraw_road;
struct s_coldraw_veggie_cell;

namespace COLDRAW {
    void __cdecl Calculate_Squish_Step_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4);

    extern s_coldraw_road _road_list[7][7];

    extern s_coldraw_veggie_cell _veggie_list[6][6];

    void __cdecl Road_Back_To_Front_(int16_t index, int16_t* y, int16_t* x);

    void __cdecl Draw_Road_List_();

    void __cdecl Load_Road_List_Anims_();

    void __cdecl Build_Road_List_Based_On_Bldg_List_();

    extern uint8_t _DAT_5a7760;
    extern uint8_t _DAT_5a778a;

    void __cdecl Draw_Simplest_Path_(int16_t prev_y, int16_t prev_x, int16_t local_y, int16_t local_x, uint8_t* p1, uint8_t* p2);

    void __cdecl Link_Buildings_();

    int16_t __cdecl N_Roads_Around_CR_(int16_t x, int16_t y);

    int16_t __cdecl Pick_Random_Road_At_And_Around_CR_(int16_t* x, int16_t* y, int16_t* conn_idx, int16_t search_count);

    uint8_t __cdecl Do_Pick_Random_Road_At_And_Around_CR_(int16_t* c, int16_t* r, int16_t* param_3);

    void __cdecl Build_Veggie_List_Based_On_Bldg_List_();

    int16_t __cdecl Veggie_Type_For_Colony_(int16_t colony_idx);

    int16_t __cdecl Pick_Random_Veggie_Anim_Entry_For_Colony_CR_(int16_t colony_index, int16_t row, int16_t col);

    void __cdecl Clear_Veggie_Draw_Flags_();

    void __cdecl Draw_Veggie_CR_(int16_t row_idx, int16_t col_idx, uint8_t draw_pass_count);

    int16_t __cdecl Do_Colony_Info_Pop_Stuff_For_Pop_(int16_t colony_idx, int16_t param_2, int16_t mode, int16_t x, int16_t y, int16_t x2, uint8_t show_shadows, int16_t* idx_out, int16_t* count_ptr, int16_t* x_limit_ptr, int16_t* field_id_ptr);

    void __cdecl Draw_Colony_Prod_Both_(int16_t colony_idx, int16_t prod_type, int16_t x, int16_t y, int16_t width, int16_t spacing, int8_t frame);

    void __cdecl Draw_Colony_Prod_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4, int16_t param_5, int16_t param_6);

    void __cdecl Draw_Info_Morale_Both_(int16_t colony_idx, int16_t x, int16_t y, int width, char is_mini);

    void __cdecl Draw_Info_Morale_(int16_t param_1, int16_t param_2, int16_t param_3, int32_t param_4);

    void __cdecl Draw_Colony_PC_Bar_(int16_t x, int16_t y, int16_t total, int16_t current);

    void __cdecl Draw_Colony_Wee_Prod_(int16_t colony_idx, int16_t prod_idx, int16_t x, int16_t y, int16_t width, int16_t height);

    void __cdecl Draw_Info_Wee_Morale_(int16_t colony_idx, int16_t x, int16_t y, int width);
}