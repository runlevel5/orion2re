#pragma once

namespace FLT1 {
    void __cdecl Draw_Box_Around_Scanned_Ship_(int16_t icon_idx);

    void __cdecl Draw_Fltscrn_Big_Ship_Icons_(int16_t param_1);

    int16_t __cdecl First_Selected_Ship_();

    int16_t __cdecl F_Ship_Icon_From_Node_Ptr_(int16_t node_idx);

    void __cdecl Draw_Fltscrn_Ship_Destination_Lines_(int16_t param_1);

    void __cdecl Draw_Fltscrn_Small_Ship_Icons_();

    void __cdecl Draw_Flashing_Small_Ship_Icon_();

    void __cdecl Fill_FltScrn_Scroll_Bar_();

    void __cdecl Fleet_Screen_();

    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* sb);

    int16_t __cdecl Scan_Small_Ship_Icons_(int16_t param_1, int16_t param_2, int16_t* param_3, int16_t* param_4);

    void __cdecl Allocate_Fltscrn_Next_Screen_Seg_();

    void __cdecl Set_Small_Ship_Stack_Pointer_(bool check_box);

    void* __cdecl Load_Fltscrn_Officer_Picture_(int16_t player_num, int16_t leader_idx, void* buffer, int16_t reload_flag);

    void __cdecl Reload_Small_Ship_Icons_();

    void __cdecl Load_Galaxy_Stars_();

    void __cdecl Reload_Fleet_Screen_();

    void Draw_Fleet_Screen_();

    void __cdecl Add_Fleet_Screen_Fields_(int16_t* relocate_fleet_field_id, int16_t* scrap_button_field_id,
                                          int16_t* fleet_return_button_field_id, int16_t* up_scroll_button_field_id,
                                          int16_t* down_scroll_button_field_id, int16_t* next_fleet_button_field_id,
                                          int16_t* prev_fleet_button_field_id, int16_t* officers_button_field_id,
                                          int16_t* all_button_field_id, int16_t* big_icon_field_ids);

    inline bool At_Least_One_Officer_(int16_t player_num) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B2AE0});
        return fn(player_num);
    }

    inline const char* const s_empty = reinterpret_cast<const char*>(0x00551100);
    inline const char* const s_plus = reinterpret_cast<const char*>(0x00551118);
    inline const char* const s_minus = reinterpret_cast<const char*>(0x0055111C);
    inline const char* const s_space = reinterpret_cast<const char*>(0x00551120);
    inline const char* const s_C = reinterpret_cast<const char*>(0x00552420);
    inline const char* const s_L = reinterpret_cast<const char*>(0x005560E8);
    inline const char* const s_dot = reinterpret_cast<const char*>(0x0055632C);
    inline const char* const s_comma = reinterpret_cast<const char*>(0x005569C8);
    inline const char* const s_S = reinterpret_cast<const char*>(0x005569CC);
    inline const char* const s_A = reinterpret_cast<const char*>(0x00557ED4);
    inline const char* const s_R = reinterpret_cast<const char*>(0x0055824C);
    inline const char* const s_U = reinterpret_cast<const char*>(0x00558540);

    void __cdecl Cancel_Star_Relocation_(int16_t star_id);

    void __cdecl Star_Relocation_(int16_t* star_from_ptr, int16_t* star_to_ptr, int16_t star_idx);

    inline bool __cdecl Okay_To_Set_Relocate_Star2_(int16_t star_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b2c20});
        return fn(star_idx);
    }

    inline bool __cdecl Okay_To_Set_Relocate_To_Star_(int16_t star_from, int16_t star_to) {
        using Fn = bool(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b2c50});
        return fn(star_from, star_to);
    }

    void __cdecl Cant_Order_Ship_Message_(const s_ship_data* ship);

    inline void __cdecl Set_Fltscrn_Big_Icons_(int16_t val) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004b2980 })(val);
    }

    void __cdecl Build_Scrap_List_(int16_t* scrap_list, int16_t* list_count);

    inline uint8_t __cdecl Scrap_Ships_(int16_t player_num, int16_t* list, int16_t count, int16_t flag) {
        using Fn = uint8_t(__cdecl*)(int16_t, int16_t*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004b2440 })(player_num, list, count, flag);
    }

    int16_t __cdecl N_Fltscrn_Ships_Selected_();

    void __cdecl Next_Ship_Icon_();

    void __cdecl Previous_Ship_Icon_();

    int16_t __cdecl Update_Selection_Flags_();

    inline uint8_t& _relocate_button_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CD6C});
    inline int16_t& _fleet_screen_bad_ship_id = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CD64});
    inline int16_t& _combat_filter_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CD68});
    inline int16_t& _n_player_ships = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CD70});
    inline int16_t& _support_filter_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CD74});
    inline void*& _multi_button_support_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0056CD78});
    inline uint8_t& _merging_relocations = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CD7C});
    inline void*& _multi_button_combat_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0056CD80});
    inline s_mouse_list* _mouse_list_from = reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005584B0});
    inline s_mouse_list* _mouse_list_to = reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005584E0});
    inline s_mouse_list* _mouse_list_all_to = reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00558510});
    inline const char* PTR_s_FLEET_LBX = "fleet.lbx";
}