#include "pch.h"

namespace PLNTSUM {
    void __cdecl Add_Planet_Summary_Bar_Field_() {
        _plntsum_scroll_bar = fields::Add_Scroll_Field_(
            0x1a2, // 418
            0x25,  // 37
            0,
            0x197, // 407
            0,
            0x197, // 407
            0xf,   // 15
            0x197, // 407
            reinterpret_cast<short*>(MOX::_scroll_bar),
            s__00551100,
            0
        );
    }

    bool __cdecl Send_Colony_Button_Selectable_() {
        return _found_colony_ship != 0;
    }

    bool __cdecl Send_Outpost_Button_Selectable_() {
        return _found_outpost_ship != 0;
    }

    bool __cdecl Planet_Is_Outpost_Planet_(short planet_idx) {
        short colony_idx = MOX::_planet[planet_idx].colony_index;

        if (colony_idx > -1) {
            return MOX::_colony[colony_idx].outpost_flag != 0;
        }

        return false;
    }

    bool __cdecl Planet_Has_Players_Colony_(int16_t planet_idx) {
        int16_t owner_id = -1;
        int16_t colony_index = MOX::_planet[planet_idx].colony_index;

        if (colony_index != -1) {
            // colony_index of -1 (0xFFFF) indicates no colony or just an outpost
            owner_id = (int16_t)MOX::_colony[colony_index].owner;
        }

        return owner_id == MOX::_PLAYER_NUM;
    }

    bool __cdecl Planet_Has_Been_Visited_(int16_t planet_index) {
        bool visited = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, (int16_t)MOX::_planet[planet_index].star_index);
        bool omniscient = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);

        if (omniscient || visited) {
            return true;
        }

        return false;
    }

    bool __cdecl Planet_In_Range_Of_A_Colony_(short planet_index) {
        int8_t star_idx = MOX::_planet[planet_index].star_index;
        s_star_data* star = &MOX::_star[star_idx];

        return SHIPMOVE::Star_In_Extended_Range_Of_Player_(star, MOX::_PLAYER_NUM);
    }

    bool __cdecl Planet_Has_Adverse_Gravity_(int16_t planet_idx, int16_t player_idx) {
        int16_t gravity_class = static_cast<int16_t>(MOX::_planet[planet_idx].gravity_class);

        // Productivity_Penalty_ returns a positive value if there is an adverse effect
        int16_t penalty = Productivity_Penalty_(player_idx, gravity_class);

        return penalty > 0;
    }

    bool __cdecl Planet_Is_Mineral_Scarce_(short planet_idx) {
        if (MOX::_planet[planet_idx].mineral_class == MINERAL_RESOURCE_POOR || 
            MOX::_planet[planet_idx].mineral_class == MINERAL_RESOURCE_ULTRA_POOR) {
            return true;
        }
        return false;
    }

    bool __cdecl Planet_Has_Hostile_Environment_(short planet_idx) {
        return MOX::_planet[planet_idx].food_per_farmer <= 0;
    }

    bool __cdecl Planet_Is_Enemy_Controlled_(int16_t planet_idx) {
        bool result = false;
        int8_t star_idx = MOX::_planet[planet_idx].star_index;

        for (int16_t i = 0; i < 5; ++i) {
            if (result) {
                return result;
            }
            int16_t p_idx = MOX::_star[star_idx].planet_index[i];
            if (p_idx > -1) {
                int16_t colony_idx = MOX::_planet[p_idx].colony_index;
                if (colony_idx > -1) {
                    if (MOX::_colony[colony_idx].owner != MOX::_PLAYER_NUM) {
                        result = true;
                    }
                }
            }
        }

        if (result) {
            return result;
        }

        for (int16_t i = 0; i < (int16_t)MOX::_ship_stack_count; ++i) {
            int16_t node_idx = MOX::_ship_stack_start[i];
            bool done_traversing = false;
            while (!done_traversing) {
                if (node_idx == -1) {
                    done_traversing = true;
                } else {
                    int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                    if (MOX::_ship[ship_idx].location == static_cast<int16_t>(star_idx)) {
                        if (HACCESS::Ship_Owner_(ship_idx) != MOX::_PLAYER_NUM) {
                            done_traversing = true;
                            result = true;
                        }
                    }
                    node_idx = MOX::_ship_node[node_idx].next_node;
                }
            }
        }

        return result;
    }

    int16_t __cdecl Colony_Ship_Auto_Colonization_(s_ship_data* ship) {
        if (ship->d.ship_weapon[1].type == 1) {
            return ship->d.ship_weapon[0].type;
        }
        return -1;
    }

    bool __cdecl Ship_Is_Not_Colonizing_(s_ship_data* ship) {
        int16_t location = ship->location;

        bool is_at_planet = (location >= 0 && location < 500);
        bool is_in_warp_range = (location >= 1000 && location < 1500);

        int16_t auto_col_status = Colony_Ship_Auto_Colonization_(ship);

        if (is_at_planet) {
            return true;
        }

        if (is_in_warp_range && auto_col_status == -1) {
            return true;
        }

        return false;
    }

    void __cdecl Update_Ship_Etas_() {
        for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
            _planet_summary[i].colony_eta = -1;
            _planet_summary[i].outpost_eta = -1;
        }

        for (int16_t ship_idx = 0; ship_idx < (int16_t)MOX::_NUM_SHIPS; ++ship_idx) {
            int16_t star_id = HAROLD::Absolute_Location_(MOX::_ship[ship_idx].location);
            int16_t owner = HACCESS::Ship_Owner_(ship_idx);

            if (owner == MOX::_PLAYER_NUM && star_id > -1) {
                uint8_t ship_type = MOX::_ship[ship_idx].d.ship_type;
                if (ship_type == 1 /* SHIP_TYPE_COLONY */ || ship_type == 4 /* SHIP_TYPE_OUTPOST */) {
                    int16_t turns = HAROLD::N_Turns_To_Star_(ship_idx, star_id);
                    int16_t planet_idx = Colony_Ship_Auto_Colonization_(&MOX::_ship[ship_idx]);

                    int16_t summary_idx = -1;
                    if (planet_idx > -1) {
                        summary_idx = Planet_Summary_List_Id_(planet_idx);
                    }

                    if (summary_idx > -1) {
                        if (ship_type == 1 /* SHIP_TYPE_COLONY */) {
                            _planet_summary[summary_idx].colony_eta = (int8_t)turns;
                        } else {
                            _planet_summary[summary_idx].outpost_eta = (int8_t)turns;
                        }
                    }
                }
            }
        }
    }

    int16_t __cdecl First_Planet_In_Summary_List_(int16_t star_index) {
        int16_t result = -1;
        int16_t i = 0;

        if (MOX::_scroll_bar->total_rows > 0) {
            do {
                if (result != -1) {
                    break;
                }

                int16_t planet_index = _planet_summary[i].explored;

                if (MOX::_planet[planet_index].star_index == star_index) {
                    result = i;
                }

                i++;
            } while (i < MOX::_scroll_bar->total_rows);
        }

        return result;
    }

    int16_t __cdecl Summary_Planet_Index_(int16_t star_index) {
        int16_t result = -1;

        for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; i++) {
            if (result != -1) {
                break;
            }

            int16_t planet_idx = PLNTSUM::_planet_summary[i].explored;

            if (MOX::_planet[planet_idx].star_index == (int8_t)star_index) {
                result = i;
            }
        }

        return result;
    }

    void __cdecl Add_Colony_Ship_Icon_Field_(int16_t* field_id, int16_t row_index) {
        // pics[13] corresponds to the colony ship icon animation header
        s_animation_header* ship_icon = PLNTSUM::_screen->pics[13];

        int16_t icon_w = animate::Get_Width_(ship_icon);
        int16_t icon_h = animate::Get_Height_(ship_icon);

        // Calculate coordinates based on hardcoded offsets and row scaling
        int16_t x1 = 100 /* 0x64 */ - icon_w;
        int16_t y1 = (row_index * 55 /* 0x37 */) - icon_h + 82 /* 0x52 */;

        int16_t x2 = x1 + icon_w;
        int16_t y2 = y1 + icon_h;

        // Register the field and store its handle
        *field_id = fields::Add_Hidden_Field_(x1, y1, x2, y2, s__00551100, 0);
    }

    void __cdecl Add_Outpost_Ship_Icon_Field_(int16_t* field_id, int16_t row) {
        s_animation_header* pic = _screen->pics[13];
        int16_t width = animate::Get_Width_(pic);
        int16_t height = animate::Get_Height_(pic);

        // Calculate top-left Y based on row index and animation height
        int16_t y = (row * 55 - height) + 84;

        *field_id = fields::Add_Hidden_Field_(
            102,
            y,
            width + 102,
            y + height,
            s__00551100,
            0
        );
    }

    void __cdecl Reset_Explored_Planets_() {
        memset(_explored_bitmap, 0, sizeof(_explored_bitmap));
    }

    void __cdecl Set_Planet_Explored_Flags_() {
        Reset_Explored_Planets_();

        for (int16_t i = 0; i < (int16_t)MOX::_planet_count; ++i) {
            // Check if the current player can view the star system containing this planet
            if (SYS::Ok_To_View_System_(MOX::_planet[i].star_index, MOX::_PLAYER_NUM)) {
                Set_Bit_Field_(PLNTSUM::_explored_bitmap, i);
            }
        }
    }

    int16_t __cdecl Productivity_Penalty_(int16_t player_idx, int16_t planet_gravity) {
        s_player* plr = &MOX::_player[player_idx];

        int8_t low_g_trait = plr->traits[TRAIT_LOW_G_WORLD];
        int8_t heavy_g_trait = plr->traits[TRAIT_HEAVY_G_WORLD];

        // A race is considered "Normal Gravity" if it has neither Low-G nor Heavy-G traits.
        bool is_normal_g_race = (low_g_trait == 0 && heavy_g_trait == 0);

        // 50% Penalty Conditions:
        // - Low-G race on Heavy-G planet (2)
        // - Normal-G race on Heavy-G planet (2)
        // - Heavy-G race on Low-G planet (0)
        if ((low_g_trait != 0 && planet_gravity == PLANET_GRAVITY_HEAVY_G) ||
            (is_normal_g_race && planet_gravity == PLANET_GRAVITY_HEAVY_G) ||
            (heavy_g_trait != 0 && planet_gravity == PLANET_GRAVITY_LOW_G)) {
            return 50;
        }

        // 25% Penalty Conditions:
        // - Low-G race on Normal-G planet (1)
        // - Normal-G race on Low-G planet (0)
        // (Note: Heavy-G races do not suffer 25% penalties on Normal-G planets)
        if ((low_g_trait != 0 && planet_gravity == PLANET_GRAVITY_NORMAL_G) ||
            (is_normal_g_race && planet_gravity == PLANET_GRAVITY_LOW_G)) {
            return 25;
        }

        return 0;
    }

    void __cdecl Add_Plntsum_Fields_() {
        _screen->buttons[0] = fields::Add_Multi_Button_Field_(441, 200, s__00551100, _screen->pics[1], &_sort_choice, 0, s__00551100, 41);
        _screen->buttons[1] = fields::Add_Multi_Button_Field_(501, 200, s__00551100, _screen->pics[2], &_sort_choice, 1, s__00551100, 41);
        _screen->buttons[2] = fields::Add_Multi_Button_Field_(567, 200, s__00551100, _screen->pics[3], &_sort_choice, 2, s__00551100, 41);

        _screen->buttons[3] = fields::Add_Radio_Button_Field_(441, 266, s__00551100, _screen->pics[4], &_filter_out_enemy_controlled_planets, s_1, 41);
        _screen->buttons[4] = fields::Add_Radio_Button_Field_(441, 289, s__00551100, _screen->pics[5], &_filter_out_adverse_gravity_planets, s_2, 41);
        _screen->buttons[5] = fields::Add_Radio_Button_Field_(441, 312, s__00551100, _screen->pics[6], &_filter_out_hostile_environment_planets, s_3, 41);
        _screen->buttons[6] = fields::Add_Radio_Button_Field_(441, 335, s__00551100, _screen->pics[7], &_filter_out_mineral_scarcity_planets, s_4, 41);
        _screen->buttons[7] = fields::Add_Radio_Button_Field_(441, 358, s__00551100, _screen->pics[8], &_filter_out_planets_out_of_range, s_5, 41);

        if (Send_Colony_Button_Selectable_()) {
            _screen->buttons[8] = fields::Add_Hidden_Field_(386, 408, 609, 413, s_C, 41);
        } else {
            _screen->buttons[8] = -1000;
        }

        if (Send_Colony_Button_Selectable_()) {
            _screen->buttons[9] = fields::Add_Hidden_Field_(413, 437, 610, 442, s_O, 41);
        } else {
            _screen->buttons[9] = -1000;
        }

        for (int i = 0; i < 8; ++i) {
            _screen->colony_icons[i] = -1000;
            _screen->output_icons[i] = -1000;
        }

        for (int i = 0; i < 8; ++i) {
            int row = MOX::_scroll_bar->first_visible_row + i;
            if (_planet_summary[row].colony_eta >= 0) {
                Add_Colony_Ship_Icon_Field_(&_screen->colony_icons[i], i);
                // Note: The original assembly writes a byte to the low byte of the short global_var.
                *reinterpret_cast<int8_t*>(&MOX::_global_var) = static_cast<int8_t>(_screen->colony_icons[i]);
            }
            if (_planet_summary[row].outpost_eta >= 0) {
                Add_Outpost_Ship_Icon_Field_(&_screen->output_icons[i], i);
            }
        }

        for (int i = 0; i < 8; ++i) {
            int16_t y1 = _plntscrn_delta_y[i] + 36;
            _screen->fields[i] = fields::Add_Hidden_Field_(18, y1, 414, y1 + 50, s__00551100, 41);
        }

        _screen->up_button = fields::Add_Button_Field_(422, 16, s__00551100, _screen->pics[9], s_minus, 40);
        _screen->dn_button = fields::Add_Button_Field_(421, 448, s__00551100, _screen->pics[10], s_plus, 40);

        Add_PlntSum_Galaxy_Map_Fields_(_screen->planet_icons);
        Add_Planet_Summary_Bar_Field_();

        _screen->esc_button = fields::Add_Button_Field_(454, 440, s__00551100, _screen->pics[11], s_space, 40);

        MOX::_scroll_bar->up_btn = _screen->up_button;
        MOX::_scroll_bar->dn_btn = _screen->dn_button;
    }

    void __cdecl Allocate_Space_For_Summary_Screen_() {
        _planet_summary = (s_plntsum_planet*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x2760);
        memset(_planet_summary, 0xFF, 0x2760);

        _screen = (s_plntsum_screen*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x115);
        memset(_screen, 0, 0x115);

        _ship_summary = (s_plntsum_ship*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xFA0);
        memset(_ship_summary, 0xFF, 0xFA0);

        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x1D);
        memset(MOX::_scroll_bar, 0, 0x1D);
    }

    void __cdecl Load_Planet_Summary_Pictures_(void) {
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        _screen->pics[0] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 0, MOX::_screen_seg);
        _screen->pics[1] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 1, MOX::_screen_seg);
        _screen->pics[2] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 2, MOX::_screen_seg);
        _screen->pics[3] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 3, MOX::_screen_seg);

        MOX::Mox_Update_();

        _screen->pics[4] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 4, MOX::_screen_seg);
        _screen->pics[5] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 5, MOX::_screen_seg);
        _screen->pics[6] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 6, MOX::_screen_seg);
        _screen->pics[7] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 7, MOX::_screen_seg);

        MOX::Mox_Update_();

        _screen->pics[8] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 8, MOX::_screen_seg);

        _send_colony_ship_button_seg = (uint32_t)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 9, MOX::_screen_seg);
        _send_outpost_ship_button_seg = (uint32_t)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 10, MOX::_screen_seg);

        _screen->pics[9] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 11, MOX::_screen_seg);
        _screen->pics[10] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 12, MOX::_screen_seg);
        _screen->pics[12] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 13, MOX::_screen_seg);

        MOX::Mox_Update_();

        _screen->pics[11] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 14, MOX::_screen_seg);

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        for (int i = 0; i < 11; ++i) {
            MOX::_planet_summary_galaxy_star_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 15 + i, MOX::_screen_seg);
            MOX::Mox_Update_();
        }

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        Load_Player_Icon_();
        palette::Set_Palette_Changes_(0, 255);
        reduce::Clear_Reduced_Palette_Groups_();
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
    }

    void __cdecl Determine_Button_Status_() {
        Build_Ship_Summary_();

        _draw_colony_button_status = 0;
        _draw_outpost_button_status = 0;

        if (MOX::_scroll_bar->total_rows > 0 && EVENTS::Event_Check_Hyperspace_Flux_() == 0) {
            // Check both colony and outpost viability
            if (Send_Colony_Button_Selectable_() && Send_Outpost_Button_Selectable_()) {
                _draw_colony_button_status = 2;
                _send_choice = 0;

                // Redundant check in original binary
                if (Send_Colony_Button_Selectable_()) {
                    _draw_outpost_button_status = 1;
                }
                return;
            }

            // Handle mutually exclusive or single-option scenarios
            if (Send_Outpost_Button_Selectable_()) {
                if (!Send_Colony_Button_Selectable_()) {
                    _draw_colony_button_status = 2;
                    _send_choice = 0;
                    return;
                }
            } else if (Send_Colony_Button_Selectable_()) {
                _draw_outpost_button_status = 2;
                _send_choice = 1;
            }
        }
    }

    void Build_Ship_Summary_() {
        _n_ships_in_summary = 0;
        // Clears the whole summary buffer (500 entries of 8 bytes = 4000 bytes)
        memset(_ship_summary, 0xFF, 4000);
        // Clears star reachability flags (18 entries of 4 bytes = 72 bytes)
        memset(_can_reach_star, 0, 18 * sizeof(uint32_t));

        _found_colony_ship = 0;
        _found_outpost_ship = 0;

        if (MOX::_ship_stack_count > 0) {
            for (int16_t stack_idx = 0; stack_idx < MOX::_ship_stack_count; ++stack_idx) {
                int16_t node_idx = MOX::_ship_stack_start[stack_idx];
                int16_t first_node_ship_idx = MOX::_ship_node[node_idx].ship_idx;

                if (HACCESS::Player_Can_Order_Ship_(MOX::_PLAYER_NUM, &MOX::_ship[first_node_ship_idx]) && node_idx != -1) {
                    bool stop_stack = false;
                    do {
                        int16_t icon_id = HACCESS::Ship_Icon_Id_From_Node_Ptr_(node_idx);
                        _ship_summary[_n_ships_in_summary].ship_icon_id = icon_id;

                        int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                        int16_t owner = HACCESS::Ship_Owner_(ship_idx);
                        s_ship_data* ship = &MOX::_ship[ship_idx];

                        if (ship->d.ship_type == 1 /* SHIP_TYPE_COLONY */ && owner == MOX::_PLAYER_NUM && ship->status < 3) {
                            _ship_summary[_n_ships_in_summary].colony_ship_idx = ship_idx;
                            if (Ship_Is_Not_Colonizing_(ship)) {
                                _found_colony_ship = 1;
                                Set_Colony_Ship_To_Star_Distances_(_n_ships_in_summary, ship_idx);
                                _ship_summary[_n_ships_in_summary].colony_available = true;
                            } else {
                                _ship_summary[_n_ships_in_summary].colony_available = false;
                            }
                        } else if (ship->d.ship_type == 4 /* SHIP_TYPE_OUTPOST */ && owner == MOX::_PLAYER_NUM && ship->status < 3) {
                            _ship_summary[_n_ships_in_summary].outpost_ship_idx = ship_idx;
                            if (Ship_Is_Not_Colonizing_(ship)) {
                                _found_outpost_ship = 1;
                                Set_Outpost_Ship_To_Star_Distances_(_n_ships_in_summary, ship_idx);
                                _ship_summary[_n_ships_in_summary].output_available = true;
                            } else {
                                _ship_summary[_n_ships_in_summary].output_available = false;
                            }
                        }

                        _n_ships_in_summary++;

                        if (_found_colony_ship != 0 && _found_outpost_ship != 0) {
                            stop_stack = true;
                        }

                        if (!stop_stack) {
                            node_idx = MOX::_ship_node[node_idx].next_node;
                            if (node_idx == -1) {
                                stop_stack = true;
                            }
                        }
                    } while (!stop_stack);
                }
            }
        }

        Update_Ship_Etas_();
    }

    void __cdecl Filter_Explored_Planets_(short* summary_count) {
        short original_count = *summary_count;
        MOX::Push_Block_(MOX::_screen_seg);

        // Allocate buffer for 360 planets (0x2760 / 28 bytes)
        s_plntsum_planet* temp_buffer = (s_plntsum_planet*)MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x2760);

        // Initialize temp buffer with 0xFF (sets all shorts to -1)
        memset(temp_buffer, 0xFF, 0x2760);

        *summary_count = 0;

        for (int16_t i = 0; i < (int16_t)MOX::_planet_count; ++i) {
            MOX::Mox_Update_();

            bool passes_filter = true;

            // Initial requirements: must be a planet, not owned by player, not an outpost
            if (MOX::_planet[i].planet_type != 3 /* PLANET_TYPE_PLANET */) {
                passes_filter = false;
            } else if (Planet_Has_Players_Colony_(i)) {
                passes_filter = false;
            } else if (Planet_Is_Outpost_Planet_(i)) {
                passes_filter = false;
            }

            // Visitation and Exploration requirements
            if (passes_filter) {
                if (!Planet_Has_Been_Visited_(i)) {
                    passes_filter = false;
                } else if (!Test_Bit_Field_(PLNTSUM::_explored_bitmap, i)) {
                    passes_filter = false;
                }
            }

            // Optional environmental/range filters
            if (passes_filter) {
                if (PLNTSUM::_filter_out_planets_out_of_range != 0 && !Planet_In_Range_Of_A_Colony_(i)) {
                    passes_filter = false;
                }
                if (passes_filter && PLNTSUM::_filter_out_adverse_gravity_planets != 0 && Planet_Has_Adverse_Gravity_(i, MOX::_PLAYER_NUM)) {
                    passes_filter = false;
                }
                if (passes_filter && PLNTSUM::_filter_out_enemy_controlled_planets != 0 && Planet_Is_Enemy_Controlled_(i)) {
                    passes_filter = false;
                }
                if (passes_filter && PLNTSUM::_filter_out_hostile_environment_planets != 0 && Planet_Has_Hostile_Environment_(i)) {
                    passes_filter = false;
                }
                if (passes_filter && PLNTSUM::_filter_out_mineral_scarcity_planets != 0 && Planet_Is_Mineral_Scarce_(i)) {
                    passes_filter = false;
                }
            }

            // If all conditions met, record planet summary
            if (passes_filter) {
                s_plntsum_planet* p_sum = &temp_buffer[*summary_count];
                p_sum->max_pop = COLCALC::Planet_Max_Population_For_Player_(&MOX::_planet[i], MOX::_PLAYER_NUM);
                p_sum->food_per_farmer = (int16_t)MOX::_planet[i].food_per_farmer;
                p_sum->minerals = (int16_t)COLCALC::_minerals_per_mine[(uint8_t)MOX::_planet[i].mineral_class];
                p_sum->mineral_class = (int16_t)MOX::_planet[i].mineral_class;
                p_sum->climate = (int16_t)MOX::_planet[i].climate;
                p_sum->explored = i;

                (*summary_count)++;
            }
        }

        // If the count of visible planets changed, update the global summary
        if (*summary_count != original_count) {
            memcpy(PLNTSUM::_planet_summary, temp_buffer, 0x2760);
            PLNTSUM::Sort_Planet_Summary_();
            MOX::_scanned_field = -1;
        }

        MOX::Pop_Block_(MOX::_screen_seg);
    }

    int16_t __cdecl Planet_Summary_Id_(int16_t explored_id) {
        int16_t result_index = -1;
        int16_t total_rows = MOX::_scroll_bar->total_rows;

        for (int16_t i = 0; i < total_rows; ++i) {
            // If we have already found a matching index, exit the loop.
            // Note: This logic follows the assembly check at the start of the loop block.
            if (result_index != -1) {
                break;
            }

            if (PLNTSUM::_planet_summary[i].explored == explored_id) {
                result_index = i;
            }
        }

        return result_index;
    }

    void __cdecl Sort_Planet_Summary_() {
        switch (_sort_choice) {
            case 0:
                _qsort(_planet_summary, (uint32_t)MOX::_scroll_bar->total_rows, 0x1c, qsort_by_climate_);
                break;
            case 1:
                _qsort(_planet_summary, (uint32_t)MOX::_scroll_bar->total_rows, 0x1c, qsort_by_max_population_);
                break;
            case 2:
                _qsort(_planet_summary, (uint32_t)MOX::_scroll_bar->total_rows, 0x1c, qsort_by_minerals_);
                break;
            default:
                break;
        }
    }

    void __cdecl Add_PlntSum_Galaxy_Map_Fields_(int16_t* fields_array) {
        for (short i = 0; i < 72; ++i) {
            fields_array[i] = -1000;
        }

        if (MOX::_NUM_STARS > 0) {
            for (short j = 0; j < (short)MOX::_NUM_STARS; ++j) {
                short x, y;
                MOVEBOX::Get_Galaxy_Map_Star_XY_(j, 443, 17, 180, 116, 0, 0, 0, &x, &y);
                fields_array[j] = fields::Add_Hidden_Field_(x, y, x + 5, y + 5, s__00551100, 41);
            }
        }
    }

    void __cdecl Scan_Summary_Box_Fields_(int16_t input_field_id, int16_t clicked_field_id, int16_t* last_input_field_id_ptr) {
        int first_row = (int)MOX::_scroll_bar->first_visible_row;
        int total_rows = (int)MOX::_scroll_bar->total_rows;
        int max_visible = first_row + 8;
        int end_idx = (first_row + total_rows < max_visible) ? (first_row + total_rows) : max_visible;

        PLNTSUM::_can_reach_scanned_planet = false;
        PLNTSUM::_box_field_scanned = -1;

        int count = end_idx - first_row;
        if (count > 0) {
            for (int16_t i = 0; i < (int16_t)count; ++i) {
                int current_row = first_row + i;
                int16_t explored_idx = PLNTSUM::_planet_summary[current_row].explored;

                if (explored_idx != -1) {
                    int16_t star_idx = (int16_t)MOX::_planet[explored_idx].star_index;

                    // Handle interaction: Click on fields/icons
                    if (PLNTSUM::_screen->fields[i] == clicked_field_id) {
                        if (PLNTSUM::_send_choice == 0 || PLNTSUM::_send_choice == 1) {
                            int16_t planet_id = PLNTSUM::Planet_Summary_Id_(explored_idx);
                            int16_t ship_id = PLNTSUM::Nearest_Ship_Id_(planet_id);
                            if (ship_id > -1) {
                                PLNTSUM::Send_Nearest_Ship_(explored_idx);
                                PLNTSUM::Determine_Button_Status_();
                                PLNTSUM::Reset_Ship_To_Star_Distances_();
                            }
                        }
                    } else if (PLNTSUM::_screen->colony_icons[i] == clicked_field_id) {
                        PLNTSUM::Undo_Send_Order_(explored_idx, 1);
                        PLNTSUM::Determine_Button_Status_();
                        PLNTSUM::Reset_Ship_To_Star_Distances_();
                    } else if (PLNTSUM::_screen->output_icons[i] == clicked_field_id) {
                        PLNTSUM::Undo_Send_Order_(explored_idx, 4);
                        PLNTSUM::Determine_Button_Status_();
                        PLNTSUM::Reset_Ship_To_Star_Distances_();
                    }
                    // Handle interaction: Hover/Input on fields
                    else if (PLNTSUM::_screen->fields[i] == input_field_id && current_row < total_rows) {
                        PLNTSUM::_box_field_scanned = (int8_t)i;
                        if (explored_idx > -1) {
                            int16_t planet_id = PLNTSUM::Planet_Summary_Id_(explored_idx);
                            int16_t ship_id = PLNTSUM::Nearest_Ship_Id_(planet_id);
                            if (ship_id != -1 && (PLNTSUM::_send_choice == 0 || PLNTSUM::_send_choice == 1)) {
                                PLNTSUM::_can_reach_scanned_planet = true;
                            }
                        }

                        if (*last_input_field_id_ptr != input_field_id) {
                            MOX::_scanned_field = star_idx + 1000 /* 0x3E8 */;
                            int16_t target_id = -1;
                            if (PLNTSUM::_send_choice == 0) {
                                target_id = *reinterpret_cast<int16_t*>(&PLNTSUM::_planet_summary[current_row].best_colony_ship_idx);
                            } else if (PLNTSUM::_send_choice == 1) {
                                target_id = *reinterpret_cast<int16_t*>(&PLNTSUM::_planet_summary[current_row].best_outpost_ship_idx);
                            }

                            if (target_id != -1) {
                                int16_t targets[2];
                                targets[0] = target_id;
                                targets[1] = -1;
                                SHIPMOVE::Ships_Try_To_Move_To_(targets, star_idx, &MOX::_g_ship_move_info);
                                MOX::_scanned_field = star_idx;
                            }
                            *last_input_field_id_ptr = input_field_id;
                        }
                    } else if (PLNTSUM::_screen->colony_icons[i] == input_field_id || PLNTSUM::_screen->output_icons[i] == input_field_id) {
                        PLNTSUM::_can_reach_scanned_planet = true;
                    }
                }
            }
        }
    }
}