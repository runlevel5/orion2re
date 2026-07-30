#include "pch.h"

namespace MOX {
    const char _ship_type_names[5][6][16] = {
        {
            "combat ships",
            "not used",
            "",
            "",
            "",
            "",
        },
        {
            "Colony Ship",
            "Kolonieschiff",
            "Colonial",
            "Nave Colonia",
            "Nave colonia",
            "Colony Ship",
        },
        {
            "Transport",
            "Transporter",
            "Transport",
            "Transporte",
            "Nave truppa",
            "Transport",
        },
        {
            "tanks",
            "not used",
            "",
            "",
            "",
            "",
        },
        {
            "Outpost Ship",
            "Au|enposten",
            "Avante-poste",
            "Puesto Avanzado",
            "Nave avamposto",
            "Outpost Ship",
        },
    };

    int8_t _tech_categories[51][2] = {
        {50, 1},
        {50, 1},
        {50, 1},
        {50, 1},
        {50, 1},
        {20, 1},
        {20, 1},
        {20, 1},
        {10, 1},
        {50, 1},
        {20, 0},
        {5, 1},
        {10, 1},
        {50, 0},
        {50, 0},
        {10, 1},
        {10, 0},
        {50, 1},
        {20, 0},
        {20, 0},
        {5, 0},
        {20, 0},
        {50, 1},
        {10, 0},
        {10, 0},
        {10, 0},
        {20, 0},
        {5, 1},
        {20, 0},
        {5, 1},
        {10, 0},
        {10, 1},
        {50, 0},
        {50, 0},
        {10, 1},
        {5, 1},
        {10, 0},
        {5, 0},
        {5, 0},
        {50, 0},
        {50, 1},
        {5, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
    };

    const char s_combat_lbx[11] = "combat.lbx";

    uint8_t _first_field_in_group[10] = {0, 18, 55, 57, 29, 7, 22, 28, 10, 74};

    s_animation_header* _fighter_seg[15][64];
    uint8_t _draw_officer_to_back;
    int16_t _ordnance_bonus[15];
    int16_t _n_fltscrn_big_icons_added;
    int16_t _n_fltscrn_big_icons;
    int8_t _n_fltscrn_rows;
    int16_t _old_small_ship_stack_ptr;
    int16_t _old_first_fltscrn_icon;
    int16_t _n_big_icons_selected;
    uint8_t _galaxy_star_anim_count;
    int16_t _fltscrn_bar_value;
    int8_t _big_icon_display_columns;
    int8_t _big_icon_display_rows;
    uint16_t _ken_trace;
    int8_t _big_icon_display_max_icons;
    int16_t _big_icon_x_offset;
    int16_t _big_icon_y_offset;
    int16_t _offpop_big_icon_selected;
    uint8_t _drawing_officer_popup;
    int16_t* _fltscrn_ship_icon_selection_list;
    uint8_t _look_at_colonies;
    s_big_icon* _fltscrn_big_icon;
    uint8_t _add_help_fields;
    uint8_t _first_time;
    int16_t _fleet_movement_bar_value;
    uint8_t _draw_expanding_box;
    uint8_t _mainscr_palette;
    s_scroll_bar* _scroll_bar;
    int16_t _fltscrn_stack_owner;
    uint8_t _add_scrap_button;
    uint8_t _skip_draw_ships;
    uint8_t _fix_palette;
    s_animation_header* _scrap_button_seg;
    s_animation_header* _colony_marker_seg[MAX_PLAYERS];
    s_animation_header* _outpost_marker_seg[MAX_PLAYERS];
    s_animation_header* _o_highlight_seg;
    uint8_t _add_all_button;
    s_animation_header* _fleet_all_button_seg;
    s_animation_header* _fleet_return_button_seg;
    int16_t _old_fltscrn_stack_owner;
    s_animation_header* _up_scroll_button_seg;
    s_animation_header* _down_scroll_button_seg;
    s_animation_header* _prev_fleet_button_seg;
    s_animation_header* _next_fleet_button_seg;
    s_animation_header* _relocate_fleet_button_seg;
    s_animation_header* _officers_button_seg;
    uint8_t _add_officers_button;
    int16_t _fltscrn_support_button_status;
    int16_t _fltscrn_combat_button_status;
    s_mp_screen* _mp_screen;
    int8_t _player_mode[15];
    int16_t _cur_ship;
    int16_t _net_destination;
    int16_t _done_button;
    int16_t _msl_impact_warning_button;
    int16_t _draw_legal_boxes_button;
    int16_t _msl_warning_button;
    int16_t _wait_button;
    int16_t _scan_button;
    int16_t _missile_field[300];
    int16_t _combat_grid[COMBAT_GRID_WIDTH][COMBAT_GRID_HEIGHT];
    int16_t _weapon_mod_field_status[WEAPON_MOD_COUNT];
    int16_t _weapon_button[8];
    int16_t _ship_turn_order[210];
    int16_t _repl_fields[10];
    int16_t _side_end_of_turn[15];
    int16_t _unused_ship_turn_order_marker;
    int16_t _design_choice_items[40];
    int16_t _weapon_mod_fields[WEAPON_MOD_COUNT];
    int16_t _weapon_arc_field[5];
    int16_t _weapon_rack_field[5];
    int16_t _multi_colored_box_color_start;
    int16_t _beam_filter_button;
    int16_t _missile_filter_button;
    int16_t _special_filter_button;
    int16_t _replacement_box_accept_button;
    int16_t _bomb_filter_button;
    const char s__00551100[1] = "";

    uint16_t _fleet_box_map_height;
    s_current_design* _design;

    int16_t _scanned_big_ship;
    int16_t _small_ship_stack_ptr;
    int16_t _scanned_small_ship;
    int16_t _n_fltscrn_officers;
    int16_t* _officer_id_list;
    s_animation_header* _star_colors_seg[11];
    s_animation_header* _fleet_galaxy_star_seg[11];
    s_animation_header* _colony_galaxy_star_seg[10];
    s_animation_header* _officer_skill_icon_seg[27];
    s_animation_header* _hl_antaran_ship_icon_seg[24];
    s_animation_header* _antaran_ship_icons[24];
    s_animation_header* _o_ring_seg[5];
    s_animation_header* _planet_mask_seg[5];
    s_animation_header* _fltscrn_small_ship_icon_seg[8];
    s_animation_header* _monster_ship_seg[4];
    s_animation_header* _hl_monster_ship_icon_seg[4];
    s_animation_header* _ship_icon_seg[32];
    s_animation_header* _hl_ship_icon_seg[32];
    s_animation_header* _fleet_background_seg;
    void* _officer_pictures_screen_seg;
    s_animation_header* _officer_main_seg;
    s_animation_header* _officer_colony_box_seg;
    s_animation_header* _officer_colony_ldrs_button_seg;
    s_animation_header* _officer_ship_ldrs_button_seg;
    s_animation_header* _officer_up_button_seg;
    s_animation_header* _officer_dn_button_seg;
    s_animation_header* _officer_prev_button_seg;
    s_animation_header* _officer_next_button_seg;
    s_animation_header* _officer_hire_button_seg;
    s_animation_header* _officer_return_button_seg;
    s_animation_header* _officer_cancel_hire_button_seg;
    s_animation_header* _selected_box_seg;
    s_animation_header* _scanned_box_seg;
    s_animation_header* _fleet_scroll_anim_seg;
    s_animation_header* _officer_normal_window_seg;
    s_animation_header* _officer_bright_window_seg;
    s_animation_header** _temp_officers_pict;
    s_animation_header* _officer_dismiss_button_seg;
    s_animation_header* _officer_fleet_box_seg;
    s_animation_header* _dull_officer_dismiss_button_seg;
    s_animation_header* _dull_officer_pool_button_seg;
    s_animation_header* _officer_pool_button_seg;
    s_animation_header* _dull_officer_hire_button_seg;
    s_animation_header* _officer_hire_replace_seg;
    int8_t _officer_scanned;
    int8_t _officer_scrn_type;

    int8_t _officer_selected;
    int16_t _special_scroll_dn_button;
    int8_t _n_weapons_loaded;
    uint8_t _n_special_systems;
    int8_t* _fltscrn_icon_selection_status;
    uint8_t _star_class_table[7][3] = {
        {20, 10, 5},
        {25, 15, 5},
        {10, 16, 30},
        {10, 16, 21},
        {32, 37, 30},
        {1, 2, 3},
        {2, 4, 6},
    };
    int8_t _food_per_farmer_table[10] = {0, 0, 0, 1, 1, 2, 2, 1, 2, 3};
    int16_t _ranged_to_hit_penalty[9] = {0, 0, -10, -20, -30, -40, -55, -70, -85};
    int16_t _ranged_damage_penalty[9] = {0, 0, -10, -20, -30, -40, -50, -60, -65};
    int16_t _cur_missile_count;
    int16_t _NUM_STARS;
    s_scroll_bar _fltpop_scroll_bar;
    int16_t _cur_map_scale;
    int16_t _cur_map_x;
    int16_t _cur_map_y;
    uint16_t _global_esc;
    int16_t _return_screen;
    char* _save_game_dates;
    char* _save_game_stardates;
    char* _star_name_loaded_string;
    int16_t _scanned_field;
    int8_t _previous_screen;
    int16_t _max_debug_saves;
    int16_t _first_debug_save;
    int16_t _debug_saves_on;
    uint8_t _turns_between_debug_saves;
    void* _sound_buffer;
    int16_t _hot_seat_flag;
    int16_t _current_screen;
    int16_t _design_screen_replacement_type;
    int16_t _design_screen_replacement_slot;
    int16_t _weapons_field[8];
    s_animation_header* _asteroid_seg;
    s_animation_header* _screen_seg;
    s_animation_header* _diplomacy_request_popup_seg;
    s_animation_header* _combat_bitmap;
    s_animation_header* _combat_map_planet_seg;
    s_animation_header* _combat_planet_seg;
    s_animation_header* _combat_planet_seg2;
    uint8_t _ship_darken_map[128][16];
    uint8_t _given_chance_to_rename_home_star[MAX_PLAYERS];
    uint8_t _player_has_ship_in_system;
    s_animation_header* _moveable_box_pict[25];
    int8_t _game_type;
    s_antaran _antarans;
    int8_t _maps_to_generate;
    uint8_t _auto_turn_exit;
    int16_t _special_scroll_up_button[2];
    uint8_t _blue_color2;
    int16_t _auto_msl_warning_off;
    s_animation_header* _draw_ship_bitmap;
    int16_t _multi_colored_line_start;
    int16_t _design_choice_item_selected[40];
    int8_t _first_special_system_ctr;
    int16_t _special_system_field[8];
    int16_t _planet_buttons[5];
    uint8_t _first_design_screen_call;
    int16_t _design_size;
    uint8_t _refresh_mode;
    uint8_t _mox_font_colors_offset;
    int16_t _star_width;
    int16_t _star_height;
    char _temp_string[512];
    char _temp_string_buf[64];
    int16_t _take_control_button;
    int16_t _auto_cmbt_button_value;
    int16_t _PLAYER_NUM;
    uint8_t _active_player[9];
    int16_t _active_count;
    int16_t _ai_retreat_flag;
    int16_t _bio_weapon_flag;
    int16_t _last_ship_status;
    int16_t _ship_status_display;
    int16_t _dont_draw_ship;
    int16_t _draw_active_missiles;
    int16_t _order_info_flag;
    int16_t _super_fast_flag;
    int8_t _real_attack_flag;
    int16_t _shields_flag;
    int16_t _msl_impact_warning_flag;
    int16_t _draw_legal_boxes_flag;
    int16_t _take_control_flag;
    int16_t _combat_far_x;
    int16_t _combat_mid_x;
    int16_t _combat_far_y;
    int16_t _combat_mid_y;
    int16_t _net_flag;
    int16_t _in_nebula_flag;
    int16_t _skan_flag;
    uint8_t _draw_design_screen_to_back;
    uint8_t _refit_ship;
    int32_t _original_cost;
    int16_t _design_name_field;
    int16_t _design_cancel_button;
    int16_t _design_clear_button;
    int16_t _design_build_button;
    s_player* _player;
    s_player* _cur_plyr_ptr;
    char* _personality_labels[7];
    char* _objective_labels[6];
    char* _treaty_labels[7];
    char* _help_category_labels[17];
    int16_t _old_system_index;
    int8_t _selected_star_color_mod;
    int8_t _selected_star_color_offset;
    int32_t _stardate;
    s_settings _settings;
    char* _hyper_field_title;
    const char s_Harbinger[12] = "Harbinger";
    uint8_t _at_least_one_tactical_combat;
    s_animation_header* _planet_summary_galaxy_star_seg[11];
    int8_t _first_weapon_ctr;
    int16_t _fleet_box_colonize_button;
    uint8_t _reloaded_ship_palette;
    uint8_t _g_galaxy_age;
    int16_t _ship_data_id;
    int16_t _cur_combat_x;
    int16_t _cur_ptr;
    int16_t _wait_ptr;
    int16_t _board_flag;
    int16_t _tactical_net_choice;
    int16_t _auto_button;
    int16_t _auto_combat;
    int16_t _speedx2_button;
    int16_t _grid_button;
    int16_t _shields_button;
    int16_t _super_fast_button;
    int16_t _self_destruct_button;
    int16_t _weapons_button;
    int16_t _specials_button;
    int16_t _cmbt_info_button;
    int16_t _flee_button;
    int16_t _board_button;
    int16_t _map_field;
    int16_t _map_y;
    int16_t _map_x;
    int16_t _center_button;
    int16_t _space_field;
    int16_t _ypos;
    int16_t _xpos;
    int16_t _saved_msl_warn;
    uint8_t _red_color_pulse_value;
    int8_t _red_color_pulse_incr;
    int16_t _fleet_ship_scanned;
    int16_t _fleet_movement_all_button;
    int16_t _galaxy_map_scanned_star;
    int8_t _draw_system_display_without_button;
    uint8_t _need_to_restore_help_list;
    int16_t _ship_icon_player_id;
    int8_t _at_least_one_report;
    uint8_t _previous_report;
    int16_t _first_node_ptr_in_stack_;
    int16_t _system_display_ring_radius[5];
    uint8_t _rot_plan_dim[5];
    int16_t _system_star_id;
    int16_t _system_star_id_for_auto_select;
    uint8_t _draw_fleet_screen_to_back;
    s_mouse_xy _mouse_XY;
    int16_t _system_display_ship_scanned;
    int16_t _system_icon_scanned;
    uint16_t _global_exit_field;
    int16_t _eta_star_id;
    int16_t _stencil_box_id;
    int16_t _draw_main_screen_stencilled;
    int16_t _animated_star;
    int8_t _just_moved_stack;
    int16_t _first_fleet_movement_icon;
    int16_t _next_free_node;
    int16_t _mid_star_x;
    int16_t _mid_star_y;
    int16_t _design_screen_replacement_item;
    int16_t _far_star_x;
    int16_t _far_star_y;
    int16_t _near_star_y;
    int16_t _near_star_x;
    const char s_fmt_contested[15] = "%s (contested)";
    void* _special_names_seg;
    void* _global_data_seg;
    s_nebula* _nebula;
    uint8_t _NUM_NEBULAS;
    int16_t _NUM_COLONIES;
    int16_t _planet_count;
    int8_t _climate_modifier_table[5] = {0, 0, 0, -10, -20};
    s_planet_data* _planet;
    const char* _roman_literals[21] = {
        nullptr,
        "I",
        "II",
        "III",
        "IV",
        "V",
        "VI",
        "VII",
        "VIII",
        "IX",
        "X",
        "XI",
        "XII",
        "XIII",
        "XIV",
        "XV",
        "XVI",
        "XVII",
        "XVIII",
        "XIX",
        "XX",
    };
    char* _planet_special_string[12];
    char* _planet_climate_string[10];
    char* _mineral_class_string[5];
    uint8_t _red_color2;
    char* _planet_size_string[5];
    char* _planet_type_string[10];
    char* _planet_group_string[4];
    char* _planet_atmosphere_string[3];
    char* _spectral_class_string[7];
    char* _admin_skill_names[9];
    char* _planet_gravity_string[3];
    int16_t _NUM_SHIPS;
    void* _global_bitmap_seg;
    uint8_t _first_time_drawn;
    uint8_t _draw_main_screen_to_back;
    uint8_t _dont_draw_main_screen_to_back;
    int16_t _zoom_count;
    int16_t _max_zoom_count;
    uint8_t _star_bg_index;
    s_animation_header* _star_bg_seg;
    void* _scan_ship_seg;
    s_animation_header* _detail_fleet_icon_seg;
    uint16_t _fleet_icon_button_count;
    int16_t _weapons_count_plus_button[8];
    int16_t _weapons_count_minus_button[8];
    int16_t _design_ship_size_field[6];
    void* _techname_seg;
    void* _tech_desc_seg;
    s_animation_header* _main_screen_seg;
    s_animation_header* _design_screen_seg;
    s_animation_header* _design_icon_lf_arrow_seg;
    s_animation_header* _design_icon_rt_arrow_seg;
    s_animation_header* _clear_design_button_seg;
    s_animation_header* _cancel_design_button_seg;
    s_animation_header* _build_design_button_seg;
    s_animation_header* _plus_button_seg;
    s_animation_header* _minus_button_seg;
    s_animation_header* _d_scroll_up_button_seg;
    s_animation_header* _d_scroll_dn_button_seg;
    s_animation_header* _dull_d_scroll_up_button_seg;
    s_animation_header* _dull_d_scroll_dn_button_seg;
    s_animation_header* _beam_filter_button_seg;
    s_animation_header* _missile_filter_button_seg;
    s_animation_header* _bomb_filter_button_seg;
    s_animation_header* _special_filter_button_seg;
    s_animation_header* _weapons_cancel_button_seg;
    s_animation_header* _weapons_accept_button_seg;
    s_animation_header* _weapons_top_box_seg;
    s_animation_header* _weapons_mid_box_seg;
    s_animation_header* _weapons_bot_box_seg;
    s_animation_header* _weapons_selected_seg;
    s_animation_header* _weapons_blank_mods_seg;
    s_animation_header* _system_repl_scroll_bar_seg;
    s_animation_header* _system_repl_box_top_seg;
    s_animation_header* _system_repl_box_mid_seg;
    s_animation_header* _system_repl_box_btm_seg;
    s_animation_header* _system_repl_box_cancel_btn;
    int16_t _weapons_clear_button[8];
    int16_t _special_system_clear_button[8];
    s_animation_header* _design_size_button_seg[6];
    s_animation_header* _darkened_size_button_seg[6];
    s_animation_header* _weapons_arc_pict_seg[5];
    s_animation_header* _weapons_arc_text_seg[5];
    s_animation_header* _weapons_rack_text_seg[5];
    s_animation_header* _weapons_special_seg[18];
    s_weapon_filter_item* _weapon_filter;
    char* _special_desc_string[SPECIAL_COUNT - 1];
    char* _weapon_notes_string[40];
    s_animation_header* _d_scroll_bar_seg;
    s_animation_header* _weapons_rack_seg;
    s_animation_header* _mainscrn_diplo_lights_seg;
    s_animation_header* _diplomacy_request_wait_seg;
    s_animation_header* _diplomacy_request_invoke_seg;
    s_animation_header* _diplomacy_request_accept_seg;
    void* _global_screen_seg;
    void* _global_cache_seg;
    s_animation_header* _mid_star_seg;
    s_animation_header* _far_star_seg;
    uint8_t _fleet_icon_selection_status[BITMAP(MAX_SHIPS)];
    s_animation_header* _star_seg[40];
    s_animation_header* _sun_seg[7];
    uint16_t _n_stats_to_generate;
    uint32_t _global_cache_size;
    uint32_t _cmd_line_seed;
    int16_t _ship_icon_lf_button;
    int16_t _ship_icon_rt_button;
    uint8_t _zoom_in;
    s_animation_header* _main_zoom_in_button_seg;
    s_animation_header* _main_zoom_out_button_seg;
    s_animation_header* _main_game_button_seg;
    s_animation_header* _main_next_button_seg;
    s_animation_header* _main_colonies_button_seg;
    s_animation_header* _main_planets_button_seg;
    s_animation_header* _main_fleets_button_seg;
    s_animation_header* _main_leaders_button_seg;
    s_animation_header* _main_races_button_seg;
    s_animation_header* _main_info_button_seg;
    int16_t _star_button[MAX_STARS];
    int16_t _main_screen_grid_field;
    int16_t _main_screen_grid_x;
    int16_t _main_screen_grid_y;
    int16_t _NUM_PLAYERS;
    uint8_t _red_color1;
    char* _government_labels[8];
    char* _special_ability_names[32];
    int32_t _stardate_at_beg_of_turn;
    int8_t _global_var;
    uint16_t _FULL_DRAW_SCREEN_FLAG;
    uint16_t _combat_init;
    void* _warp_core_remap_colors;
    s_mouse_list* _combat_mouse_grid;
    void* _stasis_bubble_remap_colors;
    s_animation_header* _combat_background_seg;
    s_animation_header* _combat_far_star;
    s_animation_header* _combat_mid_star;
    s_animation_header* _combat_near_star;
    uint16_t _apply_damage_to_planet;
    uint16_t _missile_warning;
    void* _cmbt_option_seg;
    s_animation_header* _warning_missile_seg;
    void* _self_destruct_seg;
    uint16_t _saved_msl_warning;
    void* _auto_button_seg;
    s_animation_header* _combat_options_background_seg;
    int16_t _cur_combat_y;
    int16_t _grid_flag;
    int16_t _speedx2_flag;
    int16_t _auto_combat_flag[2];
    int16_t _combat_clock_frame;
    int16_t _combat_near_x;
    int16_t _combat_near_y;
    void* _cmbt_done_button_seg;
    void* _level_symbol_seg;
    void* _scan_button_seg;
    void* _board_button_seg;
    void* _flee_button_seg;
    void* _wait_button_seg;
    void* _cmbt_info_button_seg;
    void* _weapons_button_seg;
    void* _specials_button_seg;
    s_animation_header* _msl_warning_on_seg;
    void* _warning_fighter_seg;
    s_animation_header* _ship_bitmap;
    s_animation_header* _darken_plasma_map;
    void* _shield_remap_colors;
    void* _pulsar_remap_colors;
    s_animation_header* _small_tractor_seg;
    void* _tractor_remap_colors;
    void* _bhg_remap_colors;
    void* _plasma_web_remap_colors;
    void* _stellar_converter_remap_colors;
    s_animation_header* _msl_seg[112];
    s_animation_header* _ship_cursor[4];
    s_animation_header* _shield_div_bitmap[4];
    s_animation_header* _fltscrn_monster_ship_icon[7];
    s_animation_header* _shield_mask_bitmap[16];
    int16_t _fighter_beam_type[9];
    int16_t _fighter_bomb_type[9];
    s_combat_data* _combat_data;
    s_missile* _missile;
    int16_t _missile_hit_radius[6] = {12, 20, 26, 28, 30, 30};
    s_net_tactical_combat_message _new_message;

    int16_t _gate_icon_field[4];
    int16_t _system_display_base_x;
    int16_t _system_display_base_y;
    s_sys_disp* _sys_disp;
    s_system_display_ships _system_display_ships[15];
    int16_t _system_display_grid_y[6];
    int16_t _system_display_grid_x[6];
    int16_t _system_display_grid_field;
    char* _race_names[20];
    int8_t _class_to_num_satellites[10][6] = {
        {0, 0, 1, 2, 0, 0},
        {1, 1, 2, 2, 1, 0},
        {1, 1, 2, 2, 1, 0},
        {2, 1, 2, 3, 1, 0},
        {3, 2, 3, 3, 2, 0},
        {3, 2, 3, 4, 2, 0},
        {4, 3, 4, 4, 2, 0},
        {4, 3, 4, 5, 3, 1},
        {5, 4, 5, 5, 3, 1},
        {5, 4, 5, 5, 4, 1},
    };
    uint16_t _star_fields_dim[6];

    s_ship_data* _ship;

    int16_t _combat_defender_count;
    int16_t _combat_attacker_count;
    s_qship* _qcombat_data;
    int16_t _combat_total_ship_count;
    uint8_t _combat_done_waiting;
    s_colors _combat_green = {{0x81, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89}};
    int16_t _combat_attacker_player;
    int16_t _combat_turn;
    uint8_t _colony_defense_flags;
    int16_t _cmbt_colony_id;
    int16_t _combat_defender_player;
    s_combat_bomb_result _combat_bomb_result;

    s_combat_data _global_combat_data;

    int16_t* _officers_original_star_id;

    int16_t* _officers_original_ship_id;

    void* _dimmed_all_button_seg;
    void* _dimmed_relocate_button_seg;
    void* _dimmed_scrap_button_seg;
    void* _dimmed_officers_button_seg;

    s_leader_data* _leaders;

    char* _officer_level_names[6];

    char* _star_officer_level_names[6];

    s_skill_data _skill_data[54] = {
        {0, 0x1, 0, 20, 1, 2, nullptr, "%d%%"},
        {1, 0x2, 0, 30, 1, 4, nullptr, "%d%%"},
        {2, 0x4, 0, 20, 1, 0, nullptr, "+%d"},
        {3, 0x8, 0, 30, 1, 0, nullptr, "+%d"},
        {4, 0x10, 0, 100, 1, 1, nullptr, "+%d%"},
        {5, 0x20, 0, 150, 1, 2, nullptr, "+%d%"},
        {6, 0x1, 1, 20, 1, 3, nullptr, "%d%%"},
        {7, 0x2, 1, 30, 1, 6, nullptr, "%d%%"},
        {8, 0x1, 2, 100, 1, 2, nullptr, "-%d%%"},
        {9, 0x2, 2, 150, 1, 4, nullptr, "-%d%%"},
        {10, 0x40, 0, 600, 1, 2, nullptr, "-%dBC"},
        {11, 0x80, 0, 900, 1, 4, nullptr, "-%dBC"},
        {12, 0x4, 2, 100, 1, 3, nullptr, "+%d%%"},
        {13, 0x8, 2, 150, 1, 6, nullptr, "+%d%%"},
        {14, 0x4, 1, 50, 1, 3, nullptr, "+%d"},
        {15, 0x8, 1, 75, 1, 6, nullptr, "+%d"},
        {16, 0x10, 2, 100, 1, 3, nullptr, "+%d%%"},
        {17, 0x20, 2, 150, 1, 6, nullptr, "+%d%%"},
        {18, 0x10, 1, 50, 1, 2, nullptr, "+%d"},
        {19, 0x20, 1, 75, 1, 4, nullptr, "+%d"},
        {20, 0x40, 1, 50, 1, 4, nullptr, "+%d"},
        {21, 0x80, 1, 75, 1, 8, nullptr, "+%d"},
        {22, 0x40, 2, 10, 1, 1, nullptr, "+%d"},
        {23, 0x80, 2, 15, 1, 2, nullptr, "+%d"},
        {24, 0x100, 2, 100, 1, 3, nullptr, "+%d%%"},
        {25, 0x200, 2, 150, 1, 6, nullptr, "+%d%%"},
        {26, 0x400, 2, 100, 1, 2, nullptr, "+%d%%"},
        {27, 0x800, 2, 150, 1, 4, nullptr, "+%d%%"},
        {28, 0x100, 0, 100, 1, 1, nullptr, "+10BC"},
        {29, 0x200, 0, 100, 1, 2, nullptr, "+15BC"},
        {30, 0x100, 1, 10, 2, 1, nullptr, "+%d"},
        {31, 0x200, 1, 15, 2, 2, nullptr, "+%d"},
        {32, 0x400, 0, 20, 1, 2, nullptr, "+%d"},
        {33, 0x800, 0, 30, 1, 4, nullptr, "+%d"},
        {34, 0x400, 1, 50, 1, 3, nullptr, "+%d"},
        {35, 0x800, 1, 75, 1, 6, nullptr, "+%d"},
        {36, 0x1000, 0, 50, 1, 2, nullptr, "+%d"},
        {37, 0x2000, 0, 75, 1, 4, nullptr, "+%d"},
        {38, 0x1000, 2, 100, 1, 3, nullptr, "+%d%%"},
        {39, 0x2000, 2, 150, 1, 6, nullptr, "+%d%%"},
        {40, 0x1000, 1, 20, 1, 1, nullptr, "+%d"},
        {41, 0x2000, 1, 30, 1, 2, nullptr, "+%d"},
        {42, 0x4000, 2, 50, 1, 4, nullptr, "+%d%%"},
        {43, 0x8000, 2, 75, 1, 8, nullptr, "+%d%%"},
        {44, 0x4000, 0, 20, 1, 3, nullptr, "+%d%%"},
        {45, 0x8000, 0, 30, 1, 6, nullptr, "+%d%%"},
        {46, 0x10000, 2, 20, 1, 2, nullptr, "+%d"},
        {47, 0x20000, 2, 30, 1, 4, nullptr, "+%d"},
        {48, 0x10000, 0, 20, 1, 2, nullptr, "+%d%%"},
        {49, 0x20000, 0, 30, 1, 4, nullptr, "+%d%%"},
        {50, 0x40000, 0, 100, 1, 2, nullptr, "+%d%%"},
        {51, 0x80000, 0, 150, 1, 4, nullptr, "+%d%%"},
        {52, 0x4000, 1, 50, 1, 3, nullptr, "+%d"},
        {53, 0x8000, 1, 75, 1, 6, nullptr, "+%d"},
    };

    s_colony* _colony;

    s_animation_header* _darkened_turn_button_seg;

    s_animation_header* _fleet_box_selection_seg;
    int16_t _ship_frame;
    s_animation_header* _mainscrn_plyr_done_lights_seg;
    uint16_t _skip_fields;

    uint8_t _ship_icon_width[4];
    uint8_t _ship_icon_height[4];

    s_star_data* _star;

    uint8_t _orbit_to_satellite_type[10][5] = {
        {1, 1, 1, 1, 1},
        {4, 1, 1, 1, 2},
        {3, 2, 1, 2, 2},
        {3, 3, 2, 2, 2},
        {3, 3, 2, 2, 2},
        {3, 3, 3, 3, 2},
        {3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3},
    };

    s_ship_node _ship_node[MAX_SHIPS];

    int16_t _ship_stack_count;

    int16_t _ship_stack_start[MAX_SHIPS];

    uint8_t _fleet_box_ship_ids[BITMAP(MAX_SHIPS)];

    int8_t _special_bonus[9][3] = {
        {-50, 50, 100},
        {-1, 2, 4},
        {-1, 1, 2},
        {-1, 1, 2},
        {-1, 1, 2},
        {-20, 25, 50},
        {-20, 20, 50},
        {-10, 10, 20},
        {-10, 10, 20},
    };
    void* _other_ships_screen_seg;
    void* _fleet_box_grid_sm_seg4;
    int16_t _icon_overlay_y;
    int16_t _ship_icon_count;
    int16_t _icon_overlay_x;
    int16_t _fleet_box_ship_stack;
    s_ship_icon _ship_icon[SHIP_ICON_COUNT];
    int8_t _black_hole_anim_count[MAX_STARS];
    int16_t _bill_savegame[6] = {2, 0, 0, 0, 0, 0};
    uint16_t _antaran_resource_level;
    s_crew_data _crew_data[5] = {
        {0, 0, 0, 0},
        {50, 15, 15, 5},
        {150, 30, 30, 10},
        {500, 50, 50, 15},
        {1000, 75, 75, 20},
    };
    uint16_t _chance_for_antaran_invasion;
    uint16_t _disallow_saves_code;
    int16_t _max_map_scale;
    int16_t _MAP_MAX_X;
    int16_t _MAP_MAX_Y;
    uint8_t _g_galaxy_size;
    void* _nebula_screen_seg;
    s_animation_header* _nebula_pict_seg[12][4];
    uint8_t _planet_max_farms[5] = {2, 4, 5, 7, 10};
    int8_t _planet_max_population[5] = {5, 10, 15, 20, 25};
    uint8_t _planet_size_table[5] = {1, 3, 7, 9, 10};
    s_class_group _class_to_group[6] = {
        {{0, 0, 0, 0, 1}},
        {{0, 0, 1, 2, 3}},
        {{0, 1, 2, 2, 3}},
        {{1, 2, 2, 2, 3}},
        {{1, 2, 3, 3, 3}},
        {{0, 0, 1, 2, 3}},
    };
    uint8_t _climate_roll_table[10][4] = {
        {15, 15, 10, 20},
        {55, 50, 15, 0},
        {25, 25, 10, 70},
        {5, 10, 10, 0},
        {0, 5, 10, 8},
        {0, 0, 10, 2},
        {0, 0, 11, 0},
        {0, 0, 11, 0},
        {0, 0, 11, 0},
        {0, 0, 2, 0},
    };
    uint8_t _normal_gal_climate_roll_table[10][4] = {
        {15, 15, 10, 20},
        {55, 50, 15, 0},
        {25, 25, 10, 70},
        {5, 10, 10, 0},
        {0, 5, 10, 8},
        {0, 0, 10, 2},
        {0, 0, 11, 0},
        {0, 0, 11, 0},
        {0, 0, 11, 0},
        {0, 0, 2, 0},
    };
    uint8_t _old_gal_climate_roll_table[10][4] = {
        {15, 5, 5, 20},
        {40, 30, 8, 0},
        {20, 20, 8, 50},
        {25, 25, 13, 0},
        {0, 20, 13, 30},
        {0, 0, 13, 0},
        {0, 0, 13, 0},
        {0, 0, 13, 0},
        {10, 0, 0, 0},
        {4, 0, 0, 0},
    };
    s_movable_box _moveable_box[5];
    uint8_t _fleet_movement_icons_on;
    int16_t _fleet_icon_buttons_added;
    int16_t _fleet_box_outpost_button;
    int16_t _fleet_box_attack_button;
    int16_t _fleet_box_scroll_dn_button;
    int16_t _fleet_box_transport_button;
    int16_t _fleet_box_engage_button;
    int16_t _fleet_box_scroll_up_button;
    s_animation_header* _fleet_box_name_lg_seg;
    s_animation_header* _fleet_box_name_sm_seg;
    s_animation_header* _fleet_box_bottom_lg_seg;
    s_animation_header* _fleet_box_close_lg_button_seg;
    s_animation_header* _fleet_box_close_sm_button_seg;
    s_animation_header* _fleet_box_engage_lg_button_seg;
    s_animation_header* _fleet_box_engage_sm_button_seg;
    s_animation_header* _fleet_box_colonize_lg_button_seg;
    s_animation_header* _fleet_box_colonize_sm_button_seg;
    s_animation_header* _fleet_box_transport_lg_button_seg;
    s_animation_header* _fleet_box_transport_sm_button_seg;
    s_animation_header* _fleet_box_outpost_lg_button_seg;
    s_animation_header* _fleet_box_outpost_sm_button_seg;
    s_animation_header* _fleet_box_att_xenons_lg_button_seg;
    s_animation_header* _fleet_box_att_xenons_sm_button_seg;
    s_animation_header* _fleet_box_bottom_sm_seg;
    s_animation_header* _fleet_box_grid_lg_seg;
    int16_t _fleet_movement_box_ptr[9];
    s_fleet_box _fleet_box;
    s_animation_header* _fleet_box_scroll_dn_button_seg;
    int16_t _fleet_movement_bar_field;
    s_animation_header* _fleet_box_scroll_up_button_seg;
    s_animation_header* _fleet_box_all_button_seg;
    int8_t _mouse_button_pressed;
    s_save_game_description _save_game_description[10];
    int16_t _popup_base_x;
    int16_t _popup_base_y;
    int16_t _screen_data;
    int16_t _screen_mouse_list_type;
    int16_t _temp_star_handle;
    s_animation_header* _gate_icon_seg[16];
    int16_t _moveable_box_reorder_fallback;
    int16_t _moveable_box_order[5];
    int16_t _box_off_flag[5];
    s_saved_box _saved_boxes[5];
    uint8_t _fleet_box_scroll_first_ship;
    int16_t _galaxy_map_grid_field;
    int16_t _galaxy_map_height;
    int16_t _galaxy_map_grid_y;
    int16_t _galaxy_map_width;
    int16_t _galaxy_map_grid_x;
    int16_t _galaxy_map_star_field[MAX_STARS];
    s_animation_header* _fltscrn_flashing_ship_pict_seg[8];
    s_animation_header* _fltscrn_flashing_monster_ship_icon[7];

    s_mouse_list _mouse_list_zoom_mode[5] = {
        {6, 0, 22, 22, 526, 420},
        {1, 0, 0, 0, 639, 21},
        {1, 0, 0, 0, 21, 479},
        {1, 0, 527, 0, 639, 479},
        {1, 0, 0, 421, 639, 479},
    };
    uint8_t _main_palette_player_colors[MAX_PLAYERS] = {73, 98, 110, 32, 62, 148, 45, 85};
    int16_t _h_string_ix[397];
    char _h_strings[0x4074];
    s_help_box _help_entry_list[50];
    int8_t _class_to_mineral[60] = {
        2, 1, 1, 0, 0, 1,
        2, 1, 1, 1, 0, 2,
        2, 2, 1, 1, 1, 2,
        2, 2, 2, 1, 1, 2,
        3, 2, 2, 1, 1, 2,
        3, 2, 2, 2, 1, 2,
        3, 3, 2, 2, 2, 3,
        3, 3, 3, 2, 2, 3,
        4, 3, 3, 2, 2, 3,
        4, 4, 4, 3, 2, 4,
    };
    int8_t _planet_special = 64;
    uint8_t _planet_special_weighted_chance[12] = {64, 5, 3, 2, 3, 2, 9, 4, 3, 0, 5, 0};
    int8_t _gravity_table[30] = {
        0, 0, 0, 1, 1,
        0, 0, 1, 1, 1,
        0, 1, 1, 1, 2,
        1, 1, 1, 2, 2,
        1, 1, 2, 2, 2,
        0, 0, 0, 0, 0,
    };
    s_mouse_list _mouse_list_arrow = {1, 0, 0, 0, 639, 479};
    s_mouse_list _mouse_list_relocation = {2, 0, 0, 0, 639, 479};
    s_mouse_list _cmbt_mouse_list_clock = {1, 0, 0, 0, 639, 479};
    s_mouse_list _mouse_list_variable = {20, 0, 0, 0, 639, 479};
    s_mouse_list _mouse_list_none = {0, 0, 0, 0, 639, 479};
    s_animation_header* _near_star_seg;
    const char s_ATZ_0055AA7C[4] = "ATZ";
    const char s_AT_K0_M1_D2_0055AA6C[14] = "AT &K0 M1 &D2";
    const char s_AT_H0_S0_0_0055AA60[11] = "AT H0 S0=0";
    const char s_5551212_0055AA58[8] = "5551212";
    const char s_fmt_s[3] = "%s";
    const char s_fmt_scanned_star_info[8] = "%s (%s)";

    s_ship_move_info _g_ship_move_info;

    s_fleet_movement_icon _fleet_movement_icon[MAX_SHIPS];

    void __cdecl Apply_Save_Settings(s_settings* settings) {
        MOX::_settings.xenons_exist = settings->xenons_exist;
        MOX::_settings.game_difficulty = settings->game_difficulty;
        MOX::_settings.number_of_players = settings->number_of_players;
        MOX::_settings.galaxy_size = settings->galaxy_size;
        MOX::_settings.galaxy_age = settings->galaxy_age;
        MOX::_settings.strategic_combat_flag = settings->strategic_combat_flag;
        MOX::_settings.starting_civilization_level = settings->starting_civilization_level;
        MOX::_settings.start_of_game_seed = settings->start_of_game_seed;
        MOX::_settings.random_events = settings->random_events;
    }
}