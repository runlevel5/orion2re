#pragma once
#include <cstdint>

namespace MAINSCR {
    extern int16_t _zoom_in_button;
    extern int16_t _zoom_out_button;
    extern int16_t _whole_screen_field;
    extern int16_t _game_button;
    extern int16_t _next_button;
    extern int16_t _design_button;
    extern int16_t _colonies_button;
    extern int16_t _planets_button;
    extern int16_t _fleets_button;
    extern int16_t _leaders_button;
    extern int16_t _races_button;
    extern int16_t _info_button;
    extern int16_t _treasury_window_field;
    extern int16_t _command_points_window_field;
    extern int16_t _food_window_field;
    extern int16_t _freighters_window_field;
    extern int16_t _research_window_field;
    extern int16_t _first_field;
    extern uint8_t _debug_enabled;
    extern int16_t _debug_field;
    extern int16_t _exit_game_hot_key;
    extern int16_t _influence_hot_key;
    extern bool _influence_view_enabled;
    extern int16_t _main_screen_grid_y;
    extern int16_t _main_screen_grid_x;
    extern s_animation_header* _temp_bitmap;
    extern int16_t _main_screen_scanned_star;
    extern int8_t _moving_a_stack;
    extern int8_t _track_time_flag;
    extern int8_t _clear_fleet_fields;
    extern int8_t _mouse_btn_pressed;
    extern int8_t _merging_relocations;
    extern uint8_t _warn_about_cheaters;
    extern uint8_t _zoom_box_counter;
    extern uint8_t _zoom_box_color_increment;
    extern uint8_t _zoom_box_increment_value;
    extern int8_t _officer_screen_return_screen;
    extern int8_t _next_turn_hit;
    extern int16_t _zoom_box_x;
    extern int16_t _zoom_box_y;
    extern int8_t _fleet_ship_selection_status;
    extern int16_t _fleet_icon_chosen;

    extern s_hotseat_system_box_state _hotseat_system_box[MAX_PLAYERS];

    extern uint8_t _hotseat_map_scale[MAX_PLAYERS];

    extern s_hotseat_fleet_box_state _hotseat_fleet_box[MAX_PLAYERS];

    inline int16_t& _hotseat_fleet_box_stack_ship_id(int32_t index) {
        return _hotseat_fleet_box[index].stack_ship_id;
    }

    extern int8_t _star_name_highlight_direction;

    extern uint8_t _star_name_highlight_value;

    /**
     * Allocates the scratch screen buffers used by main-screen UI rendering.
     * @param unused_param Unused legacy parameter kept for compatibility.
     */
    void* __cdecl Allocate_First_Screen_Seg_(int32_t unused_param);

    bool __cdecl Is_Main_Screen_Palette_();

    bool __cdecl Ship_Can_Be_Selected_(int16_t ship_idx);

    /**
     * Draws the current main-screen frame, including overlays and movable boxes.
     */
    void __cdecl Draw_Main_Screen_();

    /**
     * Enters the main-screen event loop and dispatches its UI interactions.
     */
    void __cdecl Main_Screen_();

    extern uint8_t _check_next_turn_exit_field;
    extern int16_t _n_twinkling_stars;
    extern int16_t _main_screen_ship_icon_scanned;
    extern s_colors _ship_direction_line_green_colors;
    extern s_colors _ship_direction_line_red_colors;
    extern uint8_t _fltpop_field_selection_status;
    extern int8_t _diplo_lights_anim_frame[MAX_PLAYERS];
    extern s_animation_header* _player_lights_seg[MAX_PLAYERS];

    void __cdecl Get_PointXY_Draw_Coords_(int16_t x, int16_t y, int16_t* out_x, int16_t* out_y);

    s_animation_header* __cdecl Get_Star_Picture_Seg_(int8_t spectral_class, int8_t pict_type, int8_t size);

    void __cdecl Get_Star_Draw_Coords_(int16_t star_idx, int16_t* out_x, int16_t* out_y);

    bool __cdecl Star_On_Screen_(int16_t star_idx);

    void __cdecl Draw_Stars_();

    /** Draws territories for systems visible under the same discovery and contact rules as star labels. */
    void __cdecl Draw_Influence_Overlay_();

    void __cdecl Draw_A_Star_(int16_t star_idx);

    void __cdecl Update_Star_Owners_();

    void __cdecl Draw_Mini_Main_Screen_();

    void __cdecl Draw_Mini_Main_Offscreen_();

    /**
     * Rebuilds star ownership and ship-icon positions after turn processing changes fleet state.
     */
    void __cdecl Update_Ship_Icons_In_Between_Turns_();

    void __cdecl Draw_Wormhole_Links_();

    void __cdecl Draw_Paralax_();

    void __cdecl Draw_Relocation_Links_();

    void __cdecl Draw_Black_Holes_();

    void __cdecl Draw_Main_Main_Screen_();

    /**
     * Draws a star label near the specified star or blits a pre-rendered bitmap label.
     * @param star_id Star to label.
     * @param use_bitmap Nonzero when buffer points to a pre-rendered label bitmap.
     * @param star_name Null-terminated text for the label.
     * @param buffer Optional bitmap used when use_bitmap is nonzero.
     */
    void __cdecl Print_Star_Name(int16_t star_id, int8_t use_bitmap, char* star_name, void* buffer);

    bool __cdecl Print_Star_Name_To_Bitmap_(int16_t star_idx, char* star_name, s_animation_header** out_bitmap_ptr);

    /**
     * Builds the current display label for a star and optional multi-owner bitmap.
     * @param star_idx Star to describe.
     * @param is_selected Nonzero when generating the highlighted variant.
     * @param out_uses_bitmap Set when the caller should draw the returned bitmap instead of plain text.
     * @param out_name Destination buffer for the generated label text.
     * @param out_bitmap_ptr Optional bitmap for contested or multi-owner labels.
     */
    void __cdecl Get_Star_Name_(int16_t star_idx, int8_t is_selected, int8_t* out_uses_bitmap, char* out_name, int32_t out_name_size, s_animation_header** out_bitmap_ptr);

    extern uint8_t _already_faded_into_main_screen;
    extern uint8_t _in_auto_next_turn_mode;

    void __cdecl Add_Map_Fields_();

    void __cdecl Add_Star_Fields_();

    void __cdecl Add_Moveable_Box_Fields_();

    void __cdecl Do_System_Popup_(int16_t system_id);

    void __cdecl Clear_Main_Screen_Fields_();

    void __cdecl Draw_Exploding_Plasma_Box_(int16_t x_start, int16_t y_start, int16_t x_box, int16_t y_box, int16_t width, int16_t height, int16_t mode);

    void __cdecl Set_Popup_Draw_XY_(int16_t popup_type, int16_t ship_icon_idx, int16_t* out_w, int16_t* out_h);

    void __cdecl Fade_Into_Main_Screen_();

    void __cdecl Print_Star_Names_();

    void __cdecl Draw_ETA_Destination_Line_();

    void __cdecl Draw_Diplomacy_Request_Lights_();

    void __cdecl Draw_Player_Done_Lights_();

    void __cdecl Draw_Game_Turn_Buttons_();

    void __cdecl Draw_Main_Screen_Filled_(int16_t should_copy_to_back);

    void __cdecl Draw_A_Gate_Icon_(int16_t star_idx);

    void __cdecl Draw_Gate_Icons_();

    void __cdecl Draw_Nebulae_();

    void __cdecl Draw_Ship_Icons_(bool force_draw);

    void __cdecl Set_Post_Fitted_Font_Style_(uint8_t c1, uint8_t c2);

    /**
     * Builds the localized research-point unit abbreviation for the HUD.
     * @param out_buffer Destination buffer for the 3-character unit text.
     */
    void __cdecl Get_Research_Unit_String_(char* out_buffer);

    /**
     * Draws the resource, stardate, and research summary widgets on the main-screen HUD.
     */
    void __cdecl Print_Main_Screen_Data_();

    /**
     * Draws the map layers that sit below movable-box stenciling.
     */
    void __cdecl Draw_Main_Screen_Before_Stencil_();

    void __cdecl Popup_XY_(int16_t index, int16_t x, int16_t y, int16_t* out_w, int16_t* out_h);

    void __cdecl Set_Player_Cheated_Flag_(int16_t player_idx, int8_t cheated_this_turn);

    void __cdecl Give_Me_All_Tech_Advances_();

    extern int16_t _omniscient_hot_key;
    extern int16_t _all_tech_cheat;
    extern int16_t _leader_cheat;
    extern int16_t _more_money_cheat;
    extern int16_t _get_tech_cheat;
    extern int16_t _omniscience_cheat;
    extern int16_t _all_ai_cheat;
    extern int16_t _hi_score_cheat;
    extern int16_t _last_field;

    /**
     * Handles main-screen cheat and debug hotkeys.
     * @param key Activated hotkey field id.
     * @param unused_param Unused legacy parameter kept for compatibility.
     */
    void __cdecl Check_Hot_Keys_(int16_t key, int32_t unused_param);

    void __cdecl Add_Cheats_();

    void __cdecl Do_Colony_Screen_(int16_t colony_idx, int16_t* status_flag);

    void __cdecl Check_Stars_XY_(int16_t x, int16_t y, int8_t flag);

    void __cdecl Check_Ships_XY_(int16_t mouse_x, int16_t mouse_y, int8_t selection_id);

    void __cdecl Scan_Stars_XY_(int16_t mouse_x, int16_t mouse_y);

    void __cdecl Scan_Ships_XY_(int16_t mouse_x, int16_t mouse_y);

    int16_t __cdecl Get_Mouse_Field_(int16_t type, int16_t* out_field, int16_t* out_index);

    void __cdecl Exit_Zooming_Mode_(int16_t* ptr);

    void __cdecl Do_Fleet_Popup_(int16_t ship_icon_id, int8_t check_overlap);

    void __cdecl Set_Painted_Fleet_Fields_(int16_t field_to_select);

    void __cdecl Draw_Fleet_Box_Selected_Ship_Seg_();

    /**
     * Draws the highlighted ship icon under the current main-screen hover target.
     */
    void __cdecl Draw_Scanned_Ship_Seg_();

    void __cdecl Print_Galaxy_Map_Scanned_Star_();

    void __cdecl Draw_Main_Screen_While_Moving_Boxes_();

    void __cdecl Check_Mouse_XY_Fields_();

    int16_t __cdecl Scan_System_Display_Fields_(int16_t mouse_input, int16_t input, int16_t *colony_btns, 
                                                int16_t *m_planet_idx, int16_t *planet_idx, 
                                                int16_t *m_ship_idx, int16_t *ship_idx, int16_t *selection);

    void __cdecl Draw_Main_Screen_For_Zooming_();

    int16_t __cdecl Get_New_Paralax_XYs_(int16_t target_map_x, int16_t target_map_y, int16_t scale);

    void __cdecl Clear_Twinkling_Star_Data_();

    void __cdecl Change_Home_Star_Name_();

    void __cdecl Warn_About_Cheating_Players_();

    /**
     * Shows the diplomacy-request popup and returns the selected response.
     * @param requester_id Player index requesting diplomacy.
     */
    int16_t __cdecl Do_Reply_To_Diplomacy_Request_(int16_t requester_id);

    int16_t __cdecl Check_For_Diplomacy_Request_();

    void __cdecl Check_Function_Keys_(int16_t key, int16_t* flag);

    void __cdecl Toggle_Game_Option_(int16_t option_idx);

    void __cdecl Do_Next_Turn_(int16_t* exit_flag, int8_t auto_mode);

    int16_t __cdecl Get_Ship_Stack_For_Officers_Screen_();

    int16_t __cdecl Get_Star_Id_For_Officers_Screen_();

    /**
     * Opens and runs the tax-rate popup until the user dismisses it.
     */
    void __cdecl Tax_Rate_Popup_();

    void __cdecl Scroll_Map_(int16_t x, int16_t y);

    void __cdecl Set_Reload_Fleet_Box_Palette_Flag_();

    void __cdecl Print_Highlighted_Star_Name_(int16_t star_idx);

    /**
     * Pulses the selected star label highlight state.
     * @param star_idx Star to highlight.
     */
    void __cdecl Highlight_Star_(int16_t star_idx);

    void __cdecl Redraw_Main_Screen_(uint8_t redraw_full);

    void __cdecl Load_Reply_To_Diplomacy_Request_Popup_();

    void __cdecl Add_Reply_To_Diplomacy_Request_Fields_(
        int16_t* accept_field,
        int16_t* invoke_field,
        int16_t* wait_field,
        int16_t* dismiss_field
    );

    void __cdecl Draw_Reply_To_Diplomacy_Request_();

    /**
     * Dispatches a network payload received while the main screen is active.
     * @param from_player_id Sending network player index.
     * @param sender_net_id_or_host_id Message-specific sender network id or host election id.
     * @param msg_type Network message type.
     * @param payload_len Payload size in bytes.
     * @param payload Raw payload buffer.
     */
    void __cdecl Main_Receive_Message_(uint32_t from_player_id, int16_t sender_net_id_or_host_id, uint16_t msg_type, uint32_t payload_len, void* payload);

    int16_t __cdecl Scan_Galaxy_Map_Fields_(int16_t field_id_1, int16_t field_id_2, int16_t* star_idx_ptr_1, int16_t* star_idx_ptr_2);
    int32_t __cdecl Zoom_Box_Width_(int16_t width, int16_t zoom_factor);
    int32_t __cdecl Zoom_Box_Height_(int16_t height, int16_t divisor);
    void __cdecl Cycle_Ship_Icons_(int16_t* ptr, int16_t* out_val, int16_t dir);
    void __cdecl Cycle_Star_Icons_(int16_t* p_star_idx, int16_t player_idx, int16_t direction);

    void __cdecl Center_Map_(int16_t x, int16_t y);

    /**
     * Scans fleet-box ship fields for click and hover interactions.
     * @param field_id_a Hovered field id.
     * @param field_id_b Activated field id.
     * @param out_chosen_offset Receives the selected fleet-entry offset.
     * @param out_scanned_icon_idx Receives the hovered fleet icon index.
     */
    int16_t __cdecl Scan_Fleet_Movement_Box_Fields_(int16_t field_id_a, int16_t field_id_b, int16_t* out_chosen_offset, int16_t* out_scanned_icon_idx);

    /**
     * Routes hover and click input to the active movable-box widgets on the main screen.
     * @param mouse_input Current hover field id.
     * @param input_field Activated field id.
     * @param scan_input Current scanned field id.
     * @param out_exit_flag Set when the handler changes screens.
     */
    void __cdecl Scan_Moveable_Box_Fields(int16_t mouse_input, int16_t input_field, int32_t scan_input, int16_t* out_exit_flag);

    void __cdecl Get_Star_Name_Info_(int16_t star_idx, int16_t* out_x, int16_t* out_y, int16_t* out_width);

    void __cdecl Mini_Main_Screen_Text_Box_(char* text_1, char* text_2);

    void __cdecl Wrapper_For_Normal_System_Fields_();

    void __cdecl Save_Hotseat_Map_Info_();
}