#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colors;
struct s_planet_data;
struct s_colony;
struct s_colony_bldg;

namespace COLONY {
    extern int32_t _building_cr[170];

    extern int16_t _colony_fields[21];

    extern int16_t _military_fields[2];

    extern int16_t _job_fields[3];

    extern int16_t _pop_fields_count[8];

    extern int16_t _sys_disp_summ_fields[5];

    extern int16_t _sys_disp_fields[3];

    extern int16_t _sys_disp_planet_fields[5];

    extern int16_t _pop_fields[42];

    extern s_colony_bldg _colony_bldgs[6];

    extern int16_t _colony_satellites[10];

    extern char _FIREPTS_LBX[12];
    extern const char _RACEICON_LBX[13];
    extern const char _PLANETS_LBX[12];
    extern const char _AMEBAFIN_LBX[13];
    extern const char _COLBLDG_LBX[12];
    extern const char _COLSUM_LBX[11];
    extern const char _COLSYSDI_LBX[13];
    extern const char _DIMTVFIN_LBX[13];
    extern const char _NEWGAME_LBX[12];
    extern const char _PLNTDFIN_LBX[13];
    extern const char _MAINPUPS_LBX[13];
    extern const char _BEAMS_LBX[10];
    extern const char _CMBTSFX_LBX[12];
    extern const char _SPHERSFX_LBX[13];
    extern char _COLROADS_LBX[13];

    extern int16_t _step_squish;
    extern int16_t _scanned_pop;
    extern int16_t _scanned_military;
    extern int16_t _scanned_satellite;

    void __cdecl Init_Colonies_();

    void __cdecl Colony_Screen_();

    bool __cdecl Colonize_Planet_(int16_t planet_idx);

    extern int16_t _colony_satellite_fields[10];

    extern int16_t _colony_saved_screen;
    extern int16_t _g_scanned_star;
    extern int16_t _g_scanned_planet;
    extern int16_t _planet_display_handle;

    extern uint16_t _colonists_count;

    extern int16_t _orbit_temp;
    extern int16_t _temp_colony_idx;

    extern int16_t _ship_bitm_n;
    extern int16_t _bldg_bitm_n;
    extern int16_t _g_ship_pic_n;
    extern int32_t _g_ship_player_n;
    extern s_animation_header* _ship_pic_bitm;
    extern s_animation_header* _ship_bitm;
    extern s_animation_header* _swap_bitm;

    extern s_animation_header* _mouse_bitm;

    extern const char _COLONY_LBX[11];
    extern const char _COLVEGGIE_LBX[13];
    extern const char _COLGCBT_LBX[12];
    extern const char _COLONY2_LBX[12];
    extern const char _COLPUPS_LBX[12];

    extern s_animation_header* _c_anims[36];

    void __cdecl Set_Colony_Globals_(int16_t param_1);

    extern uint8_t _debug_flag;
    extern uint8_t _using_colony_screen_palette;
    extern uint8_t _drawing_display;
    extern uint8_t _valid_colony;
    extern uint8_t _field_mode;
    extern int16_t _move_colonist;

    void __cdecl Draw_Bldg_CR_Dark_(int16_t col, int16_t row);

    extern int16_t _scanned_bldg_C;
    extern int16_t _scanned_bldg_R;
    extern uint8_t _full_draw;
    extern int32_t _colony_handle;
    extern int16_t _building_cr_xy[7][7][4];

    extern int16_t _colony_bldg_fields[36];

    extern int16_t _pop_fields_coord_var[8];

    extern int16_t _pop_fields_ref[3][42];

    extern int32_t _planet_handle;
    extern int32_t _star_handle;
    extern int32_t _orbit_n;
    extern uint8_t _cheats;
    extern int32_t _sys_disp_star_handle;
    extern int32_t _sys_disp_scanned_star_handle;

    void __cdecl Load_Colony_Anims_();
    void __cdecl Load_Colony_Screen_Seg_();
    void __cdecl Load_Planet_Screen_();
    s_animation_header* __cdecl Cache_Load_Planet_(int16_t planet_id);
    s_animation_header* __cdecl C_Anims_(int16_t anim_id);

    s_animation_header* __cdecl Load_Govt_Anim_(int16_t param_1);

    s_animation_header* __cdecl Govt_Anim_(int16_t param_1);

    s_animation_header* __cdecl Colony_Pop_Anim_(int16_t colony_idx, int16_t pop_idx);

    int16_t __cdecl Get_Effective_Pop_Player_(int16_t colony_idx, int16_t pop_idx);

    s_animation_header* __cdecl Colony_Pop_Icon_(int16_t race_index);

    s_animation_header* __cdecl Military_Anims_(int16_t param_1, int16_t param_2);
    s_animation_header* __cdecl Military_Anim_(int16_t animation_type, int16_t param_2);

    short __cdecl Pop_To_Pop_State_(short colony_idx, short pop_idx);

    char* __cdecl Pop_State_String_(int16_t state);

    char* __cdecl Conquered_String_(bool is_conquered);

    char* __cdecl Pop_Race_String_(int16_t colony_idx, int16_t pop_idx);

    char* __cdecl Job_String_(int16_t job_id);

    void __cdecl Get_Bldg_Box_(int16_t c, int16_t r, int16_t* x_arr, int16_t* y_arr);

    bool __cdecl XY_In_Bldg_Poly_(int16_t x, int16_t y, int16_t bldg_type, int32_t bldg_id);

    void __cdecl Update_Colony_Palette_();
    void __cdecl Make_Bldg_Array_For_Colony_(int16_t colony_handle, uint8_t flag);
    void __cdecl Reset_Mouse_Picture_();
    void __cdecl Clear_Mouse_Picture_();
    void __cdecl Picture_To_Mouse_(s_animation_header* anim_header);

    void __cdecl Load_Ship_Bitm_();

    void __cdecl Load_Bldg_Bitm_();

    void __cdecl Load_Officer_Anim_();

    void __cdecl Load_Galaxy_Map_Anims_();

    char* __cdecl CR_Building_Name_(int16_t building_id);

    char* __cdecl Real_Building_Name_(int16_t building_id);

    char* __cdecl Real_Building_Name2_(int16_t building_id);

    s_animation_header* __cdecl Player_Troop_Anim_(int16_t player_idx, int16_t anim_type, int16_t frame);

    char* __cdecl Player_Troop_Name_(int16_t player_idx, int16_t troop_type, int16_t race);

    void __cdecl Remap_Draw_Ship_Seg_N_Centered_(int16_t param_1, int16_t param_2, int16_t param_3);

    void __cdecl Do_Load_Colony_Anims_();

    s_animation_header* __cdecl Spy_Anim_(int16_t race_index);

    s_animation_header* __cdecl People_Anim_(int16_t param_1, int16_t param_2, int16_t param_3);

    s_animation_header* __cdecl Prod_Anims_(int16_t param_1, int16_t param_2, int8_t param_3);

    s_animation_header* __cdecl Import_Anims_(int16_t type, int16_t is_tens, int8_t frame);

    s_animation_header* __cdecl Short_Anims_(int16_t type, int16_t is_tens, uint8_t frame);

    int16_t __cdecl Satellite_Anim_Pic_(int16_t id);

    s_animation_header* __cdecl Satellite_Anim_(int16_t colony_id);

    void __cdecl Draw_Satellite_Dark_(int16_t x, int16_t y, int16_t satellite_id);

    s_animation_header* __cdecl Buffer_Reload_Planets_(int16_t entry_num);
    s_animation_header* __cdecl Buffer_Reload_Colony_(int16_t index);
    bool __cdecl Sat_Should_Be_Replaced_(int16_t building_id);
    void __cdecl Insert_Satellite_Into_Array_(int16_t satellite_id, int16_t* count);

    void __cdecl Remap_Draw_Ship_D_Centered_(int16_t x, int16_t y, int16_t design_index);

    void __cdecl Draw_Building_With_Bottom_Centered_(int16_t x, int16_t y, int16_t type, int16_t col, int16_t row);

    void __cdecl Remap_Draw_Ship_Centered_(int16_t x, int16_t y, int16_t ship_index);

    int16_t __cdecl Calculate_Colony_Turn_Count_From_Scrap_For_Prod_(int16_t colony_idx, int16_t product_id);

    int16_t __cdecl Calculate_Current_Production_Turn_Count_(int16_t colony_idx);

    void __cdecl Clear_Colony_Buidlings_();
    void __cdecl Clear_Satellite_Array_();

    void __cdecl Remove_Satellite_From_Array_(int16_t building_id);

    int16_t __cdecl N_Houses_In_Bldg_Array_();

    void __cdecl Find_Replacement_Slot_For_Building_(int16_t bldg_id, int16_t* out_row, int16_t* out_col);

    int16_t __cdecl CR_To_XY_(int16_t col, int16_t row, int16_t axis);

    void __cdecl Insert_Bldg_Into_Array_(int16_t num_houses_delta, int16_t bldg_id, int16_t bldg_type, int8_t use_back_to_front);

    void __cdecl Sort_Bldg_Array_Columns_();

    bool __cdecl Get_Bldg_CR_(int16_t building_id, int16_t* row_out, int16_t* col_out);

    void __cdecl Turn_To_Rubble_CR_Bldg_(int16_t param_1);

    void __cdecl Bldg_Back_To_Front_(int16_t building_type, int16_t* out_val1, int16_t* out_val2);

    void __cdecl Draw_Colony_Bldg_(int16_t col, int16_t row);

    void __cdecl Draw_Colony_Bldgs_();

    void __cdecl Draw_Colony_Satellites_();

    void __cdecl Draw_Colony_Info_Background_();

    void __cdecl Box_Galaxy_Map_Star_(int16_t star_idx, int16_t param_2);

    void __cdecl Print_Sys_Disp_Scanned_String_(char* text);

    void __cdecl Print_Scanned_String_(char* text);

    void __cdecl Print_Building_Name_(int16_t param_1, int16_t param_2);

    void __cdecl Print_Scanned_Military_();

    void __cdecl Print_Scanned_Satellite_();

    void __cdecl Draw_Colony_Info_();

    void __cdecl Draw_Colony_Info_Military_();

    int __cdecl Draw_Colony_Info_Military_For_(int param_1, int param_2, int param_3);

    void __cdecl Draw_Colony_Info_Production_For_(int32_t selection);

    int __cdecl Do_Colony_Info_Military_Stuff_For_(int military_index, int start_n, int param_3, int mode);

    void __cdecl Set_Colony_Font_To_(int32_t style);

    void __cdecl Draw_Colony_Info_Officer_();

    void __cdecl Draw_Colony_Info_Production_();

    void __cdecl Draw_Colony_Info_Pop_For_(int32_t type);

    void __cdecl Draw_Colony_Info_Pop_Scanned_();

    void __cdecl Draw_Colony_Info_Pop_();

    void __cdecl Draw_Colony_Info_Morale_();

    void __cdecl Draw_Info_Name_And_Pop_();

    void __cdecl Draw_Info_Build_();

    void __cdecl Draw_Current_Production_Graph_();

    void __cdecl Draw_Turn_Count_();

    void __cdecl Draw_Sys_Disp_Scanned_();

    void __cdecl Draw_Colony_Info_Scanned_();

    void __cdecl No_Op1_();

    void __cdecl Allocate_Mini_Planet_Cache_();

    void __cdecl Draw_Colony_Screen_();

    void __cdecl Add_Job_Field_For_(int16_t index);

    void __cdecl Add_Move_Colonist_Job_Field_For_(int16_t index);

    int __cdecl Do_Colony_Info_Military_Stuff_For_Aux_(int index, int prev_ret, int count);

    void __cdecl Add_Job_Fields_();

    void __cdecl Add_Military_Fields_();

    void __cdecl Add_Screen_Fields_();

    void __cdecl Add_Stacking_Fields_();

    void __cdecl Add_Satellite_Fields_();

    void __cdecl Add_Bldg_Fields_();

    int __cdecl Bldg_Coords_To_Centered_Screen_Coord_(int grid_y, int grid_x, int axis);

    void __cdecl Update_Fields_();

    void __cdecl Do_Informational_And_Decision_Popups_();

    bool __cdecl Colony_Has_Natives_(int16_t colony_idx);

    int8_t __cdecl Evaluate_Input_(int16_t* param_1, int16_t* param_2);

    uint32_t __cdecl Check_System_Display_Fields_(int16_t* param_1, int16_t* param_2);

    void __cdecl Do_Cheats_();

    void __cdecl Show_Morale_(int16_t colony_idx);

    void __cdecl Show_BC_Production_(int16_t colony_idx);

    void __cdecl Show_Food_Production_(int16_t colony_idx);

    void __cdecl Set_Colony_Screen_To_New_Colony_(int16_t colony_idx);

    int8_t __cdecl Evaluate_Screen_Input_(int16_t* input_field);

    void __cdecl Check_Job_Fields_(int16_t* p_clicked_id, int16_t* p_target_id);

    void __cdecl Pickup_Colonists_(int16_t colonists);

    int16_t __cdecl Get_Scanned_Pop_(int16_t field_idx);

    int16_t __cdecl Get_Selected_Pop_(int16_t param_1);

    void __cdecl Check_Military_Fields_(int16_t* param_1, int16_t* param_2);

    void __cdecl Check_Satellite_Fields_(int16_t* param_1, int16_t* param_2);

    void __cdecl Check_Bldg_Fields_(int16_t* param_1, int16_t* param_2);

    void __cdecl Fix_Colony_Queue_(int16_t colony_idx);

    void __cdecl Squish_Out_Colony_Queue_Slot_(int16_t colony_idx, int16_t slot_idx);

    void __cdecl Defragment_Colony_Queue_(int16_t colony_idx);

    void __cdecl Cleanup_Colony_Queue_(int16_t colony_idx);

    void __cdecl Col_Calc_Wrapper_(int16_t colony_idx);

    void __cdecl Reset_Field_Arrays_();

    void __cdecl Clear_Sys_Disp_Fields_();

    int16_t __cdecl Sum_Colonists_(int16_t colony_idx, int16_t job, int16_t state, int16_t race, int16_t param_5, int16_t param_6);

    void __cdecl Reset_Field_Mode_();

    void __cdecl Drop_Military_();

    void __cdecl Clear_Move_Military_();

    void __cdecl Show_Colony_Screen_();

    void __cdecl Restore_Control_();

    void __cdecl Draw_Current_Building2_(int16_t x, int16_t y, int16_t building_id);
    void __cdecl Draw_Current_Building_(int16_t building_id);
    void __cdecl Draw_Current_Ship_(int16_t ship_id);

    extern int16_t _g_colony_n;
    extern s_colors _bright_blue_font_color_array;
    extern s_colors _font_color_array;
    extern s_colors _font_dim_color_array;
    extern s_colors _font_bright_color_array;
    extern s_colors (&_red_font_color_array)[3];

    void __cdecl Set_Colony_Font_To_Blue_(int16_t style, int8_t is_bright);

    void __cdecl Set_Colony_Font_();

    void __cdecl Set_Colony_Font_To_Reg_Blue_(uint8_t font_idx, uint8_t state);

    extern int16_t _galaxy_map_name_field;

    void __cdecl Clear_Galaxy_Map_Fields_();

    extern int32_t _g_count;

    void __cdecl Draw_Planet_Summary_(int16_t x, int16_t y, int16_t planet_index);

    bool __cdecl Colony_Can_Buy_Product_0_(int16_t colony_idx);

    void __cdecl Tested_Colony_Buys_Outright_(int16_t colony_idx);

    s_animation_header* __cdecl Mini_Planet_Anim_(int16_t planet_idx);

    int16_t __cdecl Bldg_Coords_To_Effective_Frame_(int16_t x, int32_t y);

    char* __cdecl Colony_Type_(int32_t colony_idx);

    void __cdecl Set_Colony_Font_To_Red_(uint8_t style, uint8_t color_idx);

    short __cdecl Get_Next_Colony_(short current_idx, short direction);

    void __cdecl Set_Last_Colony_Selected_(int16_t colony_idx, int16_t player_idx);

    void __cdecl Reset_Step_Squish_();

    int16_t __cdecl N_Bldgs_(int16_t colony_idx);

    void __cdecl Draw_Pop_Dark_(int16_t x, int16_t y, int16_t colony_idx, int16_t pop_idx);

    void __cdecl Create_Shadow_Farmer_(int16_t unused);

    s_animation_header* __cdecl Shadow_Farmer_Bitm_();

    void __cdecl Set_Colony_Font_To_Player_(uint8_t font_style, uint8_t player_idx);

    extern s_colors (&_player_font_color_array)[8];
}