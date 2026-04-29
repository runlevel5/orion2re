#pragma once

namespace MOX {
    inline int8_t (*_tech_categories)[2] = reinterpret_cast<int8_t(*)[2]>(std::uintptr_t{0x00558FD0});
    inline const char* s_combat_lbx = reinterpret_cast<const char*>(0x00551244);
    inline uint8_t (&_first_field_in_group)[10] = *reinterpret_cast<uint8_t(*)[10]>(0x00558D30);
    inline s_animation_header* (&_fighter_seg)[15][64] =
        *reinterpret_cast<s_animation_header* (*)[15][64]>(std::uintptr_t{0x005A1CC0});
    inline uint8_t& _draw_officer_to_back = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597838});
    inline int16_t (&_ordnance_bonus)[15] =
        *reinterpret_cast<int16_t(*)[15]>(std::uintptr_t{0x00597050});
    inline int16_t& _n_fltscrn_big_icons_added = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597018});
    inline int16_t& _n_fltscrn_big_icons = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005973E2});
    inline uint8_t& _n_fltscrn_rows = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597A92});
    inline int16_t& _old_small_ship_stack_ptr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BAA8});
    inline int16_t& _old_first_fltscrn_icon = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A183C});
    inline int16_t& _n_big_icons_selected = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1838});
    inline uint8_t& _galaxy_star_anim_count = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597AEA});
    inline int16_t& _fltscrn_bar_value = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596C5E});
    inline uint8_t& _big_icon_display_columns = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059BD14});
    inline uint8_t& _big_icon_display_rows = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005974E4});
    inline uint16_t& _ken_trace = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005974EE});
    inline uint8_t& _big_icon_display_max_icons = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597828});
    inline int16_t& _big_icon_x_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A2BEA});
    inline int16_t& _big_icon_y_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059784E});
    inline int16_t& _offpop_big_icon_selected = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059C456});
    inline uint8_t& _drawing_officer_popup = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596D50});
    inline int16_t*& _fltscrn_ship_icon_selection_list = *reinterpret_cast<int16_t**>(std::uintptr_t{ 0x0059b4a0 });
    inline uint8_t& _look_at_colonies = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A185F});
    inline s_big_icon*& _fltscrn_big_icon = *reinterpret_cast<s_big_icon**>(std::uintptr_t{0x00597460});
    inline uint8_t& _add_help_fields = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597024});
    inline uint8_t& _first_time = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059749C});
    inline int16_t& _fleet_movement_bar_value = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597820});
    inline uint8_t& _draw_expanding_box = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059BF32});
    inline uint8_t& _mainscr_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A1AF4});
    inline s_scroll_bar*& _scroll_bar = *reinterpret_cast<s_scroll_bar**>(std::uintptr_t{0x00597370});
    inline int16_t& _fltscrn_stack_owner = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059D6B6});
    inline uint8_t& _add_scrap_button = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005973E1});
    inline uint8_t& _skip_draw_ships = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059bef0 });
    inline uint8_t& _fix_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059baac });
    inline s_animation_header*& _scrap_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597824});
    inline s_animation_header* (&_colony_marker_seg)[8] = *reinterpret_cast<s_animation_header* (*)[8]>(std::uintptr_t{0x00597800});
    inline s_animation_header* (&_outpost_marker_seg)[8] = *reinterpret_cast<s_animation_header* (*)[8]>(std::uintptr_t{0x005A2BC0});
    inline s_animation_header*& _o_highlight_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597708});
    inline uint8_t& _add_all_button = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059B438});
    inline s_animation_header*& _fleet_all_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596D4C});
    inline s_animation_header*& _fleet_return_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597920});
    inline int16_t& _old_fltscrn_stack_owner =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597918});
    inline s_animation_header*& _up_scroll_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597914});
    inline s_animation_header*& _down_scroll_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059C45C});
    inline s_animation_header*& _prev_fleet_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BD10});
    inline s_animation_header*& _next_fleet_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596D20});
    inline s_animation_header*& _relocate_fleet_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059C450});
    inline s_animation_header*& _officers_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1820});
    inline uint8_t& _add_officers_button = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059B4BB});
    inline short& _fltscrn_support_button_status = *reinterpret_cast<short*>(std::uintptr_t{0x0059776C});
    inline short& _fltscrn_combat_button_status = *reinterpret_cast<short*>(std::uintptr_t{0x00597464});
    inline s_mp_screen*& _mp_screen = *reinterpret_cast<s_mp_screen**>(std::uintptr_t{0x0059C42C});
    inline s_network& _net = *reinterpret_cast<s_network*>(std::uintptr_t{0x00595170});
    inline int8_t (&_player_mode)[15] =
        *reinterpret_cast<int8_t(*)[15]>(std::uintptr_t{0x0059D650});
    inline int16_t& _start_of_turn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005a6e10});
    inline int16_t& _cur_ship = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059d6c0});
    inline int16_t& _net_destination = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059d6b8});
    inline int16_t& _done_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059d6b4});
    inline int16_t& _msl_impact_warning_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005a4a98});
    inline int16_t& _draw_legal_boxes_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059bfb4});
    inline int16_t& _msl_warning_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059c028});
    inline int16_t& _wait_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059d6be});
    inline int16_t& _scan_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059bf30});
    inline int16_t (&_missile_field)[300] =
        *reinterpret_cast<int16_t(*)[300]>(std::uintptr_t{0x00597AF0});
    inline int16_t (&_combat_grid)[81][68] =
        *reinterpret_cast<int16_t (*)[81][68]>(std::uintptr_t{0x00598770});
    inline int16_t (&_weapon_mod_field_status)[15] =
        *reinterpret_cast<int16_t(*)[15]>(std::uintptr_t{0x00596CE0});
    inline int16_t* _weapon_button = reinterpret_cast<int16_t*>(std::uintptr_t{0x00596E80});
    inline int16_t* _ship_turn_order = reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BB20});
    inline int16_t* _repl_fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00596D30});
    inline int16_t* _side_end_of_turn = reinterpret_cast<int16_t*>(std::uintptr_t{0x00596DA0});
    inline int16_t& DAT_0059730e = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059730E});
    inline int16_t (&_design_choice_items)[40] =
        *reinterpret_cast<int16_t (*)[40]>(std::uintptr_t{0x00597310});
    inline int16_t* _weapon_mod_fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x00596FE0});
    inline int16_t* _weapon_arc_field = reinterpret_cast<int16_t*>(std::uintptr_t{0x00597900});
    inline int16_t* _weapon_rack_field = reinterpret_cast<int16_t*>(std::uintptr_t{0x005A49C0});
    inline int16_t& DAT_005a49c2 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A49C2});
    inline int16_t& _multi_colored_box_color_start = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059776A});
    inline int16_t& _beam_filter_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A2BE8});
    inline int16_t& _missile_filter_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597428});
    inline int16_t& _special_filter_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059790A});
    inline int16_t& _replacement_box_accept_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005978F4});
    inline int16_t& _bomb_filter_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597662});
    inline int16_t& ENDOF__save_game_description = _bomb_filter_button;
    inline int16_t& DAT_00596fe2 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596FE2});
    inline int16_t& DAT_00596fe4 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596FE4});
    inline int16_t& DAT_00597902 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597902});
    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});

    inline uint16_t& _fleet_box_map_height = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00597D70});
    inline s_tech_drive_data* _drives = reinterpret_cast<s_tech_drive_data*>(std::uintptr_t{ 0x005555e0 });
    inline s_current_design*& _design =
        *reinterpret_cast<s_current_design**>(std::uintptr_t{0x0059B330});

    inline int16_t& _scanned_big_ship = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B2A});
    inline int16_t& _small_ship_stack_ptr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977B4});
    inline int16_t& _scanned_small_ship = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A49B2});
    inline int16_t& _n_fltscrn_officers = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A183A});
    inline int16_t*& _officer_id_list = *reinterpret_cast<int16_t**>(std::uintptr_t{0x00596E94});
    // Suggested name: MOX::_star_colors_seg
    inline s_animation_header* (&_star_colors_seg)[11] =
        *reinterpret_cast<s_animation_header* (*)[11]>(std::uintptr_t{0x00597430});
    inline s_animation_header* (&_fleet_galaxy_star_seg)[11] =
        *reinterpret_cast<s_animation_header*(*)[11]>(std::uintptr_t{0x00596F50});
    inline s_animation_header* (&_colony_galaxy_star_seg)[10] =
        *reinterpret_cast<s_animation_header* (*)[10]>(std::uintptr_t{0x00597AA0});
    inline s_animation_header* (&_officer_skill_icon_seg)[27] =
        *reinterpret_cast<s_animation_header*(*)[27]>(std::uintptr_t{0x005A1BD0});
    inline s_animation_header* (&_hl_antaran_ship_icon_seg)[24] =
        *reinterpret_cast<s_animation_header* (*)[24]>(std::uintptr_t{0x00597380});
    inline s_animation_header* (&_antaran_ship_icons)[24] =
        *reinterpret_cast<s_animation_header* (*)[24]>(std::uintptr_t{0x00597670});
    inline s_animation_header* (&_o_ring_seg)[5] =
        *reinterpret_cast<s_animation_header* (*)[5]>(std::uintptr_t{0x00596D80});
    inline s_animation_header* (&_planet_mask_seg)[5] =
        *reinterpret_cast<s_animation_header* (*)[5]>(std::uintptr_t{0x00596F90});
    inline s_animation_header* (& _fltscrn_small_ship_icon_seg)[8] =
        *reinterpret_cast<s_animation_header* (*)[8]>(std::uintptr_t{0x005A1760});
    inline s_animation_header* (&_monster_ship_seg)[4] =
        *reinterpret_cast<s_animation_header* (*)[4]>(std::uintptr_t{0x0059BF10});
    inline s_animation_header* (&_hl_monster_ship_icon_seg)[4] =
        *reinterpret_cast<s_animation_header* (*)[4]>(std::uintptr_t{0x005A17A0});
    inline s_animation_header* (&_ship_icon_seg)[32] =
        *reinterpret_cast<s_animation_header* (*)[32]>(std::uintptr_t{0x005A1870});
    inline s_animation_header* (&_hl_ship_icon_seg)[32] =
        *reinterpret_cast<s_animation_header* (*)[32]>(std::uintptr_t{0x005A4A00});
    inline s_animation_header*& _fleet_background_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x005a47d8 });
    inline void*& _officer_pictures_screen_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597778});
    inline s_animation_header*& _officer_main_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597830});
    inline s_animation_header*& _officer_colony_box_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B44C});
    inline s_animation_header*& _officer_colony_ldrs_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596FAC});
    inline s_animation_header*& _officer_ship_ldrs_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596C98});
    inline void*& _officer_up_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A1B54});
    inline void*& _officer_dn_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00596E9C});
    inline void*& _officer_prev_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00596440});
    inline void*& _officer_next_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597008});
    inline void*& _officer_hire_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059D6C4});
    inline void*& _officer_return_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059B440});
    inline void*& _officer_cancel_hire_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059B5FC});
    inline void*& _selected_box_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005977E8});
    inline s_animation_header*& _scanned_box_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005977E4});
    inline s_animation_header*& _fleet_scroll_anim_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1B40});
    inline void*& _officer_normal_window_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059BD2C});
    inline void*& _officer_bright_window_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597020});
    inline s_animation_header**& _temp_officers_pict = *reinterpret_cast<s_animation_header***>(std::uintptr_t{0x0059B338});
    inline void*& _officer_dismiss_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059B5E4});
    inline s_animation_header*& _officer_fleet_box_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005979A8});
    inline void*& _dull_officer_dismiss_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005979AC});
    inline void*& _dull_officer_pool_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059b468});
    inline void*& _officer_pool_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597acc});
    inline void*& _dull_officer_hire_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A1BC4});
    inline void*& _officer_hire_replace_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A1B48});
    inline int8_t& _officer_scanned = *reinterpret_cast<int8_t*>(std::uintptr_t{0x00596E70});
    inline int8_t& _officer_scrn_type = *reinterpret_cast<int8_t*>(std::uintptr_t{ 0x00597414 });
    inline void*& _officer_colony_ldrs_button_seg_fleet = *reinterpret_cast<void**>(std::uintptr_t{0x00596C98});
    inline void*& _officer_colony_ldrs_button_seg_system = *reinterpret_cast<void**>(std::uintptr_t{0x00596FAC});
    inline int8_t& _officer_selected = *reinterpret_cast<int8_t*>(std::uintptr_t{ 0x0059D6D0 });

    inline uint16_t& _special_scroll_dn_button = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596434});
    inline uint8_t& _n_weapons_loaded = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596E7A});
    inline uint8_t& _n_special_systems = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059BD2A});

    inline int8_t*& _fltscrn_icon_selection_status =
        *reinterpret_cast<int8_t**>(std::uintptr_t{0x00596438});

    // References
    inline int8_t (&_food_per_farmer_table)[10] =
        *reinterpret_cast<int8_t(*)[10]>(std::uintptr_t{0x00559690});
    inline int8_t& food_bonus_base_4_6 = *reinterpret_cast<int8_t*>(std::uintptr_t{0x00559698});
    inline int8_t& food_bonus_base_5_8 = *reinterpret_cast<int8_t*>(std::uintptr_t{0x00559699});
    inline uint8_t* _star_class_table = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005596C0});
    inline int16_t (&_ranged_to_hit_penalty)[9] =
        *reinterpret_cast<int16_t(*)[9]>(std::uintptr_t{0x005596E0});
    inline int16_t (&_ranged_damage_penalty)[9] = 
        *reinterpret_cast<int16_t(*)[9]>(std::uintptr_t{0x005596F8});
    inline int16_t& _cur_missile_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597408});
    inline int16_t& num_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059740A });
    inline int16_t& _NUM_STARS = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059740A });
    inline s_scroll_bar& _fltpop_scroll_bar = *reinterpret_cast<s_scroll_bar*>(std::uintptr_t{0x005974C0});
    inline int16_t& _cur_map_scale = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059D604 });
    inline int16_t& _cur_map_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A18F0});
    inline int16_t& _cur_map_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A18F8});
    inline uint16_t& _global_esc = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00597774});
    inline short& _return_screen = *reinterpret_cast<short*>(std::uintptr_t{0x005977F0});
    inline char*& _save_game_dates = *reinterpret_cast<char**>(std::uintptr_t{0x005A1CA8});
    inline char*& _save_game_stardates = *reinterpret_cast<char**>(std::uintptr_t{0x005974e8});
    inline char*& _star_name_loaded_string =
        *reinterpret_cast<char**>(std::uintptr_t{0x005A2C20});
    inline int16_t& _scanned_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005a4a9a});
    inline short& _previous_screen = *reinterpret_cast<short*>(std::uintptr_t{ 0x00597AD0 });
    inline int16_t& _max_debug_saves = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059C458 });
    inline int16_t& _first_debug_save = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A1830 });
    inline int16_t& _debug_saves_on = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059D6C2 });
    inline uint8_t& _turns_between_debug_saves = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A49F0 });
    inline void*& _sound_buffer = *reinterpret_cast<void**>(std::uintptr_t{ 0x005A1B34 });
    inline int16_t& _hot_seat_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A182C});
    inline short& _current_screen = *reinterpret_cast<short*>(std::uintptr_t{ 0x00597302 });
    inline int16_t& _design_screen_replacement_type = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597402});
    inline int16_t& _design_screen_replacement_slot = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059799C});
    inline int16_t (&_weapons_field)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x0059BF20});
    inline s_animation_header*& _asteroid_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x005973F4 });
    inline s_animation_header*& _screen_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x005973E4 });
    inline s_animation_header*& _combat_bitmap = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597A94});
    inline s_animation_header*& _combat_map_planet_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596448});
    inline s_animation_header*& _combat_planet_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A17B4});
    inline s_animation_header*& _combat_planet_seg2 =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B5E8});
    inline uint8_t (&_ship_darken_map)[128][16] =
        *reinterpret_cast<uint8_t(*)[128][16]>(std::uintptr_t{0x00596450});
    inline uint8_t (&_given_chance_to_rename_home_star)[8] =
        *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005973E8});
    inline uint8_t& _player_has_ship_in_system = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596D4B});
    inline s_animation_header* (&_moveable_box_pict)[25] = *reinterpret_cast<s_animation_header* (*)[25]>(std::uintptr_t{0x005A1B60});
    inline uint8_t& _game_type = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005974DD });
    inline s_antaran& _antarans = *reinterpret_cast<s_antaran*>(std::uintptr_t{0x0059BA20});
    inline uint8_t& _maps_to_generate = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059791C });
    inline uint8_t& _auto_turn_exit = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059B320});
    inline uint16_t& _special_scroll_up_button = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059775E});
    inline int16_t& _auto_msl_warning_off = *reinterpret_cast<int16_t*>(0x0059790E);
    inline s_animation_header*& _draw_ship_bitmap = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B328});
    inline int16_t& _multi_colored_line_start = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B4B8});
    inline int16_t* _design_choice_item_selected = reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BD40});
    inline uint8_t& _first_special_system_ctr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596D4A});
    inline int16_t (&_special_system_field)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x0059BAD0});
    inline int16_t(&_planet_buttons)[5] = *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{ 0x00597470 });
    inline uint8_t& _first_design_screen_call = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059B5C9});
    inline int16_t& _design_size = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B434});
    inline uint8_t& _refresh_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059B5A4 });
    inline uint8_t& _mox_font_colors_offset = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059BA6E });
    inline int16_t& _star_width = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596C5C});
    inline int16_t& _star_height = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BA04});
    inline char (&_temp_string)[512] = *reinterpret_cast<char(*)[512]>(std::uintptr_t{0x00596230});
    inline char (&_temp_string_buf)[64] = *reinterpret_cast<char (*)[64]>(std::uintptr_t{0x00559487});
    inline int16_t& _take_control_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D62});
    inline int16_t& _auto_cmbt_button_value = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596C60});
    inline int16_t& _PLAYER_NUM = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A18F2 });
    inline char (&_active_player)[9] =
        *reinterpret_cast<char (*)[9]>(std::uintptr_t{0x00597360});
    inline int16_t& _active_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B5B6});
    inline int16_t& _ai_retreat_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B436});
    inline int16_t& _bio_weapon_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B446});
    inline int16_t& _last_ship_status = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B3A});
    inline int16_t& _ship_status_display = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B444});
    inline int16_t& _dont_draw_ship = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BF44});
    inline int16_t& _draw_active_missiles = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B4A4});
    inline int16_t& _order_info_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597834});
    inline int16_t& _super_fast_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B60E});
    inline int8_t& _real_attack_flag = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A8BBC});
    inline int16_t& _shields_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B9FA});
    inline int16_t& _msl_impact_warning_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BF68});
    inline int16_t& _draw_legal_boxes_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D72});
    inline int16_t& _take_control_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D48});
    inline int16_t& _combat_far_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B3FE});
    inline int16_t& _combat_mid_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059785C});
    inline int16_t& _combat_far_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B3FA});
    inline int16_t& _combat_mid_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597850});
    inline int16_t& _net_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B314});
    inline int16_t& _in_nebula_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BA18});
    inline int16_t& _skan_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1800});
    inline int16_t& _fleet_target = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7794});
    inline uint8_t& _draw_design_screen_to_back = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A1B44});
    inline uint8_t& _refit_ship = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597A80});
    inline int& _original_cost = *reinterpret_cast<int*>(std::uintptr_t{0x0059B324});
    inline int16_t& _design_name_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597490});
    inline int16_t& _design_cancel_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597010});
    inline int16_t& _design_clear_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B4C});
    inline int16_t& _design_build_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B60C});
    inline s_player*& _player = *reinterpret_cast<s_player**>(std::uintptr_t{ 0x0059B3D0 });
    inline s_player*& _cur_plyr_ptr = *reinterpret_cast<s_player**>(std::uintptr_t{0x005974E0});
    inline char* (&_personality_labels)[7] = *reinterpret_cast<char* (*)[7]>(std::uintptr_t{0x00596CC0});
    inline char* (&_objective_labels)[6] = *reinterpret_cast<char* (*)[6]>(std::uintptr_t{0x0059B3E0});
    inline char* (&_treaty_labels)[7] = *reinterpret_cast<char* (*)[7]>(std::uintptr_t{0x005A1B00});
    inline char* (&_help_category_labels)[17] = *reinterpret_cast<char* (*)[17]>(std::uintptr_t{0x0059BFE0});
    inline int16_t& _old_system_index = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B5F8});
    inline int8_t& _selected_star_color_mod =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x00596C50});
    inline int8_t& _selected_star_color_offset = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0059C454});
    inline int16_t* _antarans_ship_counts = reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059BA38 });
    inline int16_t* _antarans_ship_counts_end = reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059BA4A });
    inline uint32_t& _stardate = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA64 });
    inline s_settings& _settings = *reinterpret_cast<s_settings*>(std::uintptr_t{ 0x00597080 });
    inline char*& _hyper_field_title = *reinterpret_cast<char**>(std::uintptr_t{0x00598764});
    inline const char* s_Harbinger = reinterpret_cast<const char*>(std::uintptr_t{0x005576B8}); // "Harbinger"
    inline uint8_t& _at_least_one_tactical_combat = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005973E0});
    // Suggested name: MOX::_planet_summary_galaxy_star_seg
    inline s_animation_header** _planet_summary_galaxy_star_seg =
        reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B470});
    inline uint8_t& _first_weapon_ctr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059D6C8});
    inline int16_t& _fleet_box_colonize_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059D6BA});
    inline uint8_t& _reloaded_ship_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059D6BC});
    inline uint8_t& _settings_language = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597152 });
    inline uint8_t& _settings_galaxy_age = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597157 });
    inline uint8_t& _g_galaxy_age = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A1780});
    inline int16_t& _ship_data_id = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1782});
    inline int16_t& _cur_combat_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597070});
    inline int16_t& _cur_ptr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B42A});
    inline int16_t& _wait_ptr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059b5bc});
    inline int16_t& _board_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B340});
    inline int16_t& _tactical_net_choice = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005973F0});
    inline int16_t& _auto_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B3FC});
    inline int16_t& _auto_combat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B5CA});
    inline int16_t& _speedx2_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596CB4});
    inline int16_t& _grid_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BAAA});
    inline int16_t& _shields_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005979A4});
    inline int16_t& _super_fast_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597420});
    inline int16_t& _self_destruct_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BD38});
    inline int16_t& _weapons_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059742A});
    inline int16_t& _specials_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596D48});
    inline int16_t& _cmbt_info_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BAB0});
    inline int16_t& _flee_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597836});
    inline int16_t& _board_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597AD2});
    inline int16_t& _map_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596C52});
    inline int16_t& _map_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B43C});
    inline int16_t& _map_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B43A});
    inline int16_t& _center_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597852});
    inline int16_t& _space_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596FD0});
    inline int16_t& _ypos = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005976D4});
    inline int16_t& _xpos = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059745C});
    inline int16_t& _saved_msl_warn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059791A});
    inline uint8_t& _red_color_pulse_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597848});
    inline int8_t& _red_color_pulse_incr = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0059784C});
    inline uint16_t& _fleet_ship_scanned = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059736A});
    inline int16_t& _fleet_movement_all_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597466});
    inline int16_t& _galaxy_map_scanned_star = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597762});
    inline int8_t& _draw_system_display_without_button = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A49CA});
    inline uint8_t& _need_to_restore_help_list = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597D60});
    inline uint16_t& _ship_icon_player_id = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059B322});
    inline int8_t& _at_least_one_report = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005978EC});
    inline uint8_t& _previous_report = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596CB2});
    inline int16_t& _first_node_ptr_in_stack_ = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597AC8});
    inline uint8_t* _rot_plan_dim = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597758});
    inline uint16_t& _system_star_id = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A1CB8});
    inline int16_t& _system_star_id_for_auto_select = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BA1A});
    inline uint8_t& _draw_fleet_screen_to_back = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059770C});
    inline s_mouse_xy& _mouse_XY = *reinterpret_cast<s_mouse_xy*>(std::uintptr_t{0x0059B5C0});
    inline int16_t& _system_display_ship_scanned = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BFB0});
    inline int16_t& _system_icon_scanned = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597A98});
    inline uint16_t& _global_exit_field = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00597D7A});
    inline uint16_t& _player_design_num = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A1B28});
    inline int16_t& _eta_star_id = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BAAE});
    inline int16_t& _stencil_box_id = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B2C});
    inline int16_t& _draw_main_screen_stencilled = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1864});
    inline uint16_t& _animated_star = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059B5F0});
    inline s_mouse_list (&_mouse_list_zoom_mode)[5] = *reinterpret_cast<s_mouse_list(*)[5]>(std::uintptr_t{0x00559898});
    inline int8_t& _just_moved_stack = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0059B5BE});
    inline int16_t& _box_off_flag_ = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977CC});
    inline int16_t& _first_fleet_movement_icon = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D4A});

    inline int16_t& _next_free_node =\
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005a4aa4});

    inline uint8_t* _main_palette_player_colors = reinterpret_cast<uint8_t*>(std::uintptr_t{0x0054F098});

    inline uint16_t* _h_string_ix = reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059B6E0});
    inline char* _h_strings = reinterpret_cast<char*>(std::uintptr_t{0x0059D6E0});

    inline s_help_box (&_help_entry_list)[50] =
        *reinterpret_cast<s_help_box(*)[50]>(std::uintptr_t{0x005A1900});

    // Suggested name: MOX::_class_to_mineral
    inline uint8_t (&_class_to_mineral)[60] = *reinterpret_cast<uint8_t(*)[60]>(std::uintptr_t{0x00559540});

    // Suggested name: MOX::_planet_special
    inline uint8_t& _planet_special =
            *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005596A0});

    // Suggested name: MOX::_planet_special_weighted_chance
    inline uint8_t* _planet_special_weighted_chance = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005596B0});

    // Suggested name: MOX::_gravity_table
    inline uint8_t (&_gravity_table)[30] = *reinterpret_cast<uint8_t(*)[30]>(std::uintptr_t{0x00559580});

    inline s_mouse_list &_mouse_list_arrow = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00558D10});
    inline s_mouse_list& _mouse_list_relocation = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005598D8});
    inline s_mouse_list (&_cmbt_mouse_list_clock) = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00558DB8});
    inline s_mouse_list &_mouse_list_variable = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00558D20});
    inline s_mouse_list& _mouse_list_none = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00558D70});

    inline int16_t& _mid_star_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059643C});
    inline int16_t& _mid_star_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596444});
    inline int16_t& _design_screen_replacement_item = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059C41C});
    inline int16_t& _far_star_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596CAE});
    inline int16_t& _far_star_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596CB6});
    inline int16_t& _near_star_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596EA6});
    inline int16_t& _near_star_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596EA8});
    inline s_animation_header*& _near_star_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597424});

    inline const char* s_ATZ_0055AA7C =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA7C}); // "ATZ"

    inline const char* s_AT_K0_M1_D2_0055AA6C =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA6C}); // "AT &K0 M1 &D2"

    inline const char* s_AT_H0_S0_0_0055AA60 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA60}); // "AT H0 S0=0"

    inline const char* s_5551212_0055AA58 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA58}); // "5551212"

    inline const char* s_fmt_s =
        reinterpret_cast<const char*>(std::uintptr_t{0x00551130}); // "%s"

    inline const char* s_fmt_scanned_star_info =
        reinterpret_cast<const char*>(std::uintptr_t{0x005569C0}); // ">GEGGGGG%s (%s)"

    inline const char* s_fmt_contested =
        reinterpret_cast<const char*>(std::uintptr_t{0x00559930}); // "%s (contested)"

    inline void*& _special_names_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x00595390 });

    inline void*& _global_data_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x00597410 });

    inline s_nebula*& _nebula =
        *reinterpret_cast<s_nebula**>(std::uintptr_t{ 0x00597770 });

    inline uint8_t& _NUM_NEBULAS = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596D10});

    inline uint16_t& _NUM_COLONIES =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00597856 });

    inline uint16_t& _planet_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0059700E });

    // Suggested name: MOX::_climate_modifier_table
    inline int8_t (&_climate_modifier_table)[5] =
        *reinterpret_cast<int8_t(*)[5]>(std::uintptr_t{0x00559688});

    inline s_planet_data*& _planet =
        *reinterpret_cast<s_planet_data**>(std::uintptr_t{ 0x005978F8 });

    inline char** _roman_literals = reinterpret_cast<char**>(std::uintptr_t{0x00559424});

    inline char* (&_planet_special_string)[12] = *reinterpret_cast<char* (*)[12]>(std::uintptr_t{0x00597780});
    inline char* (&_planet_climate_string)[10] = *reinterpret_cast<char* (*)[10]>(std::uintptr_t{0x0059BA80});
    inline char* (&_mineral_class_string)[5] = *reinterpret_cast<char* (*)[5]>(std::uintptr_t{0x005A2C30});
    inline char* (&_planet_size_string)[5] = *reinterpret_cast<char* (*)[5]>(std::uintptr_t{0x005A47C0});
    inline char* (&_planet_type_string)[10] = *reinterpret_cast<char* (*)[10]>(std::uintptr_t{0x00596C70});
    inline char* (&_planet_group_string)[4] = *reinterpret_cast<char* (*)[4]>(std::uintptr_t{0x00596D00});
    inline char* (&_planet_atmosphere_string)[3] = *reinterpret_cast<char* (*)[3]>(std::uintptr_t{0x0059B600});
    inline char* (&_spectral_class_string)[7] = *reinterpret_cast<char* (*)[7]>(std::uintptr_t{0x0059BED0});
    inline char* (&_admin_skill_names)[9] = *reinterpret_cast<char* (*)[9]>(std::uintptr_t{0x0059BF70});

    inline char* (&_planet_gravity_string)[3] =
        *reinterpret_cast<char*(*)[3]>(std::uintptr_t{0x005A49E0});

    inline uint16_t& _NUM_SHIPS = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0059B4BC });
    inline void*& _global_bitmap_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00596DC4});
    inline uint8_t& _first_time_drawn = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596D1A});
    inline uint8_t& _draw_main_screen_to_back = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596E98});
    inline uint8_t& _dont_draw_main_screen_to_back = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059BFB2});
    inline int16_t& _zoom_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BFDC});
    inline int16_t& _max_zoom_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B448});
    inline uint8_t& _star_bg_index = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A49F8});
    inline s_animation_header*& _star_bg_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1790});
    inline s_animation_header*& _detail_fleet_icon_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A49D4});
    inline uint16_t& _fleet_icon_button_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596D18});
    inline int16_t (&_weapons_count_plus_button)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00597D50});
    inline int16_t (&_weapons_count_minus_button)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x005A1810});
    inline int16_t (&_design_ship_size_field)[6] = *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x0059BAC0});

    inline void*& _techname_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0059B31C });

    inline void*& _tech_desc_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005977EC});

    inline s_animation_header*& _main_screen_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B400});
    inline s_animation_header*& _design_screen_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059D6CC});
    inline s_animation_header*& _design_icon_lf_arrow_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B33C});
    inline s_animation_header*& _design_icon_rt_arrow_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BAB4});
    inline s_animation_header*& _clear_design_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597004});
    inline s_animation_header*& _cancel_design_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005978F0});
    inline s_animation_header*& _build_design_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596EAC});
    inline s_animation_header*& _plus_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005973F8});
    inline s_animation_header*& _minus_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B4AC});
    inline s_animation_header*& _d_scroll_up_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597714});
    inline s_animation_header*& _d_scroll_dn_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1868});
    inline s_animation_header*& _dull_d_scroll_up_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596F7C});
    inline s_animation_header*& _dull_d_scroll_dn_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597404});
    inline s_animation_header*& _beam_filter_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BF3C});
    inline s_animation_header*& _missile_filter_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005972FC});
    inline s_animation_header*& _bomb_filter_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B334});
    inline s_animation_header*& _special_filter_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1834});
    inline s_animation_header*& _weapons_cancel_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005974A0});
    inline s_animation_header*& _weapons_accept_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059783C});
    inline s_animation_header*& _weapons_top_box_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B4B0});
    inline s_animation_header*& _weapons_mid_box_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596FD4});
    inline s_animation_header*& _weapons_bot_box_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BCC4});
    inline s_animation_header*& _weapons_selected_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059C024});
    inline s_animation_header*& _weapons_blank_mods_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597000});
    inline s_animation_header*& _system_repl_scroll_bar_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597418});
    inline s_animation_header*& _system_repl_box_top_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1824});
    inline s_animation_header*& _system_repl_box_mid_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1B3C});
    inline s_animation_header*& _system_repl_box_btm_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596FA4});
    inline s_animation_header*& _system_repl_box_cancel_btn = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BD30});
    inline int16_t (&_weapons_clear_button)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x005974B0});
    inline int16_t (&_special_system_clear_button)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00597480});
    inline s_animation_header* (&_design_size_button_seg)[6] = *reinterpret_cast<s_animation_header*(*)[6]>(std::uintptr_t{0x00597740});
    inline s_animation_header** _darkened_size_button_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B410});
    inline s_animation_header** _weapons_arc_pict_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596D60});
    inline s_animation_header** weapons_arc_pict_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596D60});
    inline s_animation_header** _weapons_arc_text_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597030});
    inline s_animation_header** weapons_arc_text_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597030});
    inline s_animation_header** _weapons_rack_text_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597980});
    inline s_animation_header** _weapons_special_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597930});
    inline s_weapon_filter_item*& _weapon_filter = *reinterpret_cast<s_weapon_filter_item**>(std::uintptr_t{0x0059BA0C});
    inline char** _special_desc_string = reinterpret_cast<char**>(std::uintptr_t{0x00596EB0});
    inline char** _weapon_notes_string = reinterpret_cast<char**>(std::uintptr_t{0x00596DD0});
    inline s_animation_header*& _d_scroll_bar_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597994});
    inline s_animation_header*& _weapons_rack_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005979A0});

    inline s_animation_header*& _mainscrn_diplo_lights_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B464});

    inline void*& _global_screen_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0059B5A8 });

    inline void*& _global_cache_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0059B5B8 });

    inline s_animation_header*& _mid_star_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B610});
    inline s_animation_header*& _far_star_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A17B0});
    inline uint32_t& _black_hole_anim_bitfield = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A17C0});
    inline uint8_t (&_fleet_icon_selection_status)[64] =
        *reinterpret_cast<uint8_t(*)[64]>(std::uintptr_t{0x005A17C0});

    // Suggested name: MOX::_star_seg
    inline s_animation_header** _star_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B4C0});
    inline s_animation_header** _sun_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BF50});

    inline uint16_t& _n_stats_to_generate = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A182E});

    inline uint32_t& _global_cache_size =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA10 });

    inline uint32_t& _cmd_line_seed =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA08 });

    inline int16_t& _ship_icon_lf_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BA14});
    inline int16_t& _ship_icon_rt_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597374});

    inline uint8_t& _zoom_in = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597704});
    inline s_animation_header*& _main_zoom_in_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B318});
    inline s_animation_header*& _main_zoom_out_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597910});
    inline s_animation_header*& _main_game_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596DC0});
    inline s_animation_header*& _main_next_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B5EC});
    inline s_animation_header*& _main_colonies_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B4B4});
    inline s_animation_header*& _main_planets_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597844});
    inline s_animation_header*& _main_fleets_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597378});
    inline s_animation_header*& _main_leaders_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A2BF0});
    inline s_animation_header*& _main_races_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B42C});
    inline s_animation_header*& _main_info_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596E74});
    inline int16_t* _star_button = reinterpret_cast<int16_t*>(std::uintptr_t{0x005A2C50});
    inline int16_t& _main_screen_grid_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977F2});
    inline int16_t& _main_screen_grid_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596D52});
    inline int16_t& _main_screen_grid_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596E78});

    inline int16_t& _NUM_PLAYERS = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A2C1C });
    inline char* (&_government_labels)[8] = *reinterpret_cast<char* (*)[8]>(std::uintptr_t{0x00596FB0});
    inline char** _special_ability_names = reinterpret_cast<char**>(std::uintptr_t{0x0059B350});
    inline uint32_t& _stardate_at_beg_of_turn = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0059B344});
    inline int16_t& _global_var = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B4AA});
    inline uint16_t& _FULL_DRAW_SCREEN_FLAG = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A2C24});
    inline uint16_t& _combat_init = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005596D8});
    inline void*& _warp_core_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x00596430});
    inline s_mouse_list*& _combat_mouse_grid = *reinterpret_cast<s_mouse_list**>(std::uintptr_t{0x00596CB8});
    inline void*& _stasis_bubble_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x00596CBC});
    inline s_animation_header*& _combat_background_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596F80});
    inline s_animation_header*& _combat_far_star = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1CB0});
    inline s_animation_header*& _combat_mid_star = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596D14});
    inline s_animation_header*& _combat_near_star = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597D7C});
    inline uint16_t& _apply_damage_to_planet = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596CDC});
    inline uint16_t& _missile_warning = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596D1C});
    inline void*& _cmbt_option_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00596D44});
    inline s_animation_header*& _warning_missile_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596E90});
    inline void*& _self_destruct_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00596EA0});
    inline uint16_t& _saved_msl_warning = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596EA4});
    inline void*& _auto_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00596FA8});
    inline s_animation_header*& _combat_options_background_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059741C});
    inline int16_t& _cur_combat_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059706E});
    inline uint16_t& _grid_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00597300});
    inline uint16_t& _speedx2_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00597400});
    inline int16_t (&_auto_combat_flag)[2] =
        *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x0059740C});
    inline int16_t& _combat_clock_frame = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059745E});
    inline int16_t& _combat_near_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597468});
    inline int16_t& _combat_near_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059747A});
    inline void*& _cmbt_done_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005976D0});
    inline void*& _level_symbol_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005979B0});
    inline void*& _scan_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597840});
    inline void*& _board_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597700});
    inline void*& _flee_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005979B4});
    inline void*& _wait_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A178C});
    inline void*& _cmbt_info_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059C430});
    inline void*& _weapons_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597A84});
    inline void*& _specials_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059BD34});
    inline s_animation_header*& _msl_warning_on_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A49F4});
    inline void*& _warning_fighter_seg = *reinterpret_cast<void**>(std::uintptr_t{0x00597764});
    inline s_animation_header*& _ship_bitmap = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597A8C});
    inline s_animation_header*& _darken_plasma_map = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A49D0});
    inline void*& _shield_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x0059BECC});
    inline void*& _pulsar_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x005A2BE4});
    inline s_animation_header*& _small_tractor_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B5B0});
    inline void*& _tractor_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x005A4AA8});
    inline void*& _bhg_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x005A1860});
    inline void*& _plasma_web_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x005A1CB4});
    inline void*& _stellar_converter_remap_colors = *reinterpret_cast<void**>(std::uintptr_t{0x0059C418});
    inline s_animation_header* (&_msl_seg)[112] =
        *reinterpret_cast<s_animation_header* (*)[112]>(std::uintptr_t{0x005A47F0});
    inline s_animation_header* (&_ship_cursor)[4] = *reinterpret_cast<s_animation_header*(*)[4]>(std::uintptr_t{0x0059BF00});
    inline s_animation_header* (&_shield_div_bitmap)[4] = *reinterpret_cast<s_animation_header*(*)[4]>(std::uintptr_t{0x0059BFA0});
    inline s_animation_header* (&_fltscrn_monster_ship_icon)[7] =
        *reinterpret_cast<s_animation_header*(*)[7]>(std::uintptr_t{0x0059BFC0});
    inline s_animation_header** _shield_mask_bitmap = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BAE0});
    inline int16_t* _fighter_beam_type = reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B450});
    inline int16_t* _fighter_bomb_type = reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D80});
    inline s_combat_data*& _combat_data = *reinterpret_cast<s_combat_data**>(std::uintptr_t{0x005A2BE0});
    inline s_missile*& _missile = *reinterpret_cast<s_missile**>(std::uintptr_t{0x00597858});
    inline int16_t (&_missile_hit_radius)[6] =
        *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x00559718});
    inline s_net_combat_message& _new_message = *reinterpret_cast<s_net_combat_message*>(std::uintptr_t{0x0059B5D0});
    inline int16_t* _gate_icon_field = reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D68});
    inline int16_t& _system_display_base_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B4A6});
    inline int16_t& _system_display_base_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B4A8});
    inline s_sys_disp*& _sys_disp = *reinterpret_cast<s_sys_disp**>(std::uintptr_t{0x0059B5F4});
    inline s_system_display_ships (&_system_display_ships)[15] =
        *reinterpret_cast<s_system_display_ships(*)[15]>(std::uintptr_t{0x0059B620});
    inline int16_t& _system_display_grid_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BA6C});
    inline char* (&_race_names)[20] =
        *reinterpret_cast<char* (*)[20]>(std::uintptr_t{ 0x0059D660 });
    inline int8_t (&_class_to_num_satellites)[10][6] =
        *reinterpret_cast<int8_t (*)[10][6]>(std::uintptr_t{0x005594C8});
    inline uint8_t (&_special_bonus)[9][3] = *reinterpret_cast<uint8_t (*)[9][3]>(std::uintptr_t{ 0x0055903B });
    inline uint16_t (&_star_fields_dim)[6] = *reinterpret_cast<uint16_t(*)[6]>(std::uintptr_t{ 0x005A47B0 });

    inline s_ship_data*& _ship =
        *reinterpret_cast<s_ship_data**>(std::uintptr_t{ 0x0059BD3C });

    inline int16_t& _combat_defender_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059700C});
    inline int16_t& _combat_attacker_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059701C});
    inline s_qship*& _qcombat_data = *reinterpret_cast<s_qship**>(std::uintptr_t{0x00597498});
    inline int16_t& _combat_total_ship_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059784A});
    inline uint8_t& _combat_done_waiting = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A1B50});
    inline s_colors& _combat_green = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559710});
    inline int16_t& _combat_attacker_player = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B32C});
    inline int16_t& _combat_turn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B49C});
    inline uint8_t& _colony_defense_flags = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059B4BA});
    inline int16_t& _cmbt_colony_id = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A47BC});
    inline int16_t& _combat_defender_player = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A49CC});
    inline s_combat_bomb_result& _combat_bomb_result = *reinterpret_cast<s_combat_bomb_result*>(std::uintptr_t{0x005972B2});

    inline s_global_combat_data& _global_combat_data = *reinterpret_cast<s_global_combat_data*>(std::uintptr_t{0x0059BD90});

    inline int16_t*& _officers_original_star_id =
        *reinterpret_cast<int16_t**>(std::uintptr_t{0x0059BF38});

    inline int16_t*& _officers_original_ship_id =
        *reinterpret_cast<int16_t**>(std::uintptr_t{0x005A1CBC});

    inline void*& _dimmed_all_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A1788});
    inline void*& _dimmed_relocate_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059C434});
    inline void*& _dimmed_scrap_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A47DC});
    inline void*& _dimmed_officers_button_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A4AA0});

    inline s_leader_data*& _leaders =
        *reinterpret_cast<s_leader_data**>(std::uintptr_t{ 0x0059BF40 });

    inline char* (&_officer_level_names)[6] =
        *reinterpret_cast<char*(*)[6]>(std::uintptr_t{0x00597720});

    inline char* (&_star_officer_level_names)[6] =
        *reinterpret_cast<char*(*)[6]>(std::uintptr_t{0x005A4A80});

    inline s_skill_data (&_skill_data)[54] = *reinterpret_cast<s_skill_data(*)[54]>(std::uintptr_t{0x00559058});

    inline s_colony*& _colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{ 0x005A1828 });

    inline s_animation_header*& _darkened_turn_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1B30});

    inline s_animation_header*& _fleet_box_selection_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1784});
    inline int16_t& _ship_frame = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B1C});
    inline s_animation_header*& _mainscrn_plyr_done_lights_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A4A9C});
    inline uint16_t& _skip_fields = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A1B5A});

    inline int8_t (&_ship_icon_width)[4] = *reinterpret_cast<int8_t(*)[4]>(std::uintptr_t{0x005a49ec});
    inline int8_t (&_ship_icon_height)[4] = *reinterpret_cast<int8_t(*)[4]>(std::uintptr_t{0x0059c460});
    inline s_ship_move_info& _g_ship_move_info = *reinterpret_cast<s_ship_move_info*>(std::uintptr_t{0x0059c440});

    inline s_star_data*& _star =
        *reinterpret_cast<s_star_data**>(std::uintptr_t{ 0x005A18F4 });

    // Suggested name: MOX::orbit_to_satellite_type
    inline uint8_t* _orbit_to_satellite_type = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559508});

    // Suggested name: MOX::_ship_node
    inline s_ship_node (&_ship_node)[500] =
        *reinterpret_cast<s_ship_node(*)[500]>(std::uintptr_t{0x00597DA0});

    inline uint16_t& _ship_stack_count =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059B3F8});

    inline int16_t (&_ship_stack_start)[500] =
        *reinterpret_cast<int16_t(*)[500]>(std::uintptr_t{0x0059C030});

    inline uint32_t& _fleet_box_ship_ids =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0059D610});

    inline void*& _other_ships_screen_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0059D6B0});

    inline void*& _fleet_box_grid_sm_seg4 = *reinterpret_cast<void**>(std::uintptr_t{0x005A47E0});

    inline int16_t& _icon_overlay_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005974EC});

    inline int16_t& _ship_icon_count =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597664});

    inline int16_t& _icon_overlay_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597666});

    inline int16_t& _fleet_box_ship_stack =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A183E});

    inline s_ship_icon (&_ship_icon)[572] =
        *reinterpret_cast<s_ship_icon(*)[572]>(std::uintptr_t{0x005A2CE0});

    inline uint8_t* _black_hole_anim_count = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596CA0});

    inline int16_t (&_bill_savegame)[6] =
        *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x00559478});
    inline int16_t& _bill_savegame_2 =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055947C});

    inline int16_t& _bill_savegame_3 =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055947E});

    inline int16_t& _bill_savegame_4 =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559480});

    inline int16_t& _bill_savegame_5 =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559482});
    inline uint16_t& _antaran_resource_level = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059790C});
    inline s_crew_data* _crew_data = reinterpret_cast<s_crew_data*>(std::uintptr_t{0x00558FA8});
    inline uint16_t& _chance_for_antaran_invasion = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596D24});
    inline uint16_t& _disallow_saves_code = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00597854});

    inline int16_t& _max_map_scale = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00597768 });
    inline int16_t& _MAP_MAX_X = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00597998 });
    inline int16_t& _MAP_MAX_Y = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059799A });
    inline uint8_t& _g_galaxy_size = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597A88 });

    inline void*& _nebula_screen_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x005A2BEC });

    // Suggested name: MOX::nebula_pict_segments
    // This represents a 2D array of sprite pointers, dimensions [12][4].
    inline s_animation_header* (*_nebula_pict_seg)[4] =
        reinterpret_cast<s_animation_header* (*)[4]>(std::uintptr_t{ 0x005979C0 });

    // Suggested name: _planet_max_farms
    inline uint8_t* _planet_max_farms = reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00559660 });

    inline int8_t* _planet_max_population = reinterpret_cast<int8_t*>(std::uintptr_t{0x00559670});

    inline uint8_t* _planet_size_table = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559658});

    #pragma pack(push, 1)
    struct s_class_group {
        uint8_t group[5];
    };
    #pragma pack(pop)

    // 5 groups per spectral class
    inline s_class_group (&_class_to_group)[6] =
        *reinterpret_cast<s_class_group(*)[6]>(std::uintptr_t{0x005595C0});

    // 4 groups per climate
    inline uint8_t (*_climate_roll_table)[4] = reinterpret_cast<uint8_t(*)[4]>(std::uintptr_t{0x005595E0});

    // Suggested name: MOX::normal_galaxy_climate_roll_table
    inline uint8_t (*_normal_gal_climate_roll_table)[4] = reinterpret_cast<uint8_t(*)[4]>(std::uintptr_t{0x00559608});

    // Suggested name: MOX::old_galaxy_climate_roll_table
    inline uint8_t (*_old_gal_climate_roll_table)[4] = reinterpret_cast<uint8_t(*)[4]>(std::uintptr_t{0x00559630});

    // Array of movable box structures (size 0x1C each, total 0x8C/140 bytes)
    inline s_movable_box (&_moveable_box)[5] =
        *reinterpret_cast<s_movable_box(*)[5]>(std::uintptr_t{0x00597860});

    inline uint8_t& _fleet_movement_icons_on =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596C9C});

    inline int16_t& _fleet_icon_buttons_added = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B38});
    inline int16_t& _fleet_box_outpost_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596DBE});
    inline int16_t& _fleet_box_attack_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597D78});
    inline int16_t& _fleet_box_scroll_dn_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BA16});
    inline int16_t& _fleet_box_transport_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BFB6});
    inline int16_t& _fleet_box_engage_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B5C});
    inline int16_t& _fleet_box_scroll_up_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A49B0});

    inline s_animation_header*& _fleet_box_name_lg_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059736C});

    inline s_animation_header*& _fleet_box_name_sm_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B430});

    inline s_animation_header*& _fleet_box_bottom_lg_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059782C});

    inline s_animation_header*& _fleet_box_close_lg_button_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597924});
    inline s_animation_header*& _fleet_box_close_sm_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A1CAC});
    inline s_animation_header*& _fleet_box_engage_lg_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596C54});
    inline s_animation_header*& _fleet_box_engage_sm_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597D74});
    inline s_animation_header*& _fleet_box_colonize_lg_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B5AC});
    inline s_animation_header*& _fleet_box_colonize_sm_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00597494});
    inline s_animation_header*& _fleet_box_transport_lg_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B9FC});
    inline s_animation_header*& _fleet_box_transport_sm_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B310});
    inline s_animation_header*& _fleet_box_outpost_lg_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BF48});
    inline s_animation_header*& _fleet_box_outpost_sm_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BD18});
    inline s_animation_header*& _fleet_box_att_xenons_lg_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059B5E0});
    inline s_animation_header*& _fleet_box_att_xenons_sm_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BA68});

    inline s_animation_header*& _fleet_box_bottom_sm_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BA00});

    inline s_animation_header*& _fleet_box_grid_lg_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BEEC});

    inline int16_t* _fleet_movement_box_ptr =
            reinterpret_cast<int16_t*>(std::uintptr_t{0x00596210});

    inline s_fleet_box& _fleet_box =
        *reinterpret_cast<s_fleet_box*>(std::uintptr_t{0x005977C0});

    inline s_animation_header*& _fleet_box_scroll_dn_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596C58});
    inline int16_t& _fleet_movement_bar_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597A90});
    inline s_animation_header*& _fleet_box_scroll_up_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059BA70});
    inline s_animation_header*& _fleet_box_all_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0059C02C});

    inline int8_t& _mouse_button_pressed = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A1B58});

    inline s_fleet_movement_icon (&_fleet_movement_icon)[500] =
        *reinterpret_cast<s_fleet_movement_icon(*)[500]>(std::uintptr_t{0x0059C470});

    inline s_save_game_description (&_save_game_description)[10] = *reinterpret_cast<s_save_game_description(*)[10]>(std::uintptr_t{0x005974F0});

    inline int16_t& _popup_base_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597710});
    inline int16_t& _popup_base_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059770E});
    inline int16_t& _screen_data = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B20});
    inline int16_t& _screen_mouse_list_type = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005a1b26 });
    inline int16_t& _temp_star_handle = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005a1b28 });
    inline uint8_t& _moving_a_stack = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CEB4});
    inline uint8_t& _track_time_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CEC0});
    inline void*& _temp_bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x00596130});

    // Suggested name: MOX::_gate_icon_seg
    inline s_animation_header* (&_gate_icon_seg)[16] =
        *reinterpret_cast<s_animation_header* (*)[16]>(std::uintptr_t{0x0059BCD0});

    // Suggested name: MOX::gate_portal_icons
    inline s_animation_header** gate_portal_icons = reinterpret_cast<s_animation_header**>(0x0059BCD0);
    // Suggested name: MOX::gate_artemis_icons
    inline s_animation_header** gate_artemis_icons = reinterpret_cast<s_animation_header**>(0x0059BD00);

    // Suggested name: MOX::DAT_0059bd1e
    inline int16_t& DAT_0059bd1e =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BD1E});

    // Suggested name: MOX::moveable_box_display_order
    inline int16_t (&_moveable_box_order)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x0059BD20});

    // Suggested name: moveable_box_order_element_1
    inline int16_t& DAT_0059bd22 =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BD22});

    // Suggested name: MOX::box_visibility_flags
    inline int16_t (&_box_off_flag)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x0059C420});

    struct s_saved_box {
        int16_t field_id;
        int8_t is_open;
    };

    inline s_saved_box (&_saved_boxes)[5] =
        *reinterpret_cast<s_saved_box(*)[5]>(std::uintptr_t{0x005A1850});

    inline int16_t& _galaxy_map_grid_field =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596CAC});

    inline int16_t& _galaxy_map_height =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596cb0});

    inline int16_t& _galaxy_map_grid_y =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005a1b4e});

    inline int16_t& _galaxy_map_width = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059b428});
    inline int16_t& _galaxy_map_grid_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B52});

    inline int16_t (&_galaxy_map_star_field)[72] =
        *reinterpret_cast<int16_t(*)[72]>(std::uintptr_t{0x0059B280});

    inline s_animation_header* (&_fltscrn_flashing_ship_pict_seg)[8] =
        *reinterpret_cast<s_animation_header*(*)[8]>(std::uintptr_t{0x005976E0});

    inline s_animation_header* (&_fltscrn_flashing_monster_ship_icon)[8] =
        *reinterpret_cast<s_animation_header*(*)[8]>(std::uintptr_t{0x005A2C00});

    void __cdecl Apply_Save_Settings(s_settings* s);
}