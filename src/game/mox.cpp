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
    s_animation_header* _colony_marker_seg[8];
    s_animation_header* _outpost_marker_seg[8];
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
    short _fltscrn_support_button_status;
    short _fltscrn_combat_button_status;
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
    int16_t _combat_grid[81][68];
    int16_t _weapon_mod_field_status[15];
    int16_t _weapon_button[8];
    int16_t _ship_turn_order[210];
    int16_t _repl_fields[10];
    int16_t _side_end_of_turn[15];
    int16_t DAT_0059730e;
    int16_t _design_choice_items[40];
    int16_t _weapon_mod_fields[15];
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
    int16_t _cur_missile_count;
    int16_t _NUM_STARS;
    s_scroll_bar _fltpop_scroll_bar;
    int16_t _cur_map_scale;
    int16_t _cur_map_x;
    int16_t _cur_map_y;
    uint16_t _global_esc;
    short _return_screen;
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
    short _current_screen;
    int16_t _design_screen_replacement_type;
    int16_t _design_screen_replacement_slot;
    s_animation_header* _asteroid_seg;
    s_animation_header* _screen_seg;
    s_animation_header* _diplomacy_request_popup_seg;
    s_animation_header* _combat_bitmap;
    s_animation_header* _combat_map_planet_seg;
    s_animation_header* _combat_planet_seg;
    s_animation_header* _combat_planet_seg2;
    uint8_t _player_has_ship_in_system;
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
    uint8_t _first_design_screen_call;
    int16_t _design_size;
    uint8_t _refresh_mode;

    s_fleet_movement_icon _fleet_movement_icon[500];

    void __cdecl Apply_Save_Settings(s_settings* s) {
        MOX::_settings.xenons_exist = s->xenons_exist;
        MOX::_settings.game_difficulty = s->game_difficulty;
        MOX::_settings.number_of_players = s->number_of_players;
        MOX::_settings.galaxy_size = s->galaxy_size;
        MOX::_settings.galaxy_age = s->galaxy_age;
        MOX::_settings.strategic_combat_flag = s->strategic_combat_flag;
        MOX::_settings.starting_civilization_level = s->starting_civilization_level;
        MOX::_settings.start_of_game_seed = s->start_of_game_seed;
        MOX::_settings.random_events = s->random_events;
    }
}