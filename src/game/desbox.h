#pragma once

namespace DESBOX {
    inline uint16_t& _weapon_replacement_mods =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00566428});

    inline uint8_t& _incr_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A61E0});

    inline uint16_t& _field_item_scanned = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005663F0});
    inline int16_t& _field_item_chosen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056640C});
    inline int16_t& _replacement_box_middle_height =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566410});
    inline int16_t& _replacement_scroll_field =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005663F4});
    inline int16_t& _replacement_box_width =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566404});
    inline int16_t& _replacement_box_base_y =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566414});
    inline int16_t& _replacement_box_base_x =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566418});
    inline int16_t& _replacement_box_height =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566424});
    inline uint8_t& _beam_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EB8});
    inline uint8_t& _missile_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EBC});
    inline uint8_t& _bomb_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EC0});
    inline uint8_t& _special_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EC4});
    inline int16_t& _replacement_scroll_dn_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005663D8});
    inline int16_t& _replacement_scroll_up_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056641C});
    inline int16_t& _weapon_arc_rack_box_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566420});
    inline int16_t& _weapon_replacement_arcs = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566408});
    inline uint16_t& _weapon_replacement_rack = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A61E2});
    inline uint8_t& _wait_ctr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566400});
    inline uint8_t (&_valid_mods)[15] = *reinterpret_cast<uint8_t(*)[15]>(std::uintptr_t{0x005663E0});
    inline uint8_t* _valid_arcs_racks = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663F8});

    int16_t __cdecl Weapon_Choices_(int16_t player_num);

    bool __cdecl Have_Tractor_Beam_Weapon_();

    void __cdecl Set_Up_Weapon_Scroll_Bar_();
    void __cdecl Space_With_Weapons_();
    inline int16_t __cdecl Space_With_Weapon_(int16_t weapon_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048f140});
        return fn(weapon_idx);
    }
    void __cdecl Add_Replacement_Weapon_Fields_(int16_t* cancel_btn_field, int16_t* fullscreen_field);
    void __cdecl Draw_Weapons_Replacement_Box_();
    void __cdecl Get_Weapons_Box_Base_Dimensions_();
    void __cdecl Set_Up_Weapon_Display_(int16_t weapon_index);
    void __cdecl Weapon_Replacement_Help_(int16_t weapon_idx);
    inline int16_t __cdecl Weapon_Index_(int16_t filter_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00493990});
        return fn(filter_idx);
    }
    void __cdecl Print_Weapon_Mod_Help_Description_(int16_t mod_id);
    int16_t __cdecl Scan_Replacement_Fields_(int16_t field_id, int16_t target_field, int16_t* out_field_index, int16_t* out_visible_row);
    void __cdecl Update_Weapon_Replacement_(int16_t* exit_flag, int16_t weapon_choice, int16_t slot_idx);
    inline void __cdecl Set_New_Weapon_Modifications_(int16_t* weapon_specials) {
        using Fn = void(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004933b0});
        fn(weapon_specials);
    }
    void __cdecl Print_Weapon_Modification_Message_(int16_t mod_idx);
    int16_t __cdecl Update_Field_Item_Chosen_(int16_t weapon_idx);
    inline int __cdecl qsort_by_research_cost_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00493A00})(a, b);
    }

    void __cdecl Generic_Replacement_Box_();

    void __cdecl Draw_Darkened_Design_Screen_();

    inline void __cdecl Draw_Generic_Replacement_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048E580});
        fn();
    }

    void __cdecl Generic_Replacement_Box_Choices_();

    void __cdecl Generic_Replacement_Box_Dimensions_();

    void __cdecl Add_Generic_Replacement_Fields_(int16_t* cancel_btn_field, int16_t* bg_field, int16_t* top_box_field);

    void __cdecl Draw_Generic_Replacement_Box_Help_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4);

    void __cdecl Redraw_Main_Design_Screen_To_Background_();

    void __cdecl Weapons_Replacement_Box_();

    inline void __cdecl Special_Systems_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048f4d0});
        fn();
    }
    int8_t __cdecl Application_Type_(int16_t tech_app_id);

    void __cdecl Update_Weapon_Modification_Data_();

    int32_t __cdecl New_Mod_Space_(uint8_t mod_bit_index);

    int16_t __cdecl Valid_Modifications_(int8_t* mod_status_list);

    inline void __cdecl Print_Main_Weapon_Box_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00492170});
        fn(x, y);
    }
    void __cdecl Draw_Weapons_Scroll_Bar_();
    void __cdecl Draw_Weapons_Rack_Box_(int16_t x, int16_t y);
    void __cdecl Draw_Weapons_Arc_Box_(int16_t x, int16_t y);
    void __cdecl Draw_Weapons_Mod_Box_(int16_t x, int16_t y);

    inline void __cdecl Clear_Weapon_Replacement_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00491850)();
    }

    int __cdecl New_Arc_Space_(uint8_t arc_index);

    inline void __cdecl Get_Weapon_Mod_Field_XYs_(int16_t* unused, int16_t* x_coords, int16_t* y_coords) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t*);
        reinterpret_cast<Fn>(0x00491E30)(unused, x_coords, y_coords);
    }

    inline bool __cdecl Mod_Allowed_On_Weapon_Type_(uint8_t mod_idx) {
        using Fn = bool(__cdecl*)(uint8_t);
        return reinterpret_cast<Fn>(0x004920B0)(mod_idx);
    }

    inline bool __cdecl Should_Add_Mod_(int16_t mod_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x00492FC0)(mod_idx);
    }

    inline int16_t __cdecl Weapon_Choice_Valid_(char* msg_buf) {
        using Fn = int16_t(__cdecl*)(char*);
        return reinterpret_cast<Fn>(0x00493530)(msg_buf);
    }

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100}); // ""
    inline const char* s_plus = reinterpret_cast<const char*>(std::uintptr_t{0x00551118}); // "+"
    inline const char* s_minus = reinterpret_cast<const char*>(std::uintptr_t{0x0055111c}); // "-"
    inline const char* s_space = reinterpret_cast<const char*>(std::uintptr_t{0x00551120}); // " "
    inline const char* s__00551120 = reinterpret_cast<const char*>(std::uintptr_t{0x00551120});
    inline const char* s_B = reinterpret_cast<const char*>(std::uintptr_t{0x005560e4}); // "B"
    inline const char* s_S = reinterpret_cast<const char*>(std::uintptr_t{0x005569cc}); // "S"
    inline const char* s_O = reinterpret_cast<const char*>(std::uintptr_t{0x00556afc}); // "O"
    inline const char* s_A = reinterpret_cast<const char*>(std::uintptr_t{0x00557ed4}); // "A"
    inline const char* s_M = reinterpret_cast<const char*>(std::uintptr_t{0x00557ed8}); // "M"
}