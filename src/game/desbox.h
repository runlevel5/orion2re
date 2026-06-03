#pragma once

namespace DESBOX {
    extern uint16_t _weapon_replacement_mods;

    extern int8_t _incr_value;

    extern int16_t _field_item_scanned;
    extern int16_t _field_item_chosen;
    extern int16_t _replacement_box_middle_height;
    extern int16_t _replacement_scroll_field;
    extern int16_t _replacement_box_width;
    extern int16_t _replacement_box_base_y;
    extern int16_t _replacement_box_base_x;
    extern int16_t _replacement_box_height;
    extern uint8_t _beam_filter_button_status;
    extern uint8_t _missile_filter_button_status;
    extern uint8_t _bomb_filter_button_status;
    extern uint8_t _special_filter_button_status;
    extern int16_t _replacement_scroll_dn_button;
    extern int16_t _replacement_scroll_up_button;
    extern int16_t _weapon_arc_rack_box_field;
    extern int16_t _weapon_replacement_arcs;
    extern uint16_t _weapon_replacement_rack;
    extern uint8_t _wait_ctr;
    extern uint8_t _valid_mods[16];
    extern uint8_t _valid_arcs_racks[5];

    // Builds the filtered weapon replacement list for the current player.
    // player_idx: Player whose researched weapons are eligible for display.
    int16_t __cdecl Weapon_Choices_(int16_t player_idx);

    bool __cdecl Have_Tractor_Beam_Weapon_();

    void __cdecl Set_Up_Weapon_Scroll_Bar_();
    void __cdecl Space_With_Weapons_();
    int16_t __cdecl Space_With_Weapon_(int16_t weapon_idx);
    // Adds the weapon-replacement dialog fields.
    // cancel_btn_field: Receives the cancel button field id.
    // screen_field: Receives the fullscreen/background field id.
    void __cdecl Add_Replacement_Weapon_Fields_(int16_t* cancel_btn_field, int16_t* screen_field);
    void __cdecl Draw_Weapons_Replacement_Box_();
    void __cdecl Get_Weapons_Box_Base_Dimensions_();
    void __cdecl Set_Up_Weapon_Display_(int16_t weapon_index);
    void __cdecl Weapon_Replacement_Help_(int16_t weapon_idx);
    int16_t __cdecl Weapon_Index_(int16_t index);
    void __cdecl Print_Weapon_Mod_Help_Description_(int16_t mod_id);
    // Resolves replacement-list click/help hits and reports the matching row.
    // field_id: Scanned field id from fields::Scan_Input_().
    // target_field: Input field id from fields::Get_Input_().
    // out_field_index: Receives the visible row index when a row is scanned.
    // out_visible_row: Receives the absolute row index for the matched field.
    int16_t __cdecl Scan_Replacement_Fields_(int16_t field_id, int16_t target_field, int16_t* out_field_index, int16_t* out_visible_row);
    void __cdecl Update_Weapon_Replacement_(int16_t* exit_flag, int16_t weapon_choice, int16_t slot_idx);
    void __cdecl Set_New_Weapon_Modifications_(uint16_t* modifications);
    void __cdecl Print_Weapon_Modification_Message_(int16_t mod_idx);
    int16_t __cdecl Update_Field_Item_Chosen_(int16_t weapon_idx);
    int __cdecl qsort_by_research_cost_(const void* a, const void* b);

    // Runs the generic replacement dialog for shields or computers.
    void __cdecl Generic_Replacement_Box_();

    void __cdecl Draw_Darkened_Design_Screen_();

    // Populates the selectable special-system list and returns the count that can fit.
    int16_t __cdecl Special_System_Choices_();

    void __cdecl Set_Up_Special_System_Scroll_Bar_();

    // Computes the special-system replacement box dimensions for the visible list.
    // row_count: Number of rows that may be shown in the dialog.
    void __cdecl Special_System_Replacement_Box_Dimensions_(int32_t, int16_t row_count);

    void __cdecl Draw_Special_Systems_Replacement_Box();

    // Draws the visible special-system choices at the dialog origin.
    // base_x: Left edge of the replacement box.
    // base_y: Top edge of the replacement box.
    void __cdecl Print_Special_System_Data_(int16_t base_x, int16_t base_y);

    void __cdecl Draw_Special_Systems_Scroll_Bar_();

    void __cdecl Add_Special_System_Fields_(int16_t* btn1, int16_t* btn2);

    int16_t __cdecl Special_System_Exclusion_(int16_t special_idx, int16_t* out_conflict_id);

    int16_t __cdecl Items_Selected_();

    void __cdecl Draw_Generic_Replacement_Box_();

    void __cdecl Generic_Replacement_Box_Choices_();

    void __cdecl Generic_Replacement_Box_Dimensions_();

    void __cdecl Add_Generic_Replacement_Fields_(int16_t* cancel_btn_field, int16_t* bg_field, int16_t* top_box_field);

    // Shows context help for the generic replacement dialog.
    // input_field: Current input field id.
    // clear_btn_field: Field id for the clear button.
    // ok_btn_field: Field id for the accept button.
    // box_field: Field id for the dialog box background.
    void __cdecl Draw_Generic_Replacement_Box_Help_(int16_t input_field, int16_t clear_btn_field, int16_t ok_btn_field, int16_t box_field);

    void __cdecl Redraw_Main_Design_Screen_To_Background_();

    // Runs the weapon replacement dialog for the active design slot.
    void __cdecl Weapons_Replacement_Box_();

    // Runs the special-system replacement dialog for the active design slot.
    void __cdecl Special_Systems_Box_();
    int8_t __cdecl Application_Type_(int16_t tech_app_id);

    void __cdecl Update_Weapon_Modification_Data_();

    // Computes the per-ship cost and space added by a special-system choice.
    // item_slot_idx: Index into the current choice list.
    // out_cost: Receives the computed cost.
    // out_space: Receives the computed space.
    void __cdecl System_Added_Cost_And_Space_(int16_t item_slot_idx, int16_t* out_cost, int16_t* out_space);

    int32_t __cdecl New_Mod_Space_(uint8_t mod_bit_index);

    int16_t __cdecl Valid_Modifications_(uint8_t* mod_status_list);

    void __cdecl Print_Shield_Data_(int16_t x, int16_t y);

    void __cdecl Print_Computer_Data_(int16_t x, int16_t y);

    void __cdecl Print_Main_Weapon_Box_(int16_t x, int16_t y);
   
    void __cdecl Draw_Weapons_Scroll_Bar_();
    void __cdecl Draw_Weapons_Rack_Box_(int16_t x, int16_t y);
    void __cdecl Draw_Weapons_Arc_Box_(int16_t x, int16_t y);
    void __cdecl Draw_Weapons_Mod_Box_(int16_t x, int16_t y);

    void __cdecl Clear_Weapon_Replacement_Fields_();

    int32_t __cdecl New_Arc_Space_(uint8_t arc_index);

    void __cdecl Get_Weapon_Mod_Field_XYs_(int16_t* mod_indices, int16_t* x_coords, int16_t* y_coords);

    int8_t __cdecl Mod_Allowed_On_Weapon_Type_(uint8_t mod_idx);

    int8_t __cdecl Should_Add_Mod_(int16_t mod_idx);

    int16_t __cdecl Weapon_Choice_Valid_(char* error_msg_out);

    extern const char s__00551100[];
    extern const char s_space[];
    extern const char s__00551120[];
}