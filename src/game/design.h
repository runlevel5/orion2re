#pragma once
#include <cstdint>

namespace DESIGN {
    inline s_current_design*& _design = *reinterpret_cast<s_current_design**>(std::uintptr_t{ 0x0059B330 });
    inline uint8_t& _replacement_wait_ctr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A7B6E});
    inline int16_t& _design_size_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565040});
    inline uint8_t& _use_augmented_engines = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056503C});

    inline uint32_t& _printed_space_avail = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565054});
    inline int16_t& _design_shield_replacement_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056505C});
    inline int16_t& _design_computer_replacement_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565060});
    inline uint16_t& _printed_cost = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565058});
    inline uint8_t& _changed_design_name = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056504C});
    inline int16_t& _player_design_num = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B28});
    inline uint8_t* _add_size_button_flag = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565064});

    inline const char* s_cstdes_log = reinterpret_cast<const char*>(0x005560ec);
    inline const char* s_plus = reinterpret_cast<const char*>(0x00551118); // "+"
    inline const char* s_minus = reinterpret_cast<const char*>(0x0055111C); // "-"
    inline const char* s_pct_d = reinterpret_cast<const char*>(0x00556104); // "%d"
    inline const char* s_pct_s_pct_d = reinterpret_cast<const char*>(0x00556134); // "%s%d"
    inline const char* s_pct_d_pct = reinterpret_cast<const char*>(0x0055612C); // "%d%%"
    inline const char* s_pct_s_bracket_pct_d = reinterpret_cast<const char*>(0x00556124); // "%s (%d)"
    inline const char* s_special_tag = reinterpret_cast<const char*>(0x0055611C); // "special"
    inline const char*& PTR_s_DESIGN_LBX_005560D8 = *reinterpret_cast<const char**>(std::uintptr_t{0x005560D8}); // "DESIGN.LBX"
    inline s_animation_header*& _dull_plus_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00565044});
    inline s_animation_header*& _dull_minus_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00565048});
    inline s_animation_header*& _dark_build_design_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00565050});

    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, struct s_ship_design* ship_design);

    uint8_t __cdecl Current_Design_Size_(int16_t player_idx, int16_t design_idx);

    int8_t __cdecl Count_Weapons_Loaded_();

    int8_t __cdecl Get_Modified_Combat_Speed_(s_current_design* design);

    int8_t __cdecl Get_Base_Combat_Speed_(s_current_design* design);

    int8_t __cdecl Get_Ship_Speed_Modifier_(s_current_design* design);

    int8_t __cdecl Current_Design_Base_Combat_Speed_(s_current_design* design);

    uint8_t __cdecl Current_Design_Has_Special_System_(s_current_design* design, uint32_t system_id);

    void __cdecl Has_Damper_Field_();

    uint8_t __cdecl Current_Design_Shield_();

    int16_t __cdecl Base_Shield_Cost();

    int16_t __cdecl Get_Shild_Space_();

    void __cdecl Design_Template_Costs_(int16_t player_id);

    void __cdecl Design_Template_Space_Requirements_(int16_t player_id);

    int32_t __cdecl Total_Design_Cost_();

    int32_t __cdecl Weapon_Space_(int16_t player_idx, int16_t weapon_idx, int16_t quantity, uint16_t mod_flags, uint16_t arc_type, uint16_t ship_size);

    int32_t __cdecl Total_Space_Used_();

    int32_t __cdecl Get_Design_Hull_Size_();

    int8_t __cdecl Count_Special_Devices_();

    void __cdecl Update_N_Spec_Systems_In_Design_();

    void __cdecl Update_Weapon_Count_();

    void __cdecl Update_Calculated_Design_Data_(int16_t player_idx);

    void __cdecl Design_Screen_();

    int32_t __cdecl Get_Design_Hull_Size_();

    void __cdecl Draw_Design_Screen_();

    void __cdecl Draw_Down_Buttons_();

    void __cdecl Print_Current_Design_();

    inline void __cdecl Check_For_Auto_Repeat_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042F4A0});
        fn();
    }

    void __cdecl Load_Design_Screen_Pictures_();

    inline void __cdecl Get_Tech_Description_Lbx_Name_(char* buffer) {
        using Fn = void(__cdecl*)(char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00432D00})(buffer);
    }

    void __cdecl Build_Initial_Design_Template_(int16_t player_idx, int16_t design_idx);

    inline void __cdecl Add_Design_Buttons_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042d080 })();
    }

    void __cdecl Clear_Design_();

    void __cdecl Update_Player_Design_(int16_t player_idx, int16_t design_idx);

    inline void __cdecl Check_Generic_Replacement_Fields_(int16_t field, int16_t* refresh, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042d930 })(field, refresh, flag);
    }

    inline void __cdecl Check_Weapon_Fields_(int16_t field, int16_t* refresh, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042dda0 })(field, refresh, flag);
    }

    inline void __cdecl Check_Special_System_Fields_(int16_t field, int16_t* refresh, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042dc20 })(field, refresh, flag);
    }

    inline void __cdecl Check_Plus_Minus_Buttons_(int16_t field) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042dad0 })(field);
    }

    inline void __cdecl Get_Ship_Design_Name_(uint16_t race, int16_t size, int picture_idx, char* out_name) {
        using Fn = void(__cdecl*)(uint16_t, int16_t, int, char*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042f830 })(race, size, picture_idx, out_name);
    }

    inline void __cdecl Get_Ship_Size_Button_Field_XY_(int index, int16_t* x, int16_t* y, int16_t* w, int16_t* h) {
        using Fn = void(__cdecl*)(int, int16_t*, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0042f8b0 });
        fn(index, x, y, w, h);
    }

    inline void __cdecl Build_Design_Template_(s_current_design* design) {
        using Fn = void(__cdecl*)(s_current_design*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004315d0 })(design);
    }

    inline void __cdecl Set_Flashing_Colors_(int style, int color_idx) {
        using Fn = void(__cdecl*)(int, int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00431a50 })(style, color_idx);
    }

    inline int16_t __cdecl Current_Design_Has_Special_System_Count_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x004317a0 })();
    }

    inline int16_t __cdecl Current_Design_Armor_Absorption_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00431700 })();
    }

    inline int16_t __cdecl Shield_Strength_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00431be0 })();
    }

    inline int16_t __cdecl Current_Shield_Damage_Points_Blocked_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00431760 })();
    }

    inline void __cdecl Weapon_Mod_String_(int16_t specials, char* out, int p3) {
        using Fn = void(__cdecl*)(int16_t, char*, int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00432ac0 })(specials, out, p3);
    }

    inline void __cdecl Weapon_Arc_String_(int16_t arc, char* out) {
        using Fn = void(__cdecl*)(int16_t, char*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00432960 })(arc, out);
    }

    inline int16_t __cdecl Weapon_Needs_Racks_(int16_t type) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00432720 })(type);
    }

    inline void __cdecl Design_Weapon_Damage_String_(int16_t weapon_idx, char* out, int p3, int p4) {
        using Fn = void(__cdecl*)(int16_t, char*, int, int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042f170 })(weapon_idx, out, p3, p4);
    }

    inline bool __cdecl Space_For_Weapon_Replacement_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00431dd0 })();
    }

    inline bool __cdecl Space_For_System_Replacement_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00431c50 })();
    }

    inline int16_t __cdecl Printed_Design_Cost_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x004317d0 })();
    }
}