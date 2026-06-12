#pragma once

namespace MOX {
    extern const char _ship_type_names[5][6][16];
    extern int8_t _tech_categories[51][2];
    extern const char s_combat_lbx[11];
    extern uint8_t _first_field_in_group[10];
    extern s_animation_header* _fighter_seg[15][64];
    extern uint8_t _draw_officer_to_back;
    extern int16_t _ordnance_bonus[15];
    extern int16_t _n_fltscrn_big_icons_added;
    extern int16_t _n_fltscrn_big_icons;
    extern int8_t _n_fltscrn_rows;
    extern int16_t _old_small_ship_stack_ptr;
    extern int16_t _old_first_fltscrn_icon;
    extern int16_t _n_big_icons_selected;
    extern uint8_t _galaxy_star_anim_count;
    extern int16_t _fltscrn_bar_value;
    extern int8_t _big_icon_display_columns;
    extern int8_t _big_icon_display_rows;
    extern uint16_t _ken_trace;
    extern int8_t _big_icon_display_max_icons;
    extern int16_t _big_icon_x_offset;
    extern int16_t _big_icon_y_offset;
    extern int16_t _offpop_big_icon_selected;
    extern uint8_t _drawing_officer_popup;
    extern int16_t* _fltscrn_ship_icon_selection_list;
    extern uint8_t _look_at_colonies;
    extern s_big_icon* _fltscrn_big_icon;
    extern uint8_t _add_help_fields;
    extern uint8_t _first_time;
    extern int16_t _fleet_movement_bar_value;
    extern uint8_t _draw_expanding_box;
    extern uint8_t _mainscr_palette;
    extern s_scroll_bar* _scroll_bar;
    extern int16_t _fltscrn_stack_owner;
    extern uint8_t _add_scrap_button;
    extern uint8_t _skip_draw_ships;
    extern uint8_t _fix_palette;
    extern s_animation_header* _scrap_button_seg;
    extern s_animation_header* _colony_marker_seg[MAX_PLAYERS];
    extern s_animation_header* _outpost_marker_seg[MAX_PLAYERS];
    extern s_animation_header* _o_highlight_seg;
    extern uint8_t _add_all_button;
    extern s_animation_header* _fleet_all_button_seg;
    extern s_animation_header* _fleet_return_button_seg;
    extern int16_t _old_fltscrn_stack_owner;
    extern s_animation_header* _up_scroll_button_seg;
    extern s_animation_header* _down_scroll_button_seg;
    extern s_animation_header* _prev_fleet_button_seg;
    extern s_animation_header* _next_fleet_button_seg;
    extern s_animation_header* _relocate_fleet_button_seg;
    extern s_animation_header* _officers_button_seg;
    extern uint8_t _add_officers_button;
    extern int16_t _fltscrn_support_button_status;
    extern int16_t _fltscrn_combat_button_status;
    extern s_mp_screen* _mp_screen;
    extern int8_t _player_mode[15];
    extern int16_t _cur_ship;
    extern int16_t _net_destination;
    extern int16_t _done_button;
    extern int16_t _msl_impact_warning_button;
    extern int16_t _draw_legal_boxes_button;
    extern int16_t _msl_warning_button;
    extern int16_t _wait_button;
    extern int16_t _scan_button;
    extern int16_t _missile_field[300];
    extern int16_t _combat_grid[COMBAT_GRID_WIDTH][COMBAT_GRID_HEIGHT];
    extern int16_t _weapon_mod_field_status[15];
    extern int16_t _weapon_button[8];
    extern int16_t _ship_turn_order[210];
    extern int16_t _repl_fields[10];
    extern int16_t _side_end_of_turn[15];
    extern int16_t _unused_ship_turn_order_marker;
    extern int16_t _design_choice_items[40];
    extern int16_t _weapon_mod_fields[15];
    extern int16_t _weapon_arc_field[5];
    extern int16_t _weapon_rack_field[5];
    extern int16_t _multi_colored_box_color_start;
    extern int16_t _beam_filter_button;
    extern int16_t _missile_filter_button;
    extern int16_t _special_filter_button;
    extern int16_t _replacement_box_accept_button;
    extern int16_t _bomb_filter_button;
    extern const char s__00551100[1];

    extern uint16_t _fleet_box_map_height;
    extern s_current_design* _design;

    extern int16_t _scanned_big_ship;
    extern int16_t _small_ship_stack_ptr;
    extern int16_t _scanned_small_ship;
    extern int16_t _n_fltscrn_officers;
    extern int16_t* _officer_id_list;
    // Suggested name: MOX::_star_colors_seg
    extern s_animation_header* _star_colors_seg[11];
    extern s_animation_header* _fleet_galaxy_star_seg[11];
    extern s_animation_header* _colony_galaxy_star_seg[10];
    extern s_animation_header* _officer_skill_icon_seg[27];
    extern s_animation_header* _hl_antaran_ship_icon_seg[24];
    extern s_animation_header* _antaran_ship_icons[24];
    extern s_animation_header* _o_ring_seg[5];
    extern s_animation_header* _planet_mask_seg[5];
    extern s_animation_header* _fltscrn_small_ship_icon_seg[8];
    extern s_animation_header* _monster_ship_seg[4];
    extern s_animation_header* _hl_monster_ship_icon_seg[4];
    extern s_animation_header* _ship_icon_seg[32];
    extern s_animation_header* _hl_ship_icon_seg[32];
    extern s_animation_header* _fleet_background_seg;
    extern void* _officer_pictures_screen_seg;
    extern s_animation_header* _officer_main_seg;
    extern s_animation_header* _officer_colony_box_seg;
    extern s_animation_header* _officer_colony_ldrs_button_seg;
    extern s_animation_header* _officer_ship_ldrs_button_seg;
    extern s_animation_header* _officer_up_button_seg;
    extern s_animation_header* _officer_dn_button_seg;
    extern s_animation_header* _officer_prev_button_seg;
    extern s_animation_header* _officer_next_button_seg;
    extern s_animation_header* _officer_hire_button_seg;
    extern s_animation_header* _officer_return_button_seg;
    extern s_animation_header* _officer_cancel_hire_button_seg;
    extern s_animation_header* _selected_box_seg;
    extern s_animation_header* _scanned_box_seg;
    extern s_animation_header* _fleet_scroll_anim_seg;
    extern s_animation_header* _officer_normal_window_seg;
    extern s_animation_header* _officer_bright_window_seg;
    extern s_animation_header** _temp_officers_pict;
    extern s_animation_header* _officer_dismiss_button_seg;
    extern s_animation_header* _officer_fleet_box_seg;
    extern s_animation_header* _dull_officer_dismiss_button_seg;
    extern s_animation_header* _dull_officer_pool_button_seg;
    extern s_animation_header* _officer_pool_button_seg;
    extern s_animation_header* _dull_officer_hire_button_seg;
    extern s_animation_header* _officer_hire_replace_seg;
    extern int8_t _officer_scanned;
    extern int8_t _officer_scrn_type;
    extern int8_t _officer_selected;

    extern int16_t _special_scroll_dn_button;
    extern int8_t _n_weapons_loaded;
    extern uint8_t _n_special_systems;

    extern int8_t* _fltscrn_icon_selection_status;

    // References
    extern int8_t _food_per_farmer_table[10];
    extern uint8_t _star_class_table[7][3];
    extern int16_t _ranged_to_hit_penalty[9];
    extern int16_t _ranged_damage_penalty[9];
    extern int16_t _cur_missile_count;
    extern int16_t _NUM_STARS;
    extern s_scroll_bar _fltpop_scroll_bar;
    extern int16_t _cur_map_scale;
    extern int16_t _cur_map_x;
    extern int16_t _cur_map_y;
    extern uint16_t _global_esc;
    extern int16_t _return_screen;
    extern char* _save_game_dates;
    extern char* _save_game_stardates;
    extern char* _star_name_loaded_string;
    extern int16_t _scanned_field;
    extern int8_t _previous_screen;
    extern int16_t _max_debug_saves;
    extern int16_t _first_debug_save;
    extern int16_t _debug_saves_on;
    extern uint8_t _turns_between_debug_saves;
    extern void* _sound_buffer;
    extern int16_t _hot_seat_flag;
    extern int16_t _current_screen;
    extern int16_t _design_screen_replacement_type;
    extern int16_t _design_screen_replacement_slot;
    extern int16_t _weapons_field[8];
    extern s_animation_header* _asteroid_seg;
    extern s_animation_header* _screen_seg;
    extern s_animation_header* _diplomacy_request_popup_seg;
    extern s_animation_header* _combat_bitmap;
    extern s_animation_header* _combat_map_planet_seg;
    extern s_animation_header* _combat_planet_seg;
    extern s_animation_header* _combat_planet_seg2;
    extern uint8_t _ship_darken_map[128][16];
    extern uint8_t _given_chance_to_rename_home_star[MAX_PLAYERS];
    extern uint8_t _player_has_ship_in_system;
    extern s_animation_header* _moveable_box_pict[25];
    extern int8_t _game_type;
    extern s_antaran _antarans;
    extern int8_t _maps_to_generate;
    extern uint8_t _auto_turn_exit;
    extern int16_t _special_scroll_up_button[2];
    extern uint8_t _blue_color2;
    extern int16_t _auto_msl_warning_off;
    extern s_animation_header* _draw_ship_bitmap;
    extern int16_t _multi_colored_line_start;
    extern int16_t _design_choice_item_selected[40];
    extern int8_t _first_special_system_ctr;
    extern int16_t _special_system_field[8];
    extern int16_t _planet_buttons[5];
    extern uint8_t _first_design_screen_call;
    extern int16_t _design_size;
    extern uint8_t _refresh_mode;
    extern uint8_t _mox_font_colors_offset;
    extern int16_t _star_width;
    extern int16_t _star_height;
    extern char _temp_string[512];
    extern char _temp_string_buf[64];
    extern int16_t _take_control_button;
    extern int16_t _auto_cmbt_button_value;
    extern int16_t _PLAYER_NUM;
    extern uint8_t _active_player[9];
    extern int16_t _active_count;
    extern int16_t _ai_retreat_flag;
    extern int16_t _bio_weapon_flag;
    extern int16_t _last_ship_status;
    extern int16_t _ship_status_display;
    extern int16_t _dont_draw_ship;
    extern int16_t _draw_active_missiles;
    extern int16_t _order_info_flag;
    extern int16_t _super_fast_flag;
    extern int8_t _real_attack_flag;
    extern int16_t _shields_flag;
    extern int16_t _msl_impact_warning_flag;
    extern int16_t _draw_legal_boxes_flag;
    extern int16_t _take_control_flag;
    extern int16_t _combat_far_x;
    extern int16_t _combat_mid_x;
    extern int16_t _combat_far_y;
    extern int16_t _combat_mid_y;
    extern int16_t _net_flag;
    extern int16_t _in_nebula_flag;
    extern int16_t _skan_flag;
    extern uint8_t _draw_design_screen_to_back;
    extern uint8_t _refit_ship;
    extern int32_t _original_cost;
    extern int16_t _design_name_field;
    extern int16_t _design_cancel_button;
    extern int16_t _design_clear_button;
    extern int16_t _design_build_button;
    extern s_player* _player;
    extern s_player* _cur_plyr_ptr;
    extern char* _personality_labels[7];
    extern char* _objective_labels[6];
    extern char* _treaty_labels[7];
    extern char* _help_category_labels[17];
    extern int16_t _old_system_index;
    extern int8_t _selected_star_color_mod;
    extern int8_t _selected_star_color_offset;
    extern int32_t _stardate;
    extern s_settings _settings;
    extern char* _hyper_field_title;
    extern const char s_Harbinger[12];
    extern uint8_t _at_least_one_tactical_combat;
    extern s_animation_header* _planet_summary_galaxy_star_seg[11];
    extern int8_t _first_weapon_ctr;
    extern int16_t _fleet_box_colonize_button;
    extern uint8_t _reloaded_ship_palette;
    extern uint8_t _g_galaxy_age;
    extern int16_t _ship_data_id;
    extern int16_t _cur_combat_x;
    extern int16_t _cur_ptr;
    extern int16_t _wait_ptr;
    extern int16_t _board_flag;
    extern int16_t _tactical_net_choice;
    extern int16_t _auto_button;
    extern int16_t _auto_combat;
    extern int16_t _speedx2_button;
    extern int16_t _grid_button;
    extern int16_t _shields_button;
    extern int16_t _super_fast_button;
    extern int16_t _self_destruct_button;
    extern int16_t _weapons_button;
    extern int16_t _specials_button;
    extern int16_t _cmbt_info_button;
    extern int16_t _flee_button;
    extern int16_t _board_button;
    extern int16_t _map_field;
    extern int16_t _map_y;
    extern int16_t _map_x;
    extern int16_t _center_button;
    extern int16_t _space_field;
    extern int16_t _ypos;
    extern int16_t _xpos;
    extern int16_t _saved_msl_warn;
    extern uint8_t _red_color_pulse_value;
    extern int8_t _red_color_pulse_incr;
    extern int16_t _fleet_ship_scanned;
    extern int16_t _fleet_movement_all_button;
    extern int16_t _galaxy_map_scanned_star;
    extern int8_t _draw_system_display_without_button;
    extern uint8_t _need_to_restore_help_list;
    extern int16_t _ship_icon_player_id;
    extern int8_t _at_least_one_report;
    extern uint8_t _previous_report;
    extern int16_t _first_node_ptr_in_stack_;
    extern int16_t _system_display_ring_radius[5];
    extern uint8_t _rot_plan_dim[5];
    extern int16_t _system_star_id;
    extern int16_t _system_star_id_for_auto_select;
    extern uint8_t _draw_fleet_screen_to_back;
    extern s_mouse_xy _mouse_XY;
    extern int16_t _system_display_ship_scanned;
    extern int16_t _system_icon_scanned;
    extern uint16_t _global_exit_field;
    extern int16_t _eta_star_id;
    extern int16_t _stencil_box_id;
    extern int16_t _draw_main_screen_stencilled;
    extern uint16_t _animated_star;
    extern s_mouse_list _mouse_list_zoom_mode[5];
    extern int8_t _just_moved_stack;
    extern int16_t _first_fleet_movement_icon;

    extern int16_t _next_free_node;

    extern uint8_t _main_palette_player_colors[MAX_PLAYERS];

    extern int16_t _h_string_ix[397];
    extern char _h_strings[0x4074];

    extern s_help_box _help_entry_list[50];

    // Suggested name: MOX::_class_to_mineral
    extern int8_t _class_to_mineral[60];

    // Suggested name: MOX::_planet_special
    extern int8_t _planet_special;

    // Suggested name: MOX::_planet_special_weighted_chance
    extern uint8_t _planet_special_weighted_chance[12];

    // Suggested name: MOX::_gravity_table
    extern int8_t _gravity_table[30];

    extern s_mouse_list _mouse_list_arrow;
    extern s_mouse_list _mouse_list_relocation;
    extern s_mouse_list _cmbt_mouse_list_clock;
    extern s_mouse_list _mouse_list_variable;
    extern s_mouse_list _mouse_list_none;

    extern int16_t _mid_star_x;
    extern int16_t _mid_star_y;
    extern int16_t _design_screen_replacement_item;
    extern int16_t _far_star_x;
    extern int16_t _far_star_y;
    extern int16_t _near_star_y;
    extern int16_t _near_star_x;
    extern s_animation_header* _near_star_seg;

    extern const char s_ATZ_0055AA7C[4];

    extern const char s_AT_K0_M1_D2_0055AA6C[14];

    extern const char s_AT_H0_S0_0_0055AA60[11];

    extern const char s_5551212_0055AA58[8];

    extern const char s_fmt_s[3];

    extern const char s_fmt_scanned_star_info[8];

    extern const char s_fmt_contested[15];

    extern void* _special_names_seg;

    extern void* _global_data_seg;

    extern s_nebula* _nebula;

    extern uint8_t _NUM_NEBULAS;

    extern int16_t _NUM_COLONIES;

    extern int16_t _planet_count;

    // Suggested name: MOX::_climate_modifier_table
    extern int8_t _climate_modifier_table[5];

    extern s_planet_data* _planet;

    extern const char* _roman_literals[21];

    extern char* _planet_special_string[12];
    extern char* _planet_climate_string[10];
    extern char* _mineral_class_string[5];
    extern uint8_t _red_color2;
    extern char* _planet_size_string[5];
    extern char* _planet_type_string[10];
    extern char* _planet_group_string[4];
    extern char* _planet_atmosphere_string[3];
    extern char* _spectral_class_string[7];
    extern char* _admin_skill_names[9];

    extern char* _planet_gravity_string[3];

    extern int16_t _NUM_SHIPS;
    extern void* _global_bitmap_seg;
    extern uint8_t _first_time_drawn;
    extern uint8_t _draw_main_screen_to_back;
    extern uint8_t _dont_draw_main_screen_to_back;
    extern int16_t _zoom_count;
    extern int16_t _max_zoom_count;
    extern uint8_t _star_bg_index;
    extern s_animation_header* _star_bg_seg;
    extern void* _scan_ship_seg;
    extern s_animation_header* _detail_fleet_icon_seg;
    extern uint16_t _fleet_icon_button_count;
    extern int16_t _weapons_count_plus_button[8];
    extern int16_t _weapons_count_minus_button[8];
    extern int16_t _design_ship_size_field[6];
    extern void* _techname_seg;
    extern void* _tech_desc_seg;
    extern s_animation_header* _main_screen_seg;
    extern s_animation_header* _design_screen_seg;
    extern s_animation_header* _design_icon_lf_arrow_seg;
    extern s_animation_header* _design_icon_rt_arrow_seg;
    extern s_animation_header* _clear_design_button_seg;
    extern s_animation_header* _cancel_design_button_seg;
    extern s_animation_header* _build_design_button_seg;
    extern s_animation_header* _plus_button_seg;
    extern s_animation_header* _minus_button_seg;
    extern s_animation_header* _d_scroll_up_button_seg;
    extern s_animation_header* _d_scroll_dn_button_seg;
    extern s_animation_header* _dull_d_scroll_up_button_seg;
    extern s_animation_header* _dull_d_scroll_dn_button_seg;
    extern s_animation_header* _beam_filter_button_seg;
    extern s_animation_header* _missile_filter_button_seg;
    extern s_animation_header* _bomb_filter_button_seg;
    extern s_animation_header* _special_filter_button_seg;
    extern s_animation_header* _weapons_cancel_button_seg;
    extern s_animation_header* _weapons_accept_button_seg;
    extern s_animation_header* _weapons_top_box_seg;
    extern s_animation_header* _weapons_mid_box_seg;
    extern s_animation_header* _weapons_bot_box_seg;
    extern s_animation_header* _weapons_selected_seg;
    extern s_animation_header* _weapons_blank_mods_seg;
    extern s_animation_header* _system_repl_scroll_bar_seg;
    extern s_animation_header* _system_repl_box_top_seg;
    extern s_animation_header* _system_repl_box_mid_seg;
    extern s_animation_header* _system_repl_box_btm_seg;
    extern s_animation_header* _system_repl_box_cancel_btn;
    extern int16_t _weapons_clear_button[8];
    extern int16_t _special_system_clear_button[8];
    extern s_animation_header* _design_size_button_seg[6];
    extern s_animation_header* _darkened_size_button_seg[6];
    extern s_animation_header* _weapons_arc_pict_seg[5];
    extern s_animation_header* _weapons_arc_text_seg[5];
    extern s_animation_header* _weapons_rack_text_seg[5];
    extern s_animation_header* _weapons_special_seg[18];
    extern s_weapon_filter_item* _weapon_filter;
    extern char* _special_desc_string[39];
    extern char* _weapon_notes_string[40];
    extern s_animation_header* _d_scroll_bar_seg;
    extern s_animation_header* _weapons_rack_seg;

    extern s_animation_header* _mainscrn_diplo_lights_seg;

    extern s_animation_header* _diplomacy_request_wait_seg;
    extern s_animation_header* _diplomacy_request_invoke_seg;
    extern s_animation_header* _diplomacy_request_accept_seg;

    extern void* _global_screen_seg;

    extern void* _global_cache_seg;

    extern s_animation_header* _mid_star_seg;
    extern s_animation_header* _far_star_seg;
    extern uint8_t _fleet_icon_selection_status[64];

    // Suggested name: MOX::_star_seg
    extern s_animation_header* _star_seg[40];
    extern s_animation_header* _sun_seg[7];

    extern uint16_t _n_stats_to_generate;

    extern uint32_t _global_cache_size;

    extern uint32_t _cmd_line_seed;

    extern int16_t _ship_icon_lf_button;
    extern int16_t _ship_icon_rt_button;

    extern uint8_t _zoom_in;
    extern s_animation_header* _main_zoom_in_button_seg;
    extern s_animation_header* _main_zoom_out_button_seg;
    extern s_animation_header* _main_game_button_seg;
    extern s_animation_header* _main_next_button_seg;
    extern s_animation_header* _main_colonies_button_seg;
    extern s_animation_header* _main_planets_button_seg;
    extern s_animation_header* _main_fleets_button_seg;
    extern s_animation_header* _main_leaders_button_seg;
    extern s_animation_header* _main_races_button_seg;
    extern s_animation_header* _main_info_button_seg;
    extern int16_t _star_button[MAX_STARS];
    extern int16_t _main_screen_grid_field;
    extern int16_t _main_screen_grid_x;
    extern int16_t _main_screen_grid_y;

    extern int16_t _NUM_PLAYERS;
    extern uint8_t _red_color1;
    extern char* _government_labels[8];
    extern char* _special_ability_names[32];
    extern int32_t _stardate_at_beg_of_turn;
    extern int8_t _global_var;
    extern uint16_t _FULL_DRAW_SCREEN_FLAG;
    extern uint16_t _combat_init;
    extern void* _warp_core_remap_colors;
    extern s_mouse_list* _combat_mouse_grid;
    extern void* _stasis_bubble_remap_colors;
    extern s_animation_header* _combat_background_seg;
    extern s_animation_header* _combat_far_star;
    extern s_animation_header* _combat_mid_star;
    extern s_animation_header* _combat_near_star;
    extern uint16_t _apply_damage_to_planet;
    extern uint16_t _missile_warning;
    extern void* _cmbt_option_seg;
    extern s_animation_header* _warning_missile_seg;
    extern void* _self_destruct_seg;
    extern uint16_t _saved_msl_warning;
    extern void* _auto_button_seg;
    extern s_animation_header* _combat_options_background_seg;
    extern int16_t _cur_combat_y;
    extern int16_t _grid_flag;
    extern int16_t _speedx2_flag;
    extern int16_t _auto_combat_flag[2];
    extern int16_t _combat_clock_frame;
    extern int16_t _combat_near_x;
    extern int16_t _combat_near_y;
    extern void* _cmbt_done_button_seg;
    extern void* _level_symbol_seg;
    extern void* _scan_button_seg;
    extern void* _board_button_seg;
    extern void* _flee_button_seg;
    extern void* _wait_button_seg;
    extern void* _cmbt_info_button_seg;
    extern void* _weapons_button_seg;
    extern void* _specials_button_seg;
    extern s_animation_header* _msl_warning_on_seg;
    extern void* _warning_fighter_seg;
    extern s_animation_header* _ship_bitmap;
    extern s_animation_header* _darken_plasma_map;
    extern void* _shield_remap_colors;
    extern void* _pulsar_remap_colors;
    extern s_animation_header* _small_tractor_seg;
    extern void* _tractor_remap_colors;
    extern void* _bhg_remap_colors;
    extern void* _plasma_web_remap_colors;
    extern void* _stellar_converter_remap_colors;
    extern s_animation_header* _msl_seg[112];
    extern s_animation_header* _ship_cursor[4];
    extern s_animation_header* _shield_div_bitmap[4];
    extern s_animation_header* _fltscrn_monster_ship_icon[7];
    extern s_animation_header* _shield_mask_bitmap[16];
    extern int16_t _fighter_beam_type[9];
    extern int16_t _fighter_bomb_type[9];
    extern s_combat_data* _combat_data;
    extern s_missile* _missile;
    extern int16_t _missile_hit_radius[6];
    extern s_net_tactical_combat_message _new_message;
    extern int16_t _gate_icon_field[4];
    extern int16_t _system_display_base_x;
    extern int16_t _system_display_base_y;
    extern s_sys_disp* _sys_disp;
    extern s_system_display_ships _system_display_ships[15];
    extern int16_t _system_display_grid_y[6];
    extern int16_t _system_display_grid_x[6];
    extern int16_t _system_display_grid_field;
    extern char* _race_names[20];
    extern int8_t _class_to_num_satellites[10][6];
    extern int8_t _special_bonus[9][3];
    extern uint16_t _star_fields_dim[6];

    extern s_ship_data* _ship;

    extern int16_t _combat_defender_count;
    extern int16_t _combat_attacker_count;
    extern s_qship* _qcombat_data;
    extern int16_t _combat_total_ship_count;
    extern uint8_t _combat_done_waiting;
    extern s_colors _combat_green;
    extern int16_t _combat_attacker_player;
    extern int16_t _combat_turn;
    extern uint8_t _colony_defense_flags;
    extern int16_t _cmbt_colony_id;
    extern int16_t _combat_defender_player;
    extern s_combat_bomb_result _combat_bomb_result;

    extern s_global_combat_data _global_combat_data;

    extern int16_t* _officers_original_star_id;

    extern int16_t* _officers_original_ship_id;

    extern void* _dimmed_all_button_seg;
    extern void* _dimmed_relocate_button_seg;
    extern void* _dimmed_scrap_button_seg;
    extern void* _dimmed_officers_button_seg;

    extern s_leader_data* _leaders;

    extern char* _officer_level_names[6];

    extern char* _star_officer_level_names[6];

    extern s_skill_data _skill_data[54];

    extern s_colony* _colony;

    extern s_animation_header* _darkened_turn_button_seg;

    extern s_animation_header* _fleet_box_selection_seg;
    extern int16_t _ship_frame;
    extern s_animation_header* _mainscrn_plyr_done_lights_seg;
    extern uint16_t _skip_fields;

    extern uint8_t _ship_icon_width[4];
    extern uint8_t _ship_icon_height[4];
    extern s_ship_move_info _g_ship_move_info;

    extern s_star_data* _star;

    // Suggested name: MOX::orbit_to_satellite_type
    extern uint8_t _orbit_to_satellite_type[10][5];

    // Suggested name: MOX::_ship_node
    extern s_ship_node _ship_node[MAX_SHIPS];

    extern int16_t _ship_stack_count;

    extern int16_t _ship_stack_start[MAX_SHIPS];

    extern uint8_t _fleet_box_ship_ids[63];

    extern void* _other_ships_screen_seg;

    extern void* _fleet_box_grid_sm_seg4;

    extern int16_t _icon_overlay_y;

    extern int16_t _ship_icon_count;

    extern int16_t _icon_overlay_x;

    extern int16_t _fleet_box_ship_stack;

    extern s_ship_icon _ship_icon[572];

    extern int8_t _black_hole_anim_count[10];

    extern int16_t _bill_savegame[6];
    extern uint16_t _antaran_resource_level;
    extern s_crew_data _crew_data[5];
    extern uint16_t _chance_for_antaran_invasion;
    extern uint16_t _disallow_saves_code;

    extern int16_t _max_map_scale;
    extern int16_t _MAP_MAX_X;
    extern int16_t _MAP_MAX_Y;
    extern uint8_t _g_galaxy_size;

    extern void* _nebula_screen_seg;

    // Suggested name: MOX::nebula_pict_segments
    // This represents a 2D array of sprite pointers, dimensions [12][4].
    extern s_animation_header* _nebula_pict_seg[12][4];

    // Suggested name: _planet_max_farms
    extern uint8_t _planet_max_farms[5];

    extern int8_t _planet_max_population[5];

    extern uint8_t _planet_size_table[5];

    #pragma pack(push, 1)
    struct s_class_group {
        int8_t group[5];
    };
    #pragma pack(pop)

    // 5 groups per spectral class
    extern s_class_group _class_to_group[6];

    // 4 groups per climate
    extern uint8_t _climate_roll_table[10][4];

    // Suggested name: MOX::normal_galaxy_climate_roll_table
    extern uint8_t _normal_gal_climate_roll_table[10][4];

    // Suggested name: MOX::old_galaxy_climate_roll_table
    extern uint8_t _old_gal_climate_roll_table[10][4];

    // Array of movable box structures.
    extern s_movable_box _moveable_box[5];

    extern uint8_t _fleet_movement_icons_on;

    extern int16_t _fleet_icon_buttons_added;
    extern int16_t _fleet_box_outpost_button;
    extern int16_t _fleet_box_attack_button;
    extern int16_t _fleet_box_scroll_dn_button;
    extern int16_t _fleet_box_transport_button;
    extern int16_t _fleet_box_engage_button;
    extern int16_t _fleet_box_scroll_up_button;

    extern s_animation_header* _fleet_box_name_lg_seg;

    extern s_animation_header* _fleet_box_name_sm_seg;

    extern s_animation_header* _fleet_box_bottom_lg_seg;

    extern s_animation_header* _fleet_box_close_lg_button_seg;
    extern s_animation_header* _fleet_box_close_sm_button_seg;
    extern s_animation_header* _fleet_box_engage_lg_button_seg;
    extern s_animation_header* _fleet_box_engage_sm_button_seg;
    extern s_animation_header* _fleet_box_colonize_lg_button_seg;
    extern s_animation_header* _fleet_box_colonize_sm_button_seg;
    extern s_animation_header* _fleet_box_transport_lg_button_seg;
    extern s_animation_header* _fleet_box_transport_sm_button_seg;
    extern s_animation_header* _fleet_box_outpost_lg_button_seg;
    extern s_animation_header* _fleet_box_outpost_sm_button_seg;
    extern s_animation_header* _fleet_box_att_xenons_lg_button_seg;
    extern s_animation_header* _fleet_box_att_xenons_sm_button_seg;

    extern s_animation_header* _fleet_box_bottom_sm_seg;

    extern s_animation_header* _fleet_box_grid_lg_seg;

    extern int16_t _fleet_movement_box_ptr[9];

    extern s_fleet_box _fleet_box;

    extern s_animation_header* _fleet_box_scroll_dn_button_seg;
    extern int16_t _fleet_movement_bar_field;
    extern s_animation_header* _fleet_box_scroll_up_button_seg;
    extern s_animation_header* _fleet_box_all_button_seg;

    extern int8_t _mouse_button_pressed;

    extern s_fleet_movement_icon _fleet_movement_icon[MAX_SHIPS];

    extern s_save_game_description _save_game_description[10];

    extern int16_t _popup_base_x;
    extern int16_t _popup_base_y;
    extern int16_t _screen_data;
    extern int16_t _screen_mouse_list_type;
    extern int16_t _temp_star_handle;

    // Suggested name: MOX::_gate_icon_seg
    extern s_animation_header* _gate_icon_seg[16];

    // Cached movable-box id preserved while reordering the z-order list.
    extern int16_t _moveable_box_reorder_fallback;

    // Suggested name: MOX::moveable_box_display_order
    extern int16_t _moveable_box_order[5];

    // Suggested name: MOX::box_visibility_flags
    extern int16_t _box_off_flag[5];

    extern s_saved_box _saved_boxes[5];

    extern uint8_t _fleet_box_scroll_first_ship;

    extern int16_t _galaxy_map_grid_field;

    extern int16_t _galaxy_map_height;

    extern int16_t _galaxy_map_grid_y;

    extern int16_t _galaxy_map_width;
    extern int16_t _galaxy_map_grid_x;

    extern int16_t _galaxy_map_star_field[MAX_STARS];

    extern s_animation_header* _fltscrn_flashing_ship_pict_seg[8];

    extern s_animation_header* _fltscrn_flashing_monster_ship_icon[7];

    /**
     * Copies persisted game-setup values into the active global settings.
     * @param settings Source settings record loaded from save data.
     */
    void __cdecl Apply_Save_Settings(s_settings* settings);
}