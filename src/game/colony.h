#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colors;
struct s_planet_data;
struct s_colony;

namespace COLONY {
    inline int16_t (&_colony_fields)[21] =
        *reinterpret_cast<int16_t(*)[21]>(std::uintptr_t{0x00559E28});

    inline int16_t (&_military_fields)[2] =
        *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x00559E54});

    inline int16_t (&_job_fields)[3] =
        *reinterpret_cast<int16_t(*)[3]>(std::uintptr_t{0x00595E28});

    inline int16_t (&_sys_disp_summ_fields)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x00595E40});

    inline int16_t (&_pop_fields)[42] =
        *reinterpret_cast<int16_t(*)[42]>(std::uintptr_t{0x00595F80});

    inline s_colony_bldg (&_colony_bldgs)[6] =
        *reinterpret_cast<s_colony_bldg(*)[6]>(std::uintptr_t{0x00559E58});

    inline int16_t (&_colony_satellites)[10] =
        *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x0056EBF8});

    inline const char* _RACEICON_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559C88}); // "RACEICON.LBX"
    inline const char* _PLANETS_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559CA8}); // "PLANETS.LBX"
    inline const char* _COLBLDG_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559CE8}); // "COLBLDG.LBX"
    inline const char* _COLSUM_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559D18}); // "COLSUM.LBX"

    inline int16_t& _step_squish = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559DF0});
    inline int16_t& _scanned_pop = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559DF8});

    void __cdecl Init_Colonies_();

    void __cdecl Colony_Screen_();

    inline int16_t& _colony_saved_screen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056EA68});
    inline int16_t& _g_scanned_star = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056EBC8});
    inline int16_t& _g_scanned_planet = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00595E6C});
    inline uint16_t& _planet_display_handle = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00559E20});

    inline int16_t& _orbit_temp =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B22});

    inline int16_t& _ship_bitm_n =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559F78});
    inline int16_t& _bldg_bitm_n =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559F7C});
    inline int32_t& _g_ship_pic_n =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00559FF8});
    inline int16_t& _g_ship_player_n =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559FFC});
    inline void*& _ship_pic_bitm =
        *reinterpret_cast<void**>(std::uintptr_t{0x00595E68});
    inline void*& _ship_bitm =
        *reinterpret_cast<void**>(std::uintptr_t{0x00595F7C});

    inline const char* _COLONY_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559C48}); // "COLONY.LBX"
    inline const char* _COLONY2_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559C58}); // "COLONY2.LBX"
    inline const char* _COLPUPS_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559C98}); // "COLPUPS.LBX"

    inline s_animation_header** _c_anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00559EE8});

    inline uint8_t& _debug_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559C40});
    inline uint8_t& _using_colony_screen_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E24});
    inline uint8_t& _drawing_display = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E10});
    inline uint8_t& _valid_colony = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E14});
    inline uint8_t& _field_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559DF4});

    inline void __cdecl Draw_Bldg_CR_Dark_(int16_t c, int16_t r) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004de110 });
        fn(c, r);
    }

    inline int16_t& _scanned_bldg_C = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559E04});
    inline int16_t& _scanned_bldg_R = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559E08});
    inline uint8_t& _full_draw = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E18});
    inline int32_t& _colony_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595E50});
    inline int32_t& _planet_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595FD8});
    inline int32_t& _star_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595E58});
    inline int32_t& _orbit_n = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595E78});
    inline uint8_t& _cheats = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E1C});
    inline int32_t& _sys_disp_star_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595FD4});

    void __cdecl Load_Colony_Screen_Seg_();
    void __cdecl Load_Planet_Screen_();
    s_animation_header* __cdecl C_Anims_(int16_t index);
    inline s_animation_header* __cdecl Colony_Pop_Anim_(int16_t colony_idx, int16_t pop_idx) {
        using Fn = s_animation_header*(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004DC3C0})(colony_idx, pop_idx);
    }
    void __cdecl Update_Colony_Palette_();
    void __cdecl Make_Bldg_Array_For_Colony_(int16_t colony_handle, uint8_t flag);
    void __cdecl Reset_Mouse_Picture_();
    inline void __cdecl Clear_Mouse_Picture_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e1c90});
        fn();
    }

    void __cdecl Load_Ship_Bitm_();

    void __cdecl Load_Bldg_Bitm_();

    void __cdecl Load_Officer_Anim_();

    void __cdecl Load_Galaxy_Map_Anims_();

    char* __cdecl Real_Building_Name_(int16_t building_id);

    inline void __cdecl Remap_Draw_Ship_Seg_N_Centered_(int x, int y, int seg_id) {
        using Fn = void(__cdecl*)(int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dba50});
        fn(x, y, seg_id);
    }

    s_animation_header* __cdecl Spy_Anim_(int16_t race_index);

    s_animation_header* __cdecl People_Anim_(int16_t param_1, int16_t param_2, int16_t param_3);

    s_animation_header* __cdecl Prod_Anims_(int16_t param_1, int16_t param_2, int8_t param_3);

    int16_t __cdecl Satellite_Anim_Pic_(int16_t id);

    s_animation_header* __cdecl Satellite_Anim_(int16_t colony_id);

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
    void __cdecl Clear_Colony_Sattelites_();

    int16_t __cdecl N_Houses_In_Bldg_Array_();

    void __cdecl Find_Replacement_Slot_For_Building_(int16_t bldg_id, int16_t* out_row, int16_t* out_col);

    inline int32_t __cdecl CR_To_XY_(int16_t col, int16_t row, int32_t axis) {
        using Fn = int32_t(__cdecl*)(int16_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dd760});
        return fn(col, row, axis);
    }

    void __cdecl Insert_Bldg_Into_Array_(int16_t num_houses_delta, int16_t bldg_id, int16_t bldg_type, int8_t use_back_to_front);

    void __cdecl Sort_Bldg_Array_Columns_();

    bool __cdecl Get_Bldg_CR_(int16_t building_id, int16_t* row_out, int16_t* col_out);

    void __cdecl Bldg_Back_To_Front_(int16_t building_type, uint16_t* out_val1, uint16_t* out_val2);

    void __cdecl Draw_Colony_Bldg_(int16_t col, int16_t row);

    void __cdecl Draw_Colony_Bldgs_();

    inline void __cdecl Draw_Colony_Satellites_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004df450});
        fn();
    }

    inline void __cdecl Draw_Colony_Info_Background_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004de3f0});
        fn();
    }

    inline void __cdecl Box_Galaxy_Map_Star_(int32_t handle, int32_t mode) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004df620});
        fn(handle, mode);
    }

    void __cdecl Draw_Colony_Info_();

    inline void __cdecl Draw_Colony_Info_Military_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004de750});
        fn();
    }

    inline void __cdecl Draw_Colony_Info_Officer_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004deb20});
        fn();
    }

    inline void __cdecl Draw_Colony_Info_Production_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004de6d0});
        fn();
    }

    inline void __cdecl Draw_Colony_Info_Pop_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004de490});
        fn();
    }

    inline void __cdecl Draw_Colony_Info_Morale_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004decb0});
        fn();
    }

    inline void __cdecl Draw_Info_Name_And_Pop_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004decd0});
        fn();
    }

    inline void __cdecl Draw_Info_Build_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004deb10});
        fn();
    }

    inline void __cdecl Draw_Sys_Disp_Scanned_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004df7b0});
        fn();
    }

    inline void __cdecl Draw_Colony_Info_Scanned_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004defd0});
        fn();
    }

    inline void __cdecl No_Op1_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004df600});
        fn();
    }

    void __cdecl Allocate_Mini_Planet_Cache_();

    void __cdecl Draw_Colony_Screen_();

    inline void __cdecl Update_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004DFF60)();
    }

    inline void __cdecl Do_Informational_And_Decision_Popups_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004E1990)();
    }

    inline uint8_t __cdecl Evaluate_Input_(int16_t* input, int16_t* scanned) {
        using Fn = uint8_t(__cdecl*)(int16_t*, int16_t*);
        return reinterpret_cast<Fn>(0x004E0870)(input, scanned);
    }

    inline void __cdecl Clear_Fake_Buttons_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004B53F0)();
    }

    inline void __cdecl Fix_Colony_Queue_(int32_t handle) {
        using Fn = void(__cdecl*)(int32_t);
        reinterpret_cast<Fn>(0x004DFBD0)(handle);
    }

    inline void __cdecl Col_Calc_Wrapper_(int16_t colony_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dc110});
        fn(colony_idx);
    }

    void __cdecl Reset_Field_Arrays_();

    inline void __cdecl Clear_Sys_Disp_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004df710});
        fn();
    }

    inline uint16_t __cdecl Sum_Colonists_(int32_t colony_idx, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
        using Fn = uint16_t(__cdecl*)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        return reinterpret_cast<Fn>(0x004dd920)(colony_idx, arg2, arg3, arg4, arg5, arg6);
    }

    inline void __cdecl Reset_Field_Mode_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004dc190)();
    }

    void __cdecl Show_Colony_Screen_();

    inline int16_t& _g_colony_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559DEC});
    inline s_colors& _bright_blue_font_color_array =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559F88});
    inline s_colors& _font_color_array = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559F90});
    inline s_colors& _font_bright_color_array = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559FA0});
    inline s_colors (&_red_font_color_array)[2] =
        *reinterpret_cast<s_colors(*)[2]>(std::uintptr_t{0x00559FA8});

    void __cdecl Set_Colony_Font_To_Blue_(int16_t style, uint32_t is_bright);

    void __cdecl Set_Colony_Font_();

    inline int16_t& _galaxy_map_name_field =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00595E5C});

    void __cdecl Clear_Galaxy_Map_Fields_();

    inline uint16_t& _g_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00595E4C});

    void __cdecl Draw_Planet_Summary_(int16_t x, int16_t y, int16_t planet_index);

    bool __cdecl Colony_Can_Buy_Product_0_(int16_t colony_idx);

    void __cdecl Tested_Colony_Buys_Outright_(int16_t colony_idx);

    inline s_animation_header* __cdecl Mini_Planet_Anim_(int16_t id) {
        using Fn = s_animation_header*(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x004e20f0)(id);
    }

    int16_t __cdecl Bldg_Coords_To_Effective_Frame_(int16_t x, int32_t y);
}