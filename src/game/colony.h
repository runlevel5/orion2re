#pragma once
#include <cstdint>

struct s_animation_header;

namespace COLONY {
    inline s_colony_bldg (&_colony_bldgs)[6] =
        *reinterpret_cast<s_colony_bldg(*)[6]>(std::uintptr_t{0x00559E58});

    inline int16_t (&_colony_satellites)[10] =
        *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x0056EBF8});

    inline const char* _RACEICON_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559C88}); // "RACEICON.LBX"
    inline const char* _PLANETS_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00559CA8}); // "PLANETS.LBX"

    inline int16_t& _step_squish = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559DF0});

    void __cdecl Init_Colonies_();

    inline void __cdecl Colony_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004dfc10})();
    }

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

    inline uint8_t& _using_colony_screen_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E24});

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
    void __cdecl Update_Colony_Palette_();
    void __cdecl Make_Bldg_Array_For_Colony_(int16_t colony_handle, uint8_t flag);
    void __cdecl Reset_Mouse_Picture_();

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

    inline void __cdecl Draw_Building_With_Bottom_Centered_(int32_t x, int32_t y, int16_t building_id, int32_t param_4, int32_t param_5) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int16_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ddab0});
        fn(x, y, building_id, param_4, param_5);
    }

    inline void __cdecl Remap_Draw_Ship_Centered_(int x, int y, int index) {
        using Fn = void(__cdecl*)(int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004db8c0});
        fn(x, y, index);
    }

    int16_t __cdecl Calculate_Colony_Turn_Count_From_Scrap_For_Prod_(int16_t colony_idx, int16_t product_id);

    int16_t __cdecl Calculate_Current_Production_Turn_Count_(int16_t colony_idx);

    void __cdecl Clear_Colony_Buidlings_();
    void __cdecl Clear_Colony_Sattelites_();

    inline void __cdecl Insert_Bldg_Into_Array_(int16_t colony_idx, int16_t bldg_id, int16_t count, uint8_t is_ui_view) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dd340});
        fn(colony_idx, bldg_id, count, is_ui_view);
    }

    inline void __cdecl Sort_Bldg_Array_Columns_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dd1f0});
        fn();
    }

    bool __cdecl Get_Bldg_CR_(int16_t building_id, int16_t* row_out, int16_t* col_out);

    inline void __cdecl Bldg_Back_To_Front_(int16_t i, uint16_t* col, uint16_t* row) {
        using Fn = void(__cdecl*)(int16_t, uint16_t*, uint16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ddda0});
        fn(i, col, row);
    }

    inline void __cdecl Draw_Colony_Bldg_(uint16_t col, uint16_t row) {
        using Fn = void(__cdecl*)(uint16_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004de000});
        fn(col, row);
    }

    void __cdecl Draw_Colony_Bldgs_();
}