#pragma once
#include <cstdint>

struct s_colors;

namespace COLBLDG {
    inline int16_t* _building_indexes = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AB30});
    inline int16_t& _n_building_indexes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AB9C});
    // Size: short[54]
    inline int16_t* _building_fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F08});

    int __cdecl qsort_building_by_name_(const void* a, const void* b);

    int16_t __cdecl Calc_Font_Off_();

    inline void __cdecl Get_Font_Off_For_Buildings_(int16_t* offset) {
        using Fn = void(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00509CA0});
        fn(offset);
    }

    void __cdecl Calculate_Building_Array_();

    void __cdecl Calculate_Military_Array_();

    int16_t __cdecl Star_N_Uncolonized_Worlds_(int16_t star_index);

    int16_t __cdecl N_Prods_In_Queue_(int16_t product_id);

    inline bool __cdecl Has_Prod_In_Queue_(int16_t building_id) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00509750});
        return fn(building_id);
    }

    inline int16_t& _n_military_indexes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC0C});
    inline int16_t* _military_indexes = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055ABA0});
    inline int16_t (&_military_fields)[54] = *reinterpret_cast<int16_t(*)[54]>(std::uintptr_t{0x00571E70});

    void __cdecl Build_Arrays_();

    int16_t __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx);

    void __cdecl Build_Queue_Popup_();

    inline uint8_t& _return_to_build_popup_from_custom_design = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB10});
    inline uint8_t& _field_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB18});
    inline int16_t& _active_prod = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC10});
    inline int16_t* _current_item = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC18});
    inline int16_t& _last_app = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC28});
    inline int16_t& _last_setting = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC2C});
    inline int16_t& _scanned_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC30});
    inline char (&_g_msg)[800] = *reinterpret_cast<char (*)[800]>(std::uintptr_t{0x00571F98});
    inline int16_t& _colony_bldg_saved_screen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571EDC});
    inline int16_t& _scanned_prod = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571EFC});
    inline int16_t& _colony_auto_building = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F78});
    inline int16_t (&_fields)[11] = *reinterpret_cast<int16_t (*)[11]>(std::uintptr_t{0x00571F80});
    inline s_colors& _l_font_color_array =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x0055AB00});
    inline s_colors& _l_font_dim_color_array =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x0055AB08});
    inline int16_t (&_queue_fields)[7] = *reinterpret_cast<int16_t(*)[7]>(std::uintptr_t{0x0055AB20});

    inline void*& _buffer = *reinterpret_cast<void**>(std::uintptr_t{0x00595388});

    inline s_animation_header** _anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00571EE0});
    inline s_animation_header*& _colsum_pal_anim = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00595380});

    void __cdecl Clear_Last_App_Description_String_();

    void __cdecl Add_Build_Queue_Fields_();

    void __cdecl Add_Buildings_Fields_();

    void __cdecl Add_Militarys_Fields_();

    void __cdecl Clear_Last_App_Desc_();

    void __cdecl Update_Fields_();

    void __cdecl Draw_Build_Queue_Popup_();

    void __cdecl Draw_Buildings_List_();

    void __cdecl Draw_Militarys_List_();

    inline void __cdecl Get_Font_Off_For_Military_(int16_t* off) {
        using Fn = void(__cdecl*)(int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00509FA0})(off);
    }

    inline int16_t __cdecl Auto_Design_Type_(int16_t mil_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0050B090})(mil_idx);
    }

    void __cdecl Draw_Build_Queue_();

    void __cdecl Draw_Current_Selection_();

    inline void __cdecl Draw_Cost_And_Time_Info_(int32_t colony, int16_t item, int16_t* y_offset) {
        using Fn = void(__cdecl*)(int32_t, int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050dca0});
        fn(colony, item, y_offset);
    }

    inline void __cdecl Print_App_Description_(int x, int y, int str_idx, int app_id) {
        using Fn = void(__cdecl*)(int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050a290});
        fn(x, y, str_idx, app_id);
    }

    inline void __cdecl Draw_Ship_Design_Info_(int16_t id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050abe0});
        fn(id);
    }

    inline void __cdecl Draw_Building_Centered_In_Window_(int id) {
        using Fn = void(__cdecl*)(int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050a370});
        fn(id);
    }

    inline void __cdecl Print_Setting_Description_(int x, int y, int str_idx, int setting_id) {
        using Fn = void(__cdecl*)(int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050dc10});
        fn(x, y, str_idx, setting_id);
    }

    inline void __cdecl Draw_Building_Info_(int16_t item) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050a1b0});
        fn(item);
    }

    inline void __cdecl Draw_Ship_Info_(int16_t item) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050ad80});
        fn(item);
    }

    inline void __cdecl Draw_Active_Prod_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050ae50});
        fn();
    }

    char* __cdecl Selection_Name_(int16_t id);

    inline void __cdecl Print_Selection_Centered_(const char* name) {
        using Fn = void(__cdecl*)(const char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0050DC80})(name);
    }
}