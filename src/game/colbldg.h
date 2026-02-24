#pragma once
#include <cstdint>

struct s_colors;
struct s_ship_design;

namespace COLBLDG {
    inline int16_t* _building_indexes = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AB30});
    inline int16_t& _n_building_indexes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AB9C});
    // Size: short[54]
    inline int16_t* _building_fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F08});

    int __cdecl qsort_building_by_name_(const void* a, const void* b);

    int16_t __cdecl Calc_Font_Off_();

    void __cdecl Get_Font_Off_For_Buildings_(int16_t* offset);

    void __cdecl Get_Font_Off_For_Military_(int16_t* font_offset);

    void __cdecl Calculate_Building_Array_();

    void __cdecl Calculate_Military_Array_();

    int16_t __cdecl Star_N_Uncolonized_Worlds_(int16_t star_index);

    int16_t __cdecl N_Prods_In_Queue_(int16_t product_id);

    bool __cdecl Has_Prod_In_Queue_(int16_t building_id);

    void __cdecl Draw_Icon_(s_animation_header* anim);

    inline int16_t& _n_military_indexes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC0C});
    inline int16_t* _military_indexes = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055ABA0});
    inline int16_t (&_military_fields)[54] = *reinterpret_cast<int16_t(*)[54]>(std::uintptr_t{0x00571E70});

    void __cdecl Build_Arrays_();

    int16_t __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx);

    void __cdecl Build_Queue_Popup_();

    inline uint8_t& _return_to_build_popup_from_custom_design = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB10});
    inline uint8_t& _field_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB18});
    inline int16_t& _active_prod = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AC10});
    inline int16_t (&_current_item)[7] = *reinterpret_cast<int16_t (*)[7]>(std::uintptr_t{0x0055AC18});
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

    inline void*& _bldg_bitm = *reinterpret_cast<void**>(std::uintptr_t{0x00595384});

    inline void*& _desc_record_ptr = *reinterpret_cast<void**>(std::uintptr_t{0x00571F74});

    inline s_animation_header* (&_anims)[7] =
        *reinterpret_cast<s_animation_header* (*)[7]>(std::uintptr_t{0x00571EE0});
    inline s_animation_header*& _colsum_pal_anim = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00595380});

    void __cdecl Load_Build_Anims_();

    void __cdecl Clear_Last_App_Description_String_();

    void __cdecl Add_Build_Queue_Fields_();

    void __cdecl Add_Buildings_Fields_();

    void __cdecl Add_Militarys_Fields_();

    void __cdecl Clear_Last_App_Desc_();

    void __cdecl Update_Fields_();

    void __cdecl Draw_Build_Queue_Popup_();

    void __cdecl Draw_Buildings_List_();

    void __cdecl Draw_Militarys_List_();

    int16_t __cdecl Auto_Design_Type_(int16_t design_code);

    void __cdecl Draw_Build_Queue_();

    void __cdecl Draw_Current_Selection_();

    void __cdecl Draw_Cost_And_Time_Info_(int16_t colony_idx, int16_t product_idx, int16_t* line_count);

    void __cdecl Draw_Ship_Design_Info2_(s_ship_design* design);

    inline void __cdecl Build_Ship_Design_Info_Formatted_Paragraph_(s_ship_design* design, char* buffer, int unknown) {
        using Fn = void(__cdecl*)(s_ship_design*, char*, int);
        reinterpret_cast<Fn>(std::uintptr_t{0x0050a5c0})(design, buffer, unknown);
    }

    void __cdecl Delete_From_Colony_Queue_(short colony_idx, short queue_idx);

    void __cdecl Delete_Ship_(short ship_index);

    inline void __cdecl Squish_Out_Colony_Queue_Slot_(short colony_idx, short queue_idx) {
        using Fn = void(__cdecl*)(short, short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050B360});
        fn(colony_idx, queue_idx);
    }

    void __cdecl Clear_Colony_Queue_(int16_t colony_idx);

    void __cdecl Print_App_Description_(uint16_t x, int16_t y, uint16_t width, int16_t app_id);

    void __cdecl Print_Setting_Description_(uint16_t x, int16_t y, uint16_t width, int16_t setting_id);

    void __cdecl Draw_Ship_Design_Info_(int16_t id);

    void __cdecl Draw_Building_Centered_In_Window_(int16_t building_id);

    void __cdecl Get_Setting_Description_String_(char* buffer, int16_t setting_id);

    void __cdecl Draw_Building_Info_(int16_t building_idx);

    void __cdecl Draw_Ship_Info_(int16_t item);

    void __cdecl Draw_Active_Prod_();

    char* __cdecl Selection_Name_(int16_t id);

    inline void*& _mouse_list_to = *reinterpret_cast<void**>(std::uintptr_t{0x0055AAB0});

    void __cdecl Print_Selection_Centered_(const char* text);
}