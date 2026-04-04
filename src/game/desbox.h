#pragma once

namespace DESBOX {
    inline uint16_t& _weapon_replacement_mods =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00566428});

    inline uint8_t& _incr_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A61E0});

    inline uint16_t& _field_item_scanned = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005663F0});
    inline int16_t& _field_item_chosen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056640C});
    inline uint8_t& _beam_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EB8});
    inline uint8_t& _missile_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EBC});
    inline uint8_t& _bomb_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EC0});
    inline uint8_t& _special_filter_button_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557EC4});
    inline int16_t& _replacement_scroll_dn_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005663D8});
    inline int16_t& _replacement_scroll_up_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056641C});
    inline int16_t& _weapon_arc_rack_box_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566420});
    inline uint16_t& _weapon_replacement_arcs = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00566408});
    inline int16_t& _weapon_replacement_rack = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A61E2});
    inline uint8_t& _wait_ctr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566400});
    inline uint8_t* _valid_mods = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663E0});
    inline uint8_t& DAT_005663e1 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663E1});
    inline uint8_t& DAT_005663e2 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663E2});
    inline uint8_t* _valid_arcs_racks = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663F8});
    inline uint8_t& DAT_005663f9 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663F9});

    inline void __cdecl Weapon_Choices_(int16_t player_num) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004935D0})(player_num);
    }
    inline void __cdecl Space_With_Weapons_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00492FF0})();
    }
    inline void __cdecl Add_Replacement_Weapon_Fields_(int16_t* field_ok, int16_t* field_cancel) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00491100})(field_ok, field_cancel);
    }
    inline void __cdecl Draw_Weapons_Replacement_Box_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004918B0})();
    }
    inline void __cdecl Get_Weapons_Box_Base_Dimensions_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00492020})();
    }
    inline void __cdecl Set_Up_Weapon_Display_(int16_t slot) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00492D30})(slot);
    }
    inline void __cdecl Weapon_Replacement_Help_(int16_t slot) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004939B0})(slot);
    }
    inline int16_t __cdecl Weapon_Index_(int16_t field_item) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00493990})(field_item);
    }
    inline void __cdecl Print_Weapon_Mod_Help_Description_(int16_t mod_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00492810})(mod_idx);
    }
    inline int16_t __cdecl Scan_Replacement_Fields_(int16_t scanned_input, int16_t input, int16_t* scanned_item, int16_t* scanned_weapon_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00492C50})(scanned_input, input, scanned_item, scanned_weapon_idx);
    }
    inline void __cdecl Update_Weapon_Replacement_(int16_t* done_flag, int16_t item_chosen, int16_t slot) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00493140})(done_flag, item_chosen, slot);
    }
    inline void __cdecl Print_Weapon_Modification_Message_(int16_t mod_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00492980})(mod_idx);
    }
    inline int16_t __cdecl Update_Field_Item_Chosen_(int16_t prev_filter) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004930A0})(prev_filter);
    }
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

    inline void __cdecl Generic_Replacement_Box_Choices_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048E690});
        fn();
    }

    inline void __cdecl Generic_Replacement_Box_Dimensions_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048E820});
        fn();
    }

    inline void __cdecl Add_Generic_Replacement_Fields_(int16_t* field_1, int16_t* field_2, int16_t* field_3) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048E190});
        fn(field_1, field_2, field_3);
    }

    inline void __cdecl Draw_Generic_Replacement_Box_Help_(int32_t input, int32_t field_1, int32_t field_2, int32_t field_3) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048E380});
        fn(input, field_1, field_2, field_3);
    }

    inline void __cdecl Redraw_Main_Design_Screen_To_Background_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048F0E0});
        fn();
    }

    void __cdecl Weapons_Replacement_Box_();

    inline void __cdecl Special_Systems_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048f4d0});
        fn();
    }
    uint32_t __cdecl Application_Type_(int16_t app_id);

    void __cdecl Update_Weapon_Modification_Data_();
}