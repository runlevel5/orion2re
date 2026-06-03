#pragma once
#include <cstdint>

namespace DESIGN {
    extern int16_t _repeat_count;
    extern uint8_t _replacement_wait_ctr;
    extern int16_t _design_size_field;
    extern uint8_t _use_augmented_engines;

    extern int32_t _printed_space_avail;
    extern int16_t _design_shield_replacement_field;
    extern int16_t _design_computer_replacement_field;
    // Stored as a 32-bit value in the original binary.
    extern int32_t _printed_cost;
    extern uint8_t _changed_design_name;
    extern uint8_t _add_size_button_flag[6];

    extern const char s_comma_space[3];
    extern s_animation_header* _dull_plus_button_seg;
    extern s_animation_header* _dull_minus_button_seg;
    extern s_animation_header* _dark_build_design_button_seg;

    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, struct s_ship_design* ship_design);

    uint8_t __cdecl Current_Design_Size_(int16_t player_idx, int16_t design_idx);

    int8_t __cdecl Count_Weapons_Loaded_();

    int16_t __cdecl Get_Colony_Handle_();

    uint8_t __cdecl Current_Design_Combat_Speed_(s_current_design* design);

    uint8_t __cdecl Current_Design_Min_Combat_Speed_(s_current_design* design);

    uint8_t __cdecl Current_Design_Max_Combat_Speed_(s_current_design* design);

    uint8_t __cdecl Current_Design_Base_Combat_Speed_(s_current_design* design);

    uint8_t __cdecl Current_Design_Has_Special_System_(s_current_design* design, int16_t system_id);

    bool __cdecl Has_Damper_Field_();

    uint8_t __cdecl Current_Design_Shield_();

    int16_t __cdecl Base_Shield_Cost();

    int16_t __cdecl Get_Shild_Space_();

    void __cdecl Design_Template_Costs_(int16_t player_id);

    void __cdecl Design_Template_Space_Requirements_(int16_t player_id);

    int32_t __cdecl Total_Design_Cost_();

    int32_t __cdecl Weapon_Space_(int16_t player_idx, int16_t weapon_idx, int16_t quantity, int16_t firing_arc, int16_t ammo, int16_t specials);

    int32_t __cdecl Total_Space_Used_();

    int32_t __cdecl Total_Design_Space_();

    int8_t __cdecl Count_Special_Devices_();

    void __cdecl Clear_Special_System_Slot_(int16_t slot);

    void __cdecl Update_N_Spec_Systems_In_Design_();

    void __cdecl Update_Weapon_Count_();

    void __cdecl Update_Calculated_Design_Data_(int16_t player_idx);

    void __cdecl Print_Ship_Size_Error_Message_(int16_t ship_size_index);

    void __cdecl Design_Screen_();

    void __cdecl Draw_Design_Screen_();

    void __cdecl Draw_Down_Buttons_();

    void __cdecl Print_Current_Design_();

    void __cdecl Check_For_Auto_Repeat_();

    void __cdecl Load_Design_Screen_Pictures_();

    void __cdecl Get_Tech_Description_Lbx_Name_(char* buffer);

    void __cdecl Build_Initial_Design_Template_(int16_t player_idx, int16_t design_idx);

    void __cdecl Add_Design_Buttons_();

    int8_t __cdecl Add_Size_Button_();

    void __cdecl Clear_Design_();

    void __cdecl Update_Player_Design_(int16_t player_idx, int16_t design_idx);

    void __cdecl Check_Generic_Replacement_Fields_(int16_t clicked_field, int16_t* refresh_needed, int16_t* out_status);

    void __cdecl Check_Weapon_Fields_(int16_t clicked_field, /* unused */ int16_t* refresh, int16_t* out_status);

    void __cdecl Check_Special_System_Fields_(int16_t clicked_field_id, int16_t *unused, int16_t* out_status);

    void __cdecl Check_Plus_Minus_Buttons_(int16_t field);

    void __cdecl Get_Ship_Design_Name_(int16_t race_idx, int16_t design_idx, int16_t variant_idx, char* out_name);

    void __cdecl Get_Ship_Size_Button_Field_XY_(int16_t size_idx, int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2);

    void __cdecl Build_Design_Template_(s_ship_design* template_out);

    void __cdecl Set_Flashing_Colors_(int16_t font_style, int16_t spacing);

    int16_t __cdecl Current_Design_Structural_Points_();

    void __cdecl Cost_Reduction_For_Govt_Type_(int16_t player_idx, int32_t* cost);

    int16_t __cdecl Current_Design_Armor_Absorption_();

    int16_t __cdecl Shield_Strength_();

    int16_t __cdecl Current_Shield_Damage_Points_Blocked_();

    void __cdecl Weapon_Mod_String_(int16_t mod_mask, char* out_buffer);

    void __cdecl Weapon_Arc_String_(uint16_t arc_flags, char* dest_str);

    int16_t __cdecl Weapon_Needs_Racks_(int16_t weapon_idx);

    void __cdecl Design_Weapon_Damage_String_(int16_t weapon_id_or_slot, char* buffer, int16_t field_item_chosen, int16_t row_idx);

    bool __cdecl Space_For_Weapon_Replacement_();

    int16_t __cdecl One_Weapon_Space_Without_M_Level_(int16_t weapon_idx, int16_t mod_bonus_idx, int16_t rack_type, int16_t mod_flags);

    int16_t __cdecl Tech_App_To_Tech_Id_(int16_t app_id, int16_t category);

    int16_t __cdecl One_Weapon_Space_(int16_t weapon_id);

    bool __cdecl Space_For_System_Replacement_();

    int16_t __cdecl Printed_Design_Cost_();

    int16_t __cdecl N_Replacements_For_Generic_Item_(int16_t item_type);

    bool __cdecl Replacement_Exists_(int16_t application_type);

    int16_t __cdecl Miniaturization_Level_(int16_t player_idx, int16_t app_id);

    int16_t __cdecl Tech_Level_(int16_t player_idx, int16_t tech_idx);

    int32_t __cdecl Shield_Space_Given_M_Level_(int16_t shield_id, int16_t ship_type, int16_t player_idx);

    int16_t __cdecl Shield_Space_(int16_t shield_idx, int16_t ship_size);

    int16_t __cdecl Shield_Cost_(int16_t shield_type);

    int16_t __cdecl Cost_Given_M_Level_(int16_t base_cost, int16_t m_level);

    int16_t __cdecl Computer_Cost_(int16_t computer_type);

    int16_t __cdecl Weapon_Cost_(int16_t weapon_idx, int16_t count, int16_t arc, int16_t ammo, int16_t specials);

    int8_t __cdecl Miniaturization_Category_(int16_t tech_app);

    int32_t __cdecl Space_Given_M_Level_(int16_t amount, int16_t miniaturization_level, int8_t mode);

    int16_t __cdecl Weapon_Rack_Size_(int16_t count);

    void __cdecl Bonus_For_Special_Mods_(int16_t flags, int16_t *bonus1_out, int16_t *bonus2_out);

    int16_t __cdecl Get_Weapon_Mod_Space_Bonus_(int16_t flags);

    void __cdecl Bonus_For_Regular_Mods_(int16_t mods, int16_t* bonus1, int16_t* bonus2);

    int16_t __cdecl Weapon_Type_(int16_t weapon_idx);

    void __cdecl Clear_Weapon_Slot_(int16_t slot_idx);
}