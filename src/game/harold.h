#pragma once

namespace HAROLD {
    uint32_t __cdecl Is_Colony_Star_(int16_t star_idx);

    int16_t __cdecl Get_Scaled_Value_(int16_t value);

    int32_t __cdecl Get_Up_Scaled_Value_(int16_t value);

    bool __cdecl Empty_System_(int16_t star_idx);

    bool __cdecl Is_Home_Star_(int16_t star_idx);

    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx);

    int16_t __cdecl N_Colony_Owners_(int16_t star_idx);

    uint8_t __cdecl N_Satellites(int16_t star_index);

    void __cdecl Set_Wormhole_Id_(int16_t star_idx, uint8_t* star_bitmap);

    int16_t __cdecl N_Food_Planets_In_System_(int16_t system_index);

    bool __cdecl Star_Is_Orion_(int16_t star_idx);

    bool __cdecl Star_Is_Outpost_Star_(int16_t star_idx);

    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx);

    /** Writes the localized font LBX filename into buffer. */
    void __cdecl Get_Font_File_(char* buffer);
    /** Writes the localized officer-data LBX filename into out_name. */
    void __cdecl Get_Officer_Data_LBX_Name_(char* out_name);

    void __cdecl Set_Sound_Volume_To_Settings_();

    /** Verifies that planet/star orbit links are internally consistent. */
    void __cdecl Confirm_Planet_And_Star_Index_Link_(const char* unused_context);

    int8_t __cdecl Planet_Number_(int16_t planet_index);

    void __cdecl Set_All_Visited_Flags_();

    void __cdecl Init_Moving_Boxes_();

    /** Returns the localized H-string for the supplied message id. */
    const char* __cdecl H_Message_(int16_t id, int unused_save_slot = 0, const struct s_save_game_description* unused_save_description = nullptr);

    int16_t __cdecl N_Player_Ships_(int16_t player_idx);

    /** Writes the localized monetary unit abbreviation into buffer. */
    void __cdecl Get_Monetary_Unit_String_(char* buffer);

    void __cdecl Randomize_List_(int16_t* list, int16_t size, uint8_t use_clock);

    void __cdecl Save_Mouse_List_();

    extern s_mouse_list _saved_mouse_list[10];

    extern uint16_t _saved_mouse_list_count;

    void __cdecl Init_Fleet_Movement_Box_();

    void __cdecl Init_System_Display_Box_();

    void __cdecl Planet_Data_Screen_();

    void __cdecl Load_Language_Fonts_();

    void __cdecl Set_Language_Race_Names_();

    uint8_t* __cdecl Harold_Init_();

    void __cdecl Harold_Defaults_();

    /** Applies the player's palette-derived colors to the active font style. */
    void __cdecl Set_Player_Font_Style_(int16_t player_idx, bool apply_selected_color_offset);

    int16_t __cdecl Map_Scale_To_Zoom_Level_();

    void __cdecl Decrement_Zoom_Count_();

    int16_t __cdecl Increment_Zoom_Count_With_Limit_();

    void __cdecl Increment_Zoom_Count_();

    int16_t __cdecl Get_Map_Scale_(int16_t layer_type);

    void __cdecl Draw_Multi_Colored_Box_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, int16_t color_offset);

    int16_t __cdecl Map_Scale_Star_Size_To_Zoom_Level_(int16_t star_idx);

    int16_t __cdecl Galaxy_Size_From_N_Stars_();

    void __cdecl Restore_Mouse_List_();

    int8_t __cdecl Was_Mouse_List_Saved_();

    bool __cdecl Player_Sighted_Monster_(int16_t player_id, int16_t star_id);

    void __cdecl Load_Leader_Titles_();

    void __cdecl Load_H_Strings_();

    int16_t __cdecl User_Box_(uint8_t* refresh_flag, char* message, int16_t type);

    int16_t __cdecl Get_N_Remaining_Humans_();

    /** Returns the visible owner with the most colonies in the system for the viewing player. */
    int16_t __cdecl Star_Owner_(int16_t star_idx, int16_t viewer_player_idx);

    int16_t __cdecl Absolute_Location_(int16_t location);

    int16_t __cdecl Actual_Star_Id_(int16_t star_id, int16_t* out_actual_id);

    int16_t __cdecl XY_To_Star_Id_(int16_t x, int16_t y);

    int16_t __cdecl N_Turns_To_Star_(int16_t ship_id, int16_t star_id);

    bool __cdecl Player_Is_Omniscient_(int16_t player_idx);

    extern uint8_t _set_fade_for_first_fltpop;

    int16_t __cdecl Zoom_Level_Font_Style_();

    /** Builds a font color ramp for the specified player and style. */
    void __cdecl Get_Player_Font_Colors_(int16_t player_idx, int16_t style_idx, s_colors* style_buf, bool apply_selected_color_offset);

    bool __cdecl Contact_With_Player_(int16_t player_idx, int16_t target_idx);

    char __cdecl Contact_With_One_Colony_(int16_t star_idx);

    bool __cdecl Player_Has_Stargate_(int16_t player_idx, int16_t star_idx);

    bool __cdecl Player_Has_Jumpgate_(int16_t player_idx, int16_t star_idx);

    bool __cdecl Player_Has_Colony_(int16_t player_num, int16_t star_idx);

    /** Returns whether any planet in the system currently hosts an outpost. */
    bool __cdecl Player_Has_Outpost_(int16_t unused_player_num, int16_t star_idx);

    int8_t __cdecl Planet_Has_Outpost_(int16_t planet_idx);

    uint8_t __cdecl Player_Has_Visited_(int16_t player_idx, int16_t star_idx);

    bool __cdecl Star_Has_Artemis_Net_(s_star_data* star);

    bool __cdecl Player_Has_Artemis_Net_(s_player* player, s_star_data* star);

    bool __cdecl Player_Has_Dimensional_Portal_(s_player* player, s_star_data* star);

    bool __cdecl Star_Has_Dimensional_Portal_(s_star_data* star);

    int16_t __cdecl Planet_Type_(int16_t star_idx, int16_t star_planet);

    void __cdecl Colony_Owners_(int16_t star_idx, int16_t* player_counts, int16_t* total_count, bool have_contact);

    int16_t __cdecl Players_Home_Star_(int16_t star_idx);

    int16_t __cdecl Planet_Index_(int16_t star_idx, int16_t planet_idx);

    extern const char s_after_home_world_gen[];

    extern const char s___s__00556ae4[];
    extern const char s_in_next_turn[];

    bool __cdecl Star_Guarded_By_Monster_(int16_t location_idx, int16_t* monster_owner);

    void __cdecl Star_XY_To_Ship_Id_(s_star_data* target_star, int16_t* out_indices, int16_t* count);

    int16_t __cdecl First_Ship_Orbiting_Star_(s_star_data* star);

    void __cdecl Write_Language_Setting_File_();
}