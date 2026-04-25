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
    inline int16_t& DAT_00571F00 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F00});
    inline int16_t& _colony_auto_building = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571F78});
    inline int16_t (&_fields)[11] = *reinterpret_cast<int16_t (*)[11]>(std::uintptr_t{0x00571F80});
    inline s_colors& _l_font_color_array =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x0055AB00});
    inline s_colors& _l_font_dim_color_array =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x0055AB08});
    inline int16_t (&_queue_fields)[7] = *reinterpret_cast<int16_t(*)[7]>(std::uintptr_t{0x0055AB20});

    inline uint8_t*& _buffer = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00595388});

    inline s_animation_header*& _bldg_bitm = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00595384});

    inline uint8_t*& _desc_record_ptr = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00571F74});

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

    int16_t __cdecl First_Prod_Colony_Queue_Slot_(int16_t colony_idx, int16_t product_id);

    void __cdecl Delete_From_Colony_Queue_(short colony_idx, short queue_idx);

    void __cdecl Delete_Prod_From_Colony_Queue_(int16_t colony_idx, int16_t product_id);

    void __cdecl Delete_Ship_(short ship_index);

    void __cdecl Squish_Out_Colony_Queue_Slot_(short colony_idx, short queue_idx);

    void __cdecl Clear_Colony_Queue_(int16_t colony_idx);

    void __cdecl Print_App_Description_(uint16_t x, int16_t y, uint16_t width, int16_t app_id);

    void __cdecl Print_Setting_Description_(uint16_t x, int16_t y, uint16_t width, int16_t setting_id);

    void __cdecl Draw_Ship_Design_Info_(int16_t id);

    void __cdecl Draw_Building_Centered_In_Window_(int16_t building_id);

    void __cdecl Get_Setting_Description_String_(char* buffer, int16_t setting_id);

    void __cdecl Get_App_Description_String_(char* dest_str, int16_t app_id);

    void __cdecl Draw_Building_Info_(int16_t building_idx);

    void __cdecl Draw_Ship_Info_(int16_t item);

    void __cdecl Draw_Active_Prod_();

    char* __cdecl Selection_Name_(int16_t id);

    inline s_mouse_list (&_mouse_list_to)[4] = *reinterpret_cast<s_mouse_list(*)[4]>(std::uintptr_t{0x0055AAB0});
    inline s_mouse_list& _mouse_list_what = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x0055AAE0});
    inline s_mouse_list& _mouse_list_which_ship = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x0055AAF0});
    inline uint8_t& _hacky_speed_refitted_ship_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AB14});

    inline void __cdecl Add_Refited_Ship_To_Queue_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050ceb0});
        fn();
    }
    inline uint8_t __cdecl Something_Behind_Settting_() {
        using Fn = uint8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050c570});
        return fn();
    }
    inline void __cdecl Squish_Out_Queue_Slot_(int32_t slot) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050ce60});
        fn(slot);
    }
    inline void __cdecl Do_Draw_Build_Queue_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050bff0});
        fn();
    }
    inline void __cdecl Do_Exit_Screen_Cleanup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050b4d0});
        fn();
    }
    inline void __cdecl Remove_Ships_(int32_t flag) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050b3e0});
        fn(flag);
    }
    inline int16_t __cdecl First_Prod_Queue_Slot_(int16_t prod_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050b1c0});
        return fn(prod_idx);
    }
    inline void __cdecl Delete_From_Queue_(int32_t slot) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050cc50});
        fn(slot);
    }
    inline uint8_t __cdecl Add_To_Queue_(int16_t prod_idx) {
        using Fn = uint8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050c5c0});
        return fn(prod_idx);
    }
    inline void __cdecl Insert_Before_Queue_Slot_(int32_t slot1, int32_t slot2) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050d070});
        fn(slot1, slot2);
    }
    inline void __cdecl Defragment_Queue_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050ce20});
        fn();
    }

    void __cdecl Evaluate_Input_(int16_t* param_1, int16_t* param_2);

    void __cdecl Print_Selection_Centered_(const char* text);
}