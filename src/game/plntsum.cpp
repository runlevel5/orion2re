#include "pch.h"
#include <cstring>
#include <cstdio>

namespace PLNTSUM {

s_colors _hi_neutral_colors = {{0x40, 0xFF, 0x43, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
s_colors _red_colors = {{0xCF, 0xD3, 0xD2, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3}};
s_colors _green_colors = {{0xDB, 0xDE, 0xDD, 0xDE, 0xDE, 0xDE, 0xDE, 0xDE}};

int16_t _plntscrn_delta_y[9] = {0, 55, 109, 164, 219, 274, 329, 383, 479};
int16_t _sort_choice;
int16_t _filter_out_adverse_gravity_planets;
int16_t _filter_out_hostile_environment_planets;
int16_t _filter_out_mineral_scarcity_planets;
int16_t _filter_out_enemy_controlled_planets;
int16_t _filter_out_planets_out_of_range = 1;

s_plntsum_screen* _screen;
s_plntsum_planet* _planet_summary;
s_plntsum_ship* _ship_summary;
int16_t _n_ships_in_summary;
int8_t _can_reach_star[72];
uint8_t _explored_bitmap[45];

uint8_t _found_outpost_ship;
uint8_t _draw_colony_button_status;
int16_t _send_choice;
uint8_t _found_colony_ship;
uint8_t _draw_outpost_button_status;
int16_t _plntsum_scroll_bar;
uint32_t DAT_0056583c;
uint8_t DAT_00565840;

int16_t _last_ship_moved;
const char* PTR_s_PLNTSUM_LBX_00556AC0 = "PLNTSUM.LBX";
s_animation_header* _send_colony_ship_button_seg;
s_animation_header* _send_outpost_ship_button_seg;

bool _can_reach_scanned_planet;
int8_t _box_field_scanned;
bool _recalc_first_planet;
uint8_t _fade_flag;
uint8_t DAT_005658a4;
uint8_t _skip_draw_planets;

s_animation_header** _p_rotating_planet;

void __cdecl Summary_Screen_Print_Data_(void) {
    int16_t monster_race = -1;
    int first_visible = static_cast<int>(MOX::_scroll_bar->first_visible_row);
    int end_index = first_visible + static_cast<int>(MOX::_scroll_bar->total_rows);

    if (first_visible + 8 <= end_index) {
        end_index = first_visible + 8;
    }

    for (int16_t scroll_row = 0; scroll_row < (end_index - first_visible); ++scroll_row) {
        int16_t summary_idx = static_cast<int16_t>(first_visible + scroll_row);
        int16_t y_base = PLNTSUM::_plntscrn_delta_y[scroll_row] + 0x27;
        s_plntsum_planet* summary = &PLNTSUM::_planet_summary[summary_idx];

        if (summary->explored != -1) {
            s_planet_data* planet = &MOX::_planet[summary->explored];
            int16_t star_idx = static_cast<int16_t>(planet->star_index);
            int16_t mineral_class = summary->mineral_class;
            int16_t climate_idx = summary->climate;
            int16_t gravity_class = static_cast<int16_t>(planet->gravity_class);

            char color_choice = 0;
            if (PLNTSUM::_send_choice == 0 || PLNTSUM::_send_choice == 1) {
                if (scroll_row == (int16_t)PLNTSUM::_box_field_scanned) {
                    color_choice = (PLNTSUM::_can_reach_scanned_planet != 0) ? 1 : 2;
                }
            } else if ((int16_t)PLNTSUM::_box_field_scanned == scroll_row) {
                color_choice = 1;
            }

            s_colors font_colors;
            Colony_Owner_Colors_(planet->colony_index, &font_colors, color_choice);

            graphics::Set_Window_(0x12, y_base, 0x68, PLNTSUM::_plntscrn_delta_y[scroll_row + 1] + y_base);
            video::Clip_On_();
            fonts::Set_Font_Style_(1, &font_colors);

            int16_t special_id = 0;
            char system_special = MOX::_star[star_idx].system_special;
            if (system_special > 0 && system_special != 11 /* P_SPEC_NONE */) {
                special_id = static_cast<int16_t>(system_special);
            }

            if (special_id > 0 && HACCESS::Special_Is_Planet_Special_(special_id)) {
                char* special_str = nullptr;
                char special_buf[80];

                if (MOX::_star[star_idx].system_special == 9) {
                    int16_t ship_idx = HAROLD::First_Ship_Orbiting_Star_(&MOX::_star[star_idx]);
                    if (ship_idx != -1) {
                        special_str = MOX::_ship[ship_idx].d.name;
                    } else if (special_id == 10) {
                        strcpy(special_buf, HAROLD::H_Message_(140));
                        special_str = special_buf;
                    } else {
                        special_str = MOX::_planet_special_string[special_id];
                    }
                } else if (planet->planet_special == special_id) {
                    special_str = MOX::_planet_special_string[special_id];
                }

                if (special_str) {
                    fonts::Print_Centered_(0x3d, y_base, special_str);
                }
            }

            fonts::Set_Font_Style_(2, &font_colors);
            int16_t font_h = fonts::Get_Font_Height_();
            int16_t y_offset = (25 - font_h) - 4;

            char planet_name_buf[32];
            strcpy(planet_name_buf, HACCESS::Get_Planet_Name_No_Prime_(summary->explored));
            MISC::Set_Fitted_Font_Style_(planet_name_buf, &font_colors, 2, 2, 0, 0x56, 1);
            int16_t fitted_name_h = fonts::Get_Font_Height_();
            fonts::Print_Centered_(0x3d, y_base + (25 - fitted_name_h) * 2, planet_name_buf);

            char owner_label[100];
            bool has_owner_label = false;

            if (planet->colony_index != -1) {
                char* race_name = MOX::_player[MOX::_colony[planet->colony_index].owner].race_name;
                sprintf(owner_label, "(%s)", race_name);
                has_owner_label = true;
            } else if (HAROLD::Star_Guarded_By_Monster_(star_idx, &monster_race)) {
                char race_buf[20];
                HACCESS::Race_Name_(monster_race, race_buf);
                sprintf(owner_label, "(%s)", race_buf);
                has_owner_label = true;
            }

            if (has_owner_label) {
                fonts::Set_Font_Style_(1, &font_colors);
                fonts::Print_Centered_(0x3d, (y_base - fonts::Get_Font_Height_()) + 0x30, owner_label);
            }

            video::Clip_Off_();
            graphics::Reset_Window_();

            int16_t y_climate = y_base + y_offset;
            char* climate_str = MOX::_planet_climate_string[climate_idx];
            MISC::Set_Fitted_Font_Style_(climate_str, &font_colors, 2, 2, -1, 0x41, 1);
            int16_t climate_h = fonts::Get_Font_Height_();
            fonts::Print_Centered_(0x8c, y_climate, climate_str);

            char work_buf[100];
            fonts::Set_Font_Style_(1, &font_colors);
            sprintf(work_buf, HAROLD::H_Message_(0x141), (int)summary->food_per_farmer);
            fonts::Print_Centered_(0x8c, y_climate + climate_h + 4, work_buf);

            char* gravity_str = MOX::_planet_gravity_string[gravity_class];
            fonts::Set_Font_Style_(2, &font_colors);
            int16_t gravity_h = fonts::Get_Font_Height_();
            int16_t penalty = Productivity_Penalty_(MOX::_PLAYER_NUM, gravity_class);
            int16_t y_gravity;

            if (penalty == 0) {
                y_gravity = y_climate + (gravity_h / 2);
            } else {
                fonts::Print_Centered_(0xd9, y_climate, gravity_str);
                fonts::Set_Font_Style_(1, &font_colors);
                sprintf(work_buf, HAROLD::H_Message_(0x142), (int)penalty);
                y_gravity = y_climate + gravity_h + 4;
            }
            fonts::Print_Centered_(0xd9, y_gravity, (penalty == 0) ? gravity_str : work_buf);

            fonts::Set_Font_Style_(2, &font_colors);
            int16_t mineral_h = fonts::Get_Font_Height_();
            fonts::Print_Centered_(0x137, y_climate, MOX::_mineral_class_string[mineral_class]);
            fonts::Set_Font_Style_(1, &font_colors);
            sprintf(work_buf, HAROLD::H_Message_(0x152), (int)summary->minerals);
            fonts::Print_Centered_(0x137, y_climate + mineral_h + 4, work_buf);

            fonts::Set_Font_Style_(2, &font_colors);
            int16_t size_h = fonts::Get_Font_Height_();
            fonts::Print_Centered_(0x182, y_climate, MOX::_planet_size_string[planet->size]);
            fonts::Set_Font_Style_(1, &font_colors);
            sprintf(work_buf, HAROLD::H_Message_(0x143), (int)summary->max_pop);
            fonts::Print_Centered_(0x182, y_climate + size_h + 4, work_buf);

            if (summary->colony_eta >= 0) {
                PLNTSUM::Draw_Colony_ETA_(summary_idx, scroll_row);
            }
            if (summary->outpost_eta >= 0) {
                PLNTSUM::Draw_Outpost_ETA_(summary_idx, scroll_row);
            }
        }
    }
}

void __cdecl Send_Nearest_Ship_(int16_t planet_idx) {
    char msg_buffer[120];
    int16_t ship_list[2];
    s_ship_move_info move_info;
    int16_t nearest_ship_idx;
    int16_t summary_idx;
    int16_t target_star_idx;
    int16_t ship_origin_star_idx;
    bool should_continue = true;
    bool omniscient;
    uint8_t ship_type;
    char race_name_buffer[32];

    ship_list[0] = -1;
    ship_list[1] = -1;

    target_star_idx = (int16_t)MOX::_planet[planet_idx].star_index;
    omniscient = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
    summary_idx = Planet_Summary_Id_(planet_idx);
    nearest_ship_idx = Nearest_Ship_Id_(summary_idx);

    ship_type = MOX::_ship[nearest_ship_idx].d.ship_type;
    ship_origin_star_idx = HAROLD::Absolute_Location_(MOX::_ship[nearest_ship_idx].location);

    if (((ship_origin_star_idx != target_star_idx) && (ship_type == 1 /* COLONY_SHIP */) && (PLNTSUM::_planet_summary[summary_idx].colony_eta > 0)) ||
        ((ship_type == 4 /* OUTPOST_SHIP */) && (PLNTSUM::_planet_summary[summary_idx].outpost_eta > 0))) {
        
        sprintf(msg_buffer, HAROLD::H_Message_(0x134 /* HESTR_134_YOU_ALREADY_HAVE_ONE_C */), &MOX::_star[target_star_idx]);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        should_continue = (GENDRAW::Confirmation_Box_(msg_buffer) != 0);
        PLNTSUM::_screen->refresh_fields = true;
        if (!should_continue) {
            return;
        }
    }

    if (nearest_ship_idx <= -1 || MOX::_ship[nearest_ship_idx].location < 500) {
        if (nearest_ship_idx == -1) {
            should_continue = false;
        }
    } else {
        bool visited_origin = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, ship_origin_star_idx);
        bool visited_dest = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, target_star_idx);

        if (ship_origin_star_idx == target_star_idx) {
            sprintf(msg_buffer, HAROLD::H_Message_(0x138 /* HESTR_138_S_ALREADY_HAS_A_COURSE */), &MOX::_ship[nearest_ship_idx], &MOX::_star[target_star_idx]);
            GENDRAW::Message_Box_(msg_buffer);
            PLNTSUM::_screen->refresh_fields = true;
            return;
        }

        if (omniscient || (visited_dest && visited_origin)) {
            sprintf(msg_buffer, HAROLD::H_Message_(0x139 /* HESTR_139_S_HAS_SET_A_COURSE_FOR */), &MOX::_ship[nearest_ship_idx], &MOX::_star[ship_origin_star_idx], &MOX::_star[target_star_idx]);
        } else if (visited_dest) {
            sprintf(msg_buffer, HAROLD::H_Message_(0x13A /* HESTR_13A_S_HAS_SET_A_COURSE_SHO */), &MOX::_ship[nearest_ship_idx], &MOX::_star[target_star_idx]);
        } else if (visited_origin) {
            sprintf(msg_buffer, HAROLD::H_Message_(0x139 /* HESTR_139_S_HAS_SET_A_COURSE_FOR */), &MOX::_ship[nearest_ship_idx], &MOX::_star[ship_origin_star_idx]);
        } else {
            sprintf(msg_buffer, HAROLD::H_Message_(0x13A /* HESTR_13A_S_HAS_SET_A_COURSE_SHO */), &MOX::_ship[nearest_ship_idx]);
        }

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        should_continue = (GENDRAW::Confirmation_Box_(msg_buffer) != 0);
        PLNTSUM::_screen->refresh_fields = true;
    }

    if (should_continue) {
        int16_t best_ship_idx;
        if (PLNTSUM::_send_choice == 1) {
            best_ship_idx = PLNTSUM::_planet_summary[summary_idx].best_outpost_ship_idx;
        } else {
            best_ship_idx = PLNTSUM::_planet_summary[summary_idx].best_colony_ship_idx;
        }

        ship_list[0] = best_ship_idx;
        ship_list[1] = -1;
        
        SHIPMOVE::Ships_Try_To_Move_To_(ship_list, target_star_idx, &move_info);
        
        if (move_info.moving == 0) {
            if (move_info.hyperspace_flux != 0) {
                GENDRAW::Warning_Box_(HAROLD::H_Message_(0x13F /* HESTR_13F_SHIP_MOVEMENT_IS_NOT_A */));
                PLNTSUM::_screen->refresh_fields = true;
            }
        } else {
            bool user_confirmed_move = true;
            int16_t monster_race_summary_id;
            if (HAROLD::Star_Guarded_By_Monster_(target_star_idx, &monster_race_summary_id)) {
                HACCESS::Race_Name_(monster_race_summary_id, race_name_buffer);
                mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                
                int16_t msg_id = (PLNTSUM::_send_choice == 0) ? 0x13C /* HESTR_13C_S_IS_GUARDED_BY_A_S_DO */ : 0x13B /* HESTR_13B_S_IS_GUARDED_BY_A_S_DO */;
                sprintf(msg_buffer, HAROLD::H_Message_(msg_id), &MOX::_star[target_star_idx], race_name_buffer);
                user_confirmed_move = (GENDRAW::Confirmation_Box_(msg_buffer) != 0);
            }

            if (user_confirmed_move) {
                SHIPMOVE::Make_Ships_Move_To_(ship_list, target_star_idx, &move_info);
                
                int16_t confirm_msg_id = (move_info.turns_left == 1) ? 0x13D : 0x13E;
                sprintf(msg_buffer, HAROLD::H_Message_(confirm_msg_id), &MOX::_ship[best_ship_idx], &MOX::_star[target_star_idx], (uint32_t)(uint8_t)move_info.turns_left);
                
                GENDRAW::Message_Box_(msg_buffer);
                PLNTSUM::_screen->refresh_fields = true;
                PLNTSUM::_last_ship_moved = best_ship_idx;
                PLNTSUM::Update_Ship_Etas_();
                PLNTSUM::Set_Auto_Colonize_Flag_(&MOX::_ship[best_ship_idx], planet_idx);
            }
        }
    }
}
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
            (short*)MOX::_scroll_bar,
            "",
            0
        );
    }

    bool __cdecl Summary_Found_Colony_Ship_(int16_t ship_summary_idx) {
        if (_ship_summary[ship_summary_idx].colony_ship_idx != -1 && _ship_summary[ship_summary_idx].colony_available) {
            return true;
        }
        return false;
    }

    bool __cdecl Summary_Found_Outpost_Ship_(int16_t ship_summary_idx) {
        if (_ship_summary[ship_summary_idx].outpost_ship_idx != -1 && _ship_summary[ship_summary_idx].output_available) {
            return true;
        }
        return false;
    }

    bool __cdecl Send_Colony_Button_Selectable_() {
        return _found_colony_ship != 0;
    }

    bool __cdecl Send_Outpost_Button_Selectable_() {
        return _found_outpost_ship != 0;
    }

    bool __cdecl Planet_Is_Outpost_Planet_(int16_t planet_idx) {
        int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

        if (colony_idx > -1) {
            return MOX::_colony[colony_idx].outpost_flag != 0;
        }

        return false;
    }

    bool __cdecl Planet_Has_Players_Colony_(int16_t planet_idx) {
        int16_t owner_id = -1;
        int16_t colony_index = MOX::_planet[planet_idx].colony_index;

        if (colony_index != -1) {
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

    bool __cdecl Planet_In_Range_Of_A_Colony_(int16_t planet_idx) {
        int8_t star_idx = MOX::_planet[planet_idx].star_index;
        s_star_data* star = &MOX::_star[star_idx];

        return SHIPMOVE::Star_In_Extended_Range_Of_Player_(star, MOX::_PLAYER_NUM);
    }

    bool __cdecl Planet_Has_Adverse_Gravity_(int16_t planet_idx, int16_t player_idx) {
        int16_t gravity_class = static_cast<int16_t>(MOX::_planet[planet_idx].gravity_class);

        // Productivity_Penalty_ returns a positive value if there is an adverse effect
        int16_t penalty = Productivity_Penalty_(player_idx, gravity_class);

        return penalty > 0;
    }

    bool __cdecl Planet_Is_Mineral_Scarce_(int16_t planet_idx) {
        if (MOX::_planet[planet_idx].mineral_class == MINERAL_RESOURCE_POOR || 
            MOX::_planet[planet_idx].mineral_class == MINERAL_RESOURCE_ULTRA_POOR) {
            return true;
        }
        return false;
    }

    bool __cdecl Planet_Has_Hostile_Environment_(int16_t planet_idx) {
        return MOX::_planet[planet_idx].food_per_farmer <= 0;
    }

    bool __cdecl Planet_Is_Enemy_Controlled_(int16_t planet_idx, int16_t) {
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

    void __cdecl Set_Auto_Colonize_Flag_(s_ship_data* ship, int16_t flag) {
        ship->d.ship_weapon[1].type = 1;
        ship->d.ship_weapon[0].type = flag;
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

            if ((int16_t)MOX::_planet[planet_idx].star_index == star_index) {
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
        *field_id = fields::Add_Hidden_Field_(x1, y1, x2, y2, "", 0);
    }

    void __cdecl Add_Outpost_Ship_Icon_Field_(int16_t* field_id, int16_t row_index) {
        s_animation_header* pic = _screen->pics[13];
        int16_t width = animate::Get_Width_(pic);
        int16_t height = animate::Get_Height_(pic);

        // Calculate top-left Y based on row index and animation height
        int16_t y = (row_index * 55 - height) + 84;

        *field_id = fields::Add_Hidden_Field_(
            102,
            y,
            width + 102,
            y + height,
            "",
            0
        );
    }

    void __cdecl Reset_Explored_Planets_() {
        memset(_explored_bitmap, 0, sizeof(_explored_bitmap));
    }

    void __cdecl Reset_Ship_To_Star_Distances_() {
        int choice_mode;

        if (PLNTSUM::_send_choice == 0) {
            choice_mode = 1;
        } else {
            choice_mode = 4;
        }

        for (short i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
            if (choice_mode == 1) {
                PLNTSUM::_planet_summary[i].best_colony_eta = 9999;
            } else if (choice_mode == 4) {
                PLNTSUM::_planet_summary[i].best_outpost_eta = 9999;
            }
        }

        for (int16_t i = 0; i < PLNTSUM::_n_ships_in_summary; ++i) {
            if (choice_mode == 1) {
                short ship_idx = PLNTSUM::_ship_summary[i].colony_ship_idx;
                if (ship_idx > -1 && ship_idx < MOX::_NUM_SHIPS) {
                    PLNTSUM::Set_Colony_Ship_To_Star_Distances_(i, ship_idx);
                }
            } else if (choice_mode == 4) {
                short ship_idx = PLNTSUM::_ship_summary[i].outpost_ship_idx;
                if (ship_idx > -1 && ship_idx < MOX::_NUM_SHIPS) {
                    PLNTSUM::Set_Outpost_Ship_To_Star_Distances_(i, ship_idx);
                }
            }
        }
    }

    void __cdecl Set_Planet_Explored_Flags_() {
        Reset_Explored_Planets_();

        for (int16_t i = 0; i < (int16_t)MOX::_planet_count; ++i) {
            // Check if the current player can view the star system containing this planet
            if (SYS::Ok_To_View_System_(MOX::_planet[i].star_index, MOX::_PLAYER_NUM)) {
                struct_::Set_Bit_Field_(PLNTSUM::_explored_bitmap, i);
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
        _screen->buttons[0] = fields::Add_Multi_Button_Field_(441, 200, "", _screen->pics[1], &_sort_choice, 0, "", 41);
        _screen->buttons[1] = fields::Add_Multi_Button_Field_(501, 200, "", _screen->pics[2], &_sort_choice, 1, "", 41);
        _screen->buttons[2] = fields::Add_Multi_Button_Field_(567, 200, "", _screen->pics[3], &_sort_choice, 2, "", 41);

        _screen->buttons[3] = fields::Add_Radio_Button_Field_(441, 266, "", _screen->pics[4], &_filter_out_enemy_controlled_planets, "1", 41);
        _screen->buttons[4] = fields::Add_Radio_Button_Field_(441, 289, "", _screen->pics[5], &_filter_out_adverse_gravity_planets, "2", 41);
        _screen->buttons[5] = fields::Add_Radio_Button_Field_(441, 312, "", _screen->pics[6], &_filter_out_hostile_environment_planets, "3", 41);
        _screen->buttons[6] = fields::Add_Radio_Button_Field_(441, 335, "", _screen->pics[7], &_filter_out_mineral_scarcity_planets, "4", 41);
        _screen->buttons[7] = fields::Add_Radio_Button_Field_(441, 358, "", _screen->pics[8], &_filter_out_planets_out_of_range, "5", 41);

        if (Send_Colony_Button_Selectable_()) {
            _screen->buttons[8] = fields::Add_Hidden_Field_(386, 408, 609, 413, "C", 41);
        } else {
            _screen->buttons[8] = -1000;
        }

        if (Send_Outpost_Button_Selectable_()) {
            _screen->buttons[9] = fields::Add_Hidden_Field_(413, 437, 610, 442, "O", 41);
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
                // Preserve the original byte store from the field handle into _global_var.
                memcpy(&MOX::_global_var, &_screen->colony_icons[i], sizeof(MOX::_global_var));
            }
            if (_planet_summary[row].outpost_eta >= 0) {
                Add_Outpost_Ship_Icon_Field_(&_screen->output_icons[i], i);
            }
        }

        for (int i = 0; i < 8; ++i) {
            int16_t y1 = _plntscrn_delta_y[i] + 36;
            _screen->fields[i] = fields::Add_Hidden_Field_(18, y1, 414, y1 + 50, "", 41);
        }

        _screen->up_button = fields::Add_Button_Field_(422, 16, "", _screen->pics[9], "-", 40);
        _screen->dn_button = fields::Add_Button_Field_(421, 448, "", _screen->pics[10], "+", 40);

        Add_PlntSum_Galaxy_Map_Fields_(_screen->planet_icons);
        Add_Planet_Summary_Bar_Field_();

        _screen->esc_button = fields::Add_Button_Field_(
            454,
            440,
            "",
            _screen->pics[11],
            /* Preserves the raw 0x1B control byte token used by the original field wiring. */
            "\x1B",
            40
        );

        MOX::_scroll_bar->up_btn = _screen->up_button;
        MOX::_scroll_bar->dn_btn = _screen->dn_button;
    }

    void __cdecl Allocate_Space_For_Summary_Screen_() {
        _planet_summary = (s_plntsum_planet*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*_planet_summary) * 360);
        memset(_planet_summary, 0xFF, sizeof(*_planet_summary) * 360);

        _screen = (s_plntsum_screen*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*_screen));
        memset(_screen, 0, sizeof(*_screen));

        _ship_summary = (s_plntsum_ship*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*_ship_summary) * 500);
        memset(_ship_summary, 0xFF, sizeof(*_ship_summary) * 500);

        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*MOX::_scroll_bar));
        memset(MOX::_scroll_bar, 0, sizeof(*MOX::_scroll_bar));
    }

    void __cdecl Load_Planet_Summary_Pictures_(void) {
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        _screen->pics[0] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 0, MOX::_screen_seg);
        _screen->pics[1] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 1, MOX::_screen_seg);
        _screen->pics[2] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 2, MOX::_screen_seg);
        _screen->pics[3] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 3, MOX::_screen_seg);

        RUSS::Mox_Update_();

        _screen->pics[4] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 4, MOX::_screen_seg);
        _screen->pics[5] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 5, MOX::_screen_seg);
        _screen->pics[6] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 6, MOX::_screen_seg);
        _screen->pics[7] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 7, MOX::_screen_seg);

        RUSS::Mox_Update_();

        _screen->pics[8] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 8, MOX::_screen_seg);

        _send_colony_ship_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 9, MOX::_screen_seg);
        _send_outpost_ship_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 10, MOX::_screen_seg);

        _screen->pics[9] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 11, MOX::_screen_seg);
        _screen->pics[10] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 12, MOX::_screen_seg);
        _screen->pics[12] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 13, MOX::_screen_seg);

        RUSS::Mox_Update_();

        _screen->pics[11] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 14, MOX::_screen_seg);

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        for (int i = 0; i < 11; ++i) {
            MOX::_planet_summary_galaxy_star_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_PLNTSUM_LBX_00556AC0, 15 + i, MOX::_screen_seg);
            RUSS::Mox_Update_();
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
            if (Send_Outpost_Button_Selectable_()) {
                if (Send_Colony_Button_Selectable_()) {
                    _draw_colony_button_status = 2;
                    _send_choice = 0;

                    if (!Send_Outpost_Button_Selectable_()) {
                        return;
                    }

                    _draw_outpost_button_status = 1;
                    return;
                }
            }

            if (Send_Colony_Button_Selectable_()) {
                if (!Send_Outpost_Button_Selectable_()) {
                    _draw_colony_button_status = 2;
                    _send_choice = 0;
                    return;
                }
            }

            if (!Send_Colony_Button_Selectable_() && Send_Outpost_Button_Selectable_()) {
                _draw_outpost_button_status = 2;
                _send_choice = 1;
            }
        }
    }

    void __cdecl Check_For_Colony_And_Outpost_Ships_() {
        bool stop_searching = false;

        if (MOX::_ship_stack_count <= 0) {
            return;
        }

        for (int16_t stack_idx = 0; stack_idx < (int16_t)MOX::_ship_stack_count; ++stack_idx) {
            if (stop_searching) {
                break;
            }

            int16_t node_idx = MOX::_ship_stack_start[stack_idx];
            bool stop_group = false;

            while (true) {
                int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                s_ship_data* ship = &MOX::_ship[ship_idx];

                bool can_order = HACCESS::Player_Can_Order_Ship_(MOX::_PLAYER_NUM, ship);

                // Colony ship check (SHIP_TYPE_COLONY = 1)
                if (ship->d.ship_type == SHIP_TYPE_COLONY) {
                    if (can_order && ship->status != 1) {
                        if (Ship_Is_Not_Colonizing_(ship)) {
                            _draw_colony_button_status = 1;
                            _found_colony_ship = 1;
                        }
                    }
                }

                // Outpost ship check (SHIP_TYPE_OUTPOST = 4)
                if (ship->d.ship_type == SHIP_TYPE_OUTPOST) {
                    if (can_order && ship->status != 1) {
                        if (Ship_Is_Not_Colonizing_(ship)) {
                            _draw_outpost_button_status = 1;
                            // The original binary sets the colony-found flag here as well,
                            // and the early-exit check depends on preserving that behavior.
                            _found_colony_ship = 1;
                        }
                    }
                }

                // Terminate if we have found a ship of interest and an external condition is met
                if (_found_colony_ship != 0 && _found_outpost_ship != 0) {
                    stop_group = true;
                    stop_searching = true;
                }

                if (stop_group) {
                    break;
                }

                node_idx = MOX::_ship_node[node_idx].next_node;
                if (node_idx == -1) {
                    stop_group = true;
                    break;
                }
            }
        }
    }

    void Build_Ship_Summary_() {
        _n_ships_in_summary = 0;
        memset(_ship_summary, 0xFF, sizeof(*_ship_summary) * 500);
        memset(_can_reach_star, 0, sizeof(_can_reach_star));

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

    void __cdecl Filter_Explored_Planets_(int16_t* summary_count) {
        int16_t original_count = *summary_count;
        allocate::Push_Block_(MOX::_screen_seg);

        s_plntsum_planet* temp_buffer =
            (s_plntsum_planet*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*temp_buffer) * 360);

        memset(temp_buffer, 0xFF, sizeof(*temp_buffer) * 360);

        *summary_count = 0;

        for (int16_t i = 0; i < (int16_t)MOX::_planet_count; ++i) {
            RUSS::Mox_Update_();

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
                } else if (!struct_::Test_Bit_Field_(PLNTSUM::_explored_bitmap, i)) {
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
                if (passes_filter && PLNTSUM::_filter_out_enemy_controlled_planets != 0 && Planet_Is_Enemy_Controlled_(i, MOX::_PLAYER_NUM)) {
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
                p_sum->food_per_farmer = (int16_t)(int8_t)MOX::_planet[i].food_per_farmer;
                p_sum->minerals = (int16_t)COLCALC::_minerals_per_mine[MOX::_planet[i].mineral_class];
                p_sum->mineral_class = (int16_t)MOX::_planet[i].mineral_class;
                p_sum->climate = (int16_t)MOX::_planet[i].climate;
                p_sum->explored = i;

                (*summary_count)++;
            }
        }

        // If the count of visible planets changed, update the global summary
        if (*summary_count != original_count) {
            memcpy(PLNTSUM::_planet_summary, temp_buffer, sizeof(*temp_buffer) * 360);
            PLNTSUM::Sort_Planet_Summary_();
            MOX::_scanned_field = -1;
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    int16_t __cdecl Planet_Summary_List_Id_(int16_t planet_idx) {
        int16_t result = -1;
        int16_t total_rows = MOX::_scroll_bar->total_rows;

        if (total_rows > 0) {
            for (int16_t i = 0; i < total_rows; ++i) {
                if (result != -1) {
                    break;
                }

                if (PLNTSUM::_planet_summary[i].explored == planet_idx) {
                    result = i;
                }
            }
        }

        return result;
    }

    void __cdecl Set_Colony_Ship_To_Star_Distances_(int16_t ship_summary_idx, int16_t ship_idx) {
        bool in_stasis_1000 = false;
        bool in_stasis_500 = false;
        int16_t moving_ships[2];
        s_ship_move_info move_info;

        int16_t location = MOX::_ship[ship_idx].location;

        if (location >= 1000 && location < 1500) {
            in_stasis_1000 = true;
        } else {
            in_stasis_1000 = false;
        }

        if (location >= 500 && location < 1000) {
            in_stasis_500 = true;
        } else {
            in_stasis_500 = false;
        }

        moving_ships[0] = _ship_summary[ship_summary_idx].colony_ship_idx;
        moving_ships[1] = -1;

        for (int16_t star_idx = 0; star_idx < 72; ++star_idx) {
            int16_t summary_planet_idx = Summary_Planet_Index_(star_idx);

            if (summary_planet_idx != -1) {
                SHIPMOVE::Ships_Try_To_Move_To_(moving_ships, star_idx, &move_info);

                if (move_info.moving != 0) {
                    int16_t turns = (uint8_t)move_info.turns_left;
                    _can_reach_star[star_idx] = 1;

                    if (in_stasis_1000) {
                        turns += 1000;
                    } else if (in_stasis_500) {
                        turns += 2000;
                    }

                    if (_planet_summary[summary_planet_idx].best_colony_eta == -1 ||
                        turns < _planet_summary[summary_planet_idx].best_colony_eta) 
                    {
                        for (int16_t p = 0; p < 5; ++p) {
                            int16_t planet_idx = MOX::_star[star_idx].planet_index[p];
                            if (planet_idx != -1) {
                                int16_t summary_id = Planet_Summary_Id_(planet_idx);
                                if (summary_id != -1) {
                                    _planet_summary[summary_id].best_colony_eta = turns;
                                    _planet_summary[summary_id].best_colony_ship_idx = ship_idx;
                                    _planet_summary[summary_id].blocked = 0;
                                    _planet_summary[summary_id].ship_summary_idx = ship_summary_idx;
                                }
                            }
                        }
                    }
                } else {
                    if (move_info.blackhole_blocks != 0) {
                        if (_planet_summary[summary_planet_idx].best_colony_eta == -1) {
                            _planet_summary[summary_planet_idx].blocked = 1;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Set_Outpost_Ship_To_Star_Distances_(int16_t ship_summary_idx, int16_t ship_idx) {
        bool in_stasis_1000 = false;
        bool in_stasis_500 = false;
        int16_t moving_ships[2];
        s_ship_move_info move_info;

        int16_t location = MOX::_ship[ship_idx].location;

        if (location >= 1000 && location < 1500) {
            in_stasis_1000 = true;
        } else if (location >= 500 && location < 1000) {
            in_stasis_500 = true;
        }

        moving_ships[0] = _ship_summary[ship_summary_idx].outpost_ship_idx;
        moving_ships[1] = -1;

        for (int16_t star_idx = 0; star_idx < 72; ++star_idx) {
            int16_t summary_planet_idx = Summary_Planet_Index_(star_idx);

            if (summary_planet_idx != -1) {
                SHIPMOVE::Ships_Try_To_Move_To_(moving_ships, star_idx, &move_info);

                if (move_info.moving != 0) {
                    _can_reach_star[star_idx] = 1;
                    int16_t turns = (uint8_t)move_info.turns_left;

                    if (in_stasis_1000) {
                        turns += 1000;
                    } else if (in_stasis_500) {
                        turns += 2000;
                    }

                    if (_planet_summary[summary_planet_idx].best_outpost_eta == -1 ||
                        turns < _planet_summary[summary_planet_idx].best_outpost_eta) 
                    {
                        for (int16_t p = 0; p < 5; ++p) {
                            int16_t planet_idx = MOX::_star[star_idx].planet_index[p];
                            if (planet_idx != -1) {
                                int16_t summary_id = Planet_Summary_Id_(planet_idx);
                                if (summary_id != -1) {
                                    _planet_summary[summary_id].best_outpost_eta = turns;
                                    _planet_summary[summary_id].best_outpost_ship_idx = ship_idx;
                                    _planet_summary[summary_id].blocked = 0;
                                    _planet_summary[summary_id].ship_summary_idx = ship_summary_idx;
                                }
                            }
                        }
                    }
                } else {
                    if (move_info.blackhole_blocks != 0 && _planet_summary[summary_planet_idx].best_outpost_eta == -1) {
                        _planet_summary[summary_planet_idx].blocked = 1;
                    }
                }
            }
        }
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

    int __cdecl qsort_by_climate_(const void* a, const void* b) {
        const s_plntsum_planet* lhs = (const s_plntsum_planet*)a;
        const s_plntsum_planet* rhs = (const s_plntsum_planet*)b;
        return (int)rhs->climate - (int)lhs->climate;
    }

    int __cdecl qsort_by_minerals_(const void* a, const void* b) {
        const s_plntsum_planet* lhs = (const s_plntsum_planet*)a;
        const s_plntsum_planet* rhs = (const s_plntsum_planet*)b;
        return (int)rhs->minerals - (int)lhs->minerals;
    }

    int __cdecl qsort_by_max_population_(const void* a, const void* b) {
        const s_plntsum_planet* lhs = (const s_plntsum_planet*)a;
        const s_plntsum_planet* rhs = (const s_plntsum_planet*)b;
        return (int)rhs->max_pop - (int)lhs->max_pop;
    }

    void __cdecl Sort_Planet_Summary_() {
        switch (_sort_choice) {
            case 0:
                qsort(_planet_summary, (uint32_t)MOX::_scroll_bar->total_rows, sizeof(*_planet_summary), qsort_by_climate_);
                break;
            case 1:
                qsort(_planet_summary, (uint32_t)MOX::_scroll_bar->total_rows, sizeof(*_planet_summary), qsort_by_minerals_);
                break;
            case 2:
                qsort(_planet_summary, (uint32_t)MOX::_scroll_bar->total_rows, sizeof(*_planet_summary), qsort_by_max_population_);
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
                fields_array[j] = fields::Add_Hidden_Field_(x, y, x + 5, y + 5, "", 41);
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
                                target_id = PLNTSUM::_planet_summary[current_row].best_colony_ship_idx;
                            } else if (PLNTSUM::_send_choice == 1) {
                                target_id = PLNTSUM::_planet_summary[current_row].best_outpost_ship_idx;
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

    void __cdecl Load_Player_Icon_() {
        int entry_num = (int)(short)((uint8_t)MOX::_player[MOX::_PLAYER_NUM].color + 0x4C);

        s_animation_header* player_icon = (s_animation_header*)farload::Far_Reload_Next_(
            PLNTSUM::PTR_s_PLNTSUM_LBX_00556AC0,
            entry_num,
            MOX::_screen_seg
        );

        PLNTSUM::_screen->pics[13] = player_icon;
    }

    void __cdecl Wrapper_For_Planet_Summary_Galaxy_Map_() {
        short star_x, star_y;
        uint32_t color = 0xE2;

        graphics::Set_Window_(443, 17, 623, 133);
        video::Clip_On_();

        if (MOX::_scanned_field != -1) {
            short star_idx = MOX::_scanned_field;
            if (star_idx < 1000) {
                Draw_Nearest_Ship_Directional_Line_(star_idx);
            } else if (star_idx < MOX::_NUM_STARS + 1000) {
                Draw_Nearest_Ship_Directional_Line_(star_idx - 1000);
            }

            if (star_idx >= 1000 && star_idx < MOX::_NUM_STARS + 1000) {
                star_idx -= 1000;
            }

            MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx, 443, 17, 180, 116, 0, 0, 2, &star_x, &star_y);

            int8_t owner = MOX::_star[star_idx].owner;
            if (owner > -1) {
                color = (uint32_t)(MOX::_player[owner].color * 6 + 212);
            }

            graphics::Box_(star_x - 1, star_y - 1, 9, 9, 1, (uint8_t)color);
        }

        for (int16_t i = 0; i < PLNTSUM::_n_ships_in_summary; ++i) {
            bool found_colony = Summary_Found_Colony_Ship_(i);
            bool found_outpost = Summary_Found_Outpost_Ship_(i);

            if (found_colony || found_outpost) {
                short marker_x, marker_y;
                Get_Colony_Outpost_Marker_XY_(i, &marker_x, &marker_y);
                Print_Colony_Outpost_Marker_(marker_x, marker_y, i, found_colony, found_outpost);
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();

        MOVEBOX::Draw_Galaxy_Map_Box_(nullptr, nullptr, 443, 17, 180, 116, 0, 0, 0, 0, 2, -1);

        PLNTSUM::_planet_summary->scan_highlight_pulse += (short)(PLNTSUM::_planet_summary->scan_highlight_pulse_step * 2);

        int8_t step = PLNTSUM::_planet_summary->scan_highlight_pulse_step;
        if (step > 0) {
            if (PLNTSUM::_planet_summary->scan_highlight_pulse >= 3) {
                PLNTSUM::_planet_summary->scan_highlight_pulse_step = -step;
            }
        } else if (step < 0) {
            if (PLNTSUM::_planet_summary->scan_highlight_pulse <= 0) {
                PLNTSUM::_planet_summary->scan_highlight_pulse_step = -step;
            }
        }
    }

    int16_t __cdecl Nearest_Ship_Id_(int16_t index) {
        if (PLNTSUM::_send_choice == 0) {
            return PLNTSUM::_planet_summary[index].best_colony_ship_idx;
        }

        if (PLNTSUM::_send_choice == 1) {
            return PLNTSUM::_planet_summary[index].best_outpost_ship_idx;
        }

        return -1;
    }

void __cdecl Draw_Nearest_Ship_Directional_Line_(int16_t star_idx) {
    int16_t summary_idx;
    s_colors colors;
    uint8_t colony_selectable;
    uint8_t outpost_selectable;
    bool colony_active = false;
    bool outpost_active = false;
    int16_t ship_idx;
    int16_t marker_x, marker_y;
    int16_t star_x, star_y;
    int16_t dist;

    summary_idx = Summary_Planet_Index_(star_idx);

    colors.colors[0] = 0xE2;
    colors.colors[1] = 0xE3;
    colors.colors[2] = 0xE4;
    colors.colors[3] = 0xE5;
    colors.colors[4] = 0xE4;
    colors.colors[5] = 0xE3;
    colors.colors[6] = 0xE3;
    colors.colors[7] = 0xE2;

    colony_selectable = (uint8_t)Send_Colony_Button_Selectable_();
    outpost_selectable = (uint8_t)Send_Outpost_Button_Selectable_();

    if (summary_idx == -1) {
        return;
    }

    if (!colony_selectable && !outpost_selectable) {
        return;
    }

    if (PLNTSUM::_draw_colony_button_status == 1) {
        colony_active = true;
    } else if (PLNTSUM::_draw_outpost_button_status == 1) {
        outpost_active = true;
    } else if (!colony_selectable && outpost_selectable) {
        outpost_active = true;
    } else {
        colony_active = true;
    }

    if (colony_active) {
        ship_idx = PLNTSUM::_planet_summary[summary_idx].best_colony_ship_idx;
    } else {
        ship_idx = PLNTSUM::_planet_summary[summary_idx].best_outpost_ship_idx;
    }

    if (ship_idx == -1) {
        return;
    }

    bool ship_status_is_one = (MOX::_ship[ship_idx].status == 1);

    Get_Colony_Outpost_Marker_XY_(PLNTSUM::_planet_summary[summary_idx].ship_summary_idx, &marker_x, &marker_y);

    if (colony_active && ship_idx > -1 && PLNTSUM::_can_reach_star[star_idx] != 0 && !ship_status_is_one) {
        MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx, 0x1BB, 0x11, 0xB4, 0x74, 0, 0, 0, &star_x, &star_y);
        dist = special::Range_(marker_x, marker_y, star_x + 4, star_y + 3);
        if (dist > 10) {
            SHIPS::Draw_Directional_Multi_Colored_Line_(marker_x + 7, marker_y + 2, star_x + 2, star_y + 2, &colors, MOX::_multi_colored_line_start);
            
            int32_t next_idx = (int32_t)MOX::_multi_colored_line_start + 1;
            int32_t abs_val = (next_idx >= 0) ? next_idx : -next_idx;
            int32_t result = abs_val % 8;
            MOX::_multi_colored_line_start = (int16_t)((next_idx >= 0) ? result : -result);
        }
    }

    if (outpost_active && ship_idx > -1 && PLNTSUM::_can_reach_star[star_idx] != 0 && !ship_status_is_one) {
        MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx, 0x1BB, 0x11, 0xB4, 0x74, 0, 0, 0, &star_x, &star_y);
        dist = special::Range_(marker_x, marker_y, star_x + 4, star_y + 3);
        if (dist > 10) {
            SHIPS::Draw_Directional_Multi_Colored_Line_(marker_x + 8, marker_y + 2, star_x + 2, star_y + 2, &colors, MOX::_multi_colored_line_start);
            
            int32_t next_idx = (int32_t)MOX::_multi_colored_line_start + 1;
            int32_t abs_val = (next_idx >= 0) ? next_idx : -next_idx;
            int32_t result = abs_val % 8;
            MOX::_multi_colored_line_start = (int16_t)((next_idx >= 0) ? result : -result);
        }
    }
}

void __cdecl Undo_Send_Order_(int16_t planet_idx, int16_t order_type) {
    int16_t ship_idx = -1;
    int16_t ships[2];
    bool found = false;
    char buffer1[200];
    char buffer2[200];
    char eta_str[40];

    ships[0] = -1;
    ships[1] = (int16_t)0xffff;

    for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
        if (found) break;

        if (PLNTSUM::_planet_summary[i].explored == planet_idx) {
            if (order_type == 1) {
                ship_idx = PLNTSUM::_planet_summary[i].best_colony_ship_idx;
            } else {
                ship_idx = PLNTSUM::_planet_summary[i].best_outpost_ship_idx;
            }

            if (!MAINSCR::Ship_Can_Be_Selected_(ship_idx)) {
                int16_t msg_id = (order_type == 1) ? 0x149 /* HESTR_149_YOU_ARE_UNABLE_TO_UNDO */ : 0x14A /* HESTR_14A_YOU_ARE_UNABLE_TO_UNDO */;
                const char* msg = HAROLD::H_Message_(msg_id);
                strcpy(buffer1, msg);
                char* planet_name = HACCESS::Get_Planet_Name_No_Prime_(planet_idx);
                sprintf(buffer2, buffer1, planet_name);
                GENDRAW::Warning_Box_(buffer2);
            }
            found = true;
        }
    }

    if (ship_idx != -1) {
        int16_t star_id = HAROLD::XY_To_Star_Id_(MOX::_ship[ship_idx].x, MOX::_ship[ship_idx].y);
        
        if (star_id == -1) {
            ships[0] = ship_idx;
            star_id = HACCESS::Nearest_Star_To_XY_(ships);
            
            if (star_id == -1) {
                star_id = MOX::_planet[MOX::_player[MOX::_PLAYER_NUM].home_planet_id].star_index;
            }
        }

        if (star_id == -1) {
            return;
        }

        ships[0] = ship_idx;
        SHIPMOVE::Ships_Try_To_Move_To_(ships, star_id, &MOX::_g_ship_move_info);

        int16_t eta_msg_id = (MOX::_g_ship_move_info.turns_left == 1) ? 0x14B /* HESTR_14B_ETA_D_TURN */ : 0x14C /* HESTR_14C_ETA_D_TURNS */;
        const char* eta_raw = HAROLD::H_Message_(eta_msg_id);
        sprintf(eta_str, eta_raw, (uint32_t)(uint8_t)MOX::_g_ship_move_info.turns_left);

        int16_t heading_msg_id = (order_type == 1) ? 0x14D /* HESTR_14D_COLONY_SHIP_HEADING_TO */ : 0x14E /* HESTR_14E_OUTPOST_SHIP_HEADING_T */;
        const char* heading_raw = HAROLD::H_Message_(heading_msg_id);
        strcpy(buffer1, heading_raw);

        const char* eta_param = (MOX::_g_ship_move_info.turns_left == 0) ? "" : eta_str;
        char* planet_name = HACCESS::Get_Planet_Name_No_Prime_(planet_idx);

        sprintf(buffer2, buffer1, planet_name, &MOX::_star[star_id], eta_param);

        if (GENDRAW::Do_Confirmation_Box_(buffer2)) {
            SHIPMOVE::Make_Ships_Move_To_(ships, star_id, &MOX::_g_ship_move_info);
            PLNTSUM::Update_Ship_Etas_();
            SHIPS::Clear_Auto_Colonization_Flag_(&MOX::_ship[ship_idx]);
        }
        PLNTSUM::_screen->refresh_fields = true;
    }
}

void __cdecl Print_Colony_Outpost_Marker_(int16_t x, int16_t y, int16_t ship_summary_idx, int8_t colony, int8_t outpost) {
    (void)ship_summary_idx;
    s_colors inactive_colors;
    s_colors active_colors;

    inactive_colors.colors[0] = 0;
    inactive_colors.colors[1] = ':';
    inactive_colors.colors[2] = ':';
    inactive_colors.colors[3] = 'C';
    inactive_colors.colors[4] = 'B';
    inactive_colors.colors[5] = 'A';
    inactive_colors.colors[6] = '?';
    inactive_colors.colors[7] = '?';
    active_colors.colors[0] = 0;

    for (int16_t i = 1; i < 8; ++i) {
        int16_t color_idx = PLNTSUM::_planet_summary->scan_highlight_pulse + i;
        if (color_idx < 1) {
            color_idx = 1;
        }
        if (color_idx > 7) {
            color_idx = 7;
        }
        active_colors.colors[i] = inactive_colors.colors[color_idx];
    }

    if (outpost != 0 && colony != 0) {
        int16_t cur_x = x + 3;
        int16_t cur_y = y - 1;

        if (MOX::_scanned_field > -1 && MOX::_scanned_field < 1000) {
            if (PLNTSUM::_send_choice == 1) {
                fonts::Set_Font_Style_(0, &active_colors);
                int16_t bottom = fonts::Get_Font_Height_() + cur_y;
                int16_t right = fonts::Get_String_Width_("oc") + cur_x;
                graphics::Fill_(cur_x, cur_y, right, bottom, 1);
                fonts::Print_(cur_x, cur_y, "o");
                fonts::Set_Font_Style_(0, &inactive_colors);
                int16_t text_x = fonts::Get_String_Width_("o") + cur_x;
                fonts::Print_(text_x, cur_y, "c");
                return;
            }

            if (PLNTSUM::_send_choice == 0) {
                fonts::Set_Font_Style_(0, &inactive_colors);
                int16_t bottom = fonts::Get_Font_Height_() + cur_y;
                int16_t right = fonts::Get_String_Width_("oc") + cur_x;
                graphics::Fill_(cur_x, cur_y, right, bottom, 1);
                fonts::Print_(cur_x, cur_y, "o");
                fonts::Set_Font_Style_(0, &active_colors);
                int16_t text_x = fonts::Get_String_Width_("o") + cur_x;
                fonts::Print_(text_x, cur_y, "c");
                return;
            }
        }

        fonts::Set_Font_Style_(0, &inactive_colors);
        fonts::Print_(cur_x, cur_y, "oc");
    } else if (outpost != 0) {
        int16_t cur_x = x + 6;
        int16_t cur_y = y - 1;

        if (MOX::_scanned_field > -1 && MOX::_scanned_field < 1000) {
            fonts::Set_Font_Style_(0, &active_colors);
            int16_t bottom = fonts::Get_Font_Height_() + cur_y;
            int16_t right = fonts::Get_String_Width_("o") + cur_x;
            graphics::Fill_(cur_x, cur_y, right, bottom, 1);
        } else {
            fonts::Set_Font_Style_(0, &inactive_colors);
        }

        fonts::Print_(cur_x, cur_y, "o");
    } else if (colony != 0) {
        int16_t cur_x = x + 6;
        int16_t cur_y = y - 1;

        if (MOX::_scanned_field > -1 && MOX::_scanned_field < 1000) {
            fonts::Set_Font_Style_(0, &active_colors);
            int16_t bottom = fonts::Get_Font_Height_() + cur_y;
            int16_t right = fonts::Get_String_Width_("c") + cur_x;
            graphics::Fill_(cur_x, cur_y, right, bottom, 1);
        } else {
            fonts::Set_Font_Style_(0, &inactive_colors);
        }

        fonts::Print_(cur_x, cur_y, "c");
    }
}

void __cdecl Get_Colony_Outpost_Marker_XY_(int16_t summary_idx, int16_t* out_x, int16_t* out_y) {
    s_plntsum_ship* summary = &PLNTSUM::_ship_summary[summary_idx];
    int16_t ship_idx = summary->outpost_ship_idx;

    if (ship_idx <= summary->colony_ship_idx) {
        ship_idx = summary->colony_ship_idx;
    }

    s_ship_data* ship = &MOX::_ship[ship_idx];
    int16_t location = ship->location;

    bool is_space_location = (location >= 500 && location < 1000);
    bool is_travel_location = (location >= 1000 && location < 1500);

    if (is_space_location) {
        MOVEBOX::Get_Galaxy_Map_Space_XY_(summary->ship_icon_id, 443, 17, 180, 116, out_x, out_y);
        *out_x += 2;
        return;
    }

    int16_t star_idx = location;
    if (is_travel_location) {
        star_idx = HAROLD::XY_To_Star_Id_(ship->x, ship->y);
    }

    MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx, 443, 17, 180, 116, 0, 0, 2, out_x, out_y);
    *out_x += 2;
}

void Draw_Rotating_Planets_() {
    int16_t first_row = MOX::_scroll_bar->first_visible_row;
    int16_t total_rows = MOX::_scroll_bar->total_rows;
    int32_t upper_bound = first_row + total_rows;

    // Draw at most 8 rows starting from the scrollbar position
    if (first_row + 8 < upper_bound) {
        upper_bound = first_row + 8;
    }

    int32_t count = (int16_t)upper_bound - first_row;
    if (count > 0) {
        for (int16_t i = 0; i < (int16_t)count; ++i) {
            int16_t explored_planet_idx = PLNTSUM::_planet_summary[first_row + i].explored;

            if (explored_planet_idx != -1) {
                s_planet_data* planet = &MOX::_planet[explored_planet_idx];

                // Planet animation index is determined by climate type and planet size
                int16_t anim_idx = (int16_t)planet->climate * 5 + (int16_t)planet->size;
                s_animation_header* anim = PLNTSUM::_p_rotating_planet[anim_idx];

                int16_t width = animate::Get_Width_(anim);
                int16_t height = animate::Get_Height_(anim);

                // Center the planet image within the designated list cell area (86x25)
                int16_t x = (86 /* 0x56 */ - width) / 2 + 18 /* 0x12 */;
                int16_t y = PLNTSUM::_plntscrn_delta_y[i] + (25 /* 0x19 */ - height) / 2 + 45 /* 0x2d */;

                animate::Draw_(x, y, anim);
            }
        }
    }
}

void __cdecl Fill_Summary_Screen_Scroll_Bar_(void) {
    int16_t y_base;
    uint8_t color = 150 /* 0x96 */;
    int16_t x = 423 /* 0x1a7 */;

    graphics::Set_Window_(421 /* 0x1a5 */, 37 /* 0x25 */, 431 /* 0x1af */, 444 /* 0x1bc */);
    video::Clip_On_();

    // Calculate visual offset for the scroll thumb based on scrollbar position
    y_base = MOX::_scroll_bar->track_len_px - (MOX::_scroll_bar->thumb_height_px / 2) - MOX::_scroll_bar->thumb_center_y;

    for (int16_t i = 0; i < 10; ++i) {
        int16_t y1 = y_base + 38 /* 0x26 */;
        int16_t y2 = MOX::_scroll_bar->thumb_height_px + y_base + 37 /* 0x25 */;
        
        line::Line_(x, y1, x, y2, color);

        // Assembly implementation of parity check: alternates color decrement
        if ((i % 2) == 0) {
            color--;
        }
        x++;
    }

    video::Clip_Off_();
    graphics::Reset_Window_();

    // Redraw the scroll background frame or overlay
    animate::Draw_(421 /* 0x1a5 */, 37 /* 0x25 */, PLNTSUM::_screen->pics[12]);

    PLNTSUM::_recalc_first_planet = false;
}

void __cdecl Load_Planet_Pictures_() {
    for (int16_t i = 0; i < 50; ++i) {
        s_animation_header* segment = Load_Planet_Segment_(i);
        _p_rotating_planet[i] = segment;
        RUSS::Mox_Update_();
    }
}

void __cdecl Draw_Mini_Planet_Summary_Screen_() {
    if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
        video::Set_Page_Back_();
        graphics::Fill_(0, 0, 639, 479, 0);
        animate::Draw_(0, 0, PLNTSUM::_screen->pics[0]);
        video::Copy_Back_To_Off_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        video::Set_Page_Off_();
    }

    graphics::Fill_(0, 0, 639, 479, 0);
    animate::Draw_(0, 0, PLNTSUM::_screen->pics[0]);
    graphics::Reset_Window_();
    video::Clip_Off_();

    int16_t input_id = fields::Auto_Input_();
    if ((int16_t)PLNTSUM::_plntsum_scroll_bar == input_id) {
        PLNTSUM::_recalc_first_planet = true;
    }

    PLNTSUM::Draw_Rotating_Planets_();
    PLNTSUM::Wrapper_For_Planet_Summary_Galaxy_Map_();
    SCROLL::Draw_Generic_Vertical_Scroll_Bar_(MOX::_scroll_bar, PLNTSUM::Fill_Summary_Screen_Scroll_Bar_);
    PLNTSUM::Summary_Screen_Print_Data_();

    if (MOX::_scanned_field > -1) {
        PLNTSUM::Summary_Screen_Print_Scanned_();
    }

    for (int16_t i = 0; i < (int16_t)PLNTSUM::_n_ships_in_summary; ++i) {
        if (PLNTSUM::_fade_flag != 0) {
            break;
        }

        int16_t ship_icon_id = PLNTSUM::_ship_summary[i].ship_icon_id;
        int16_t node_idx = MOX::_ship_icon[ship_icon_id].node_idx;
        int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
        s_ship_data* ship_ptr = &MOX::_ship[ship_idx];

        if (ship_ptr->location >= 500) {
            uint8_t type = ship_ptr->d.ship_type;
            if (type == 1 || type == 4) {
                PLNTSUM::Draw_Nearest_Ship_Directional_Line_(ship_ptr->location);
            }
        }
    }

    animate::Set_Animation_Frame_(PLNTSUM::_send_colony_ship_button_seg, (int)PLNTSUM::_draw_colony_button_status);
    animate::Draw_(454, 386, PLNTSUM::_send_colony_ship_button_seg);

    animate::Set_Animation_Frame_(PLNTSUM::_send_outpost_ship_button_seg, (int)PLNTSUM::_draw_outpost_button_status);
    animate::Draw_(454, 413, PLNTSUM::_send_outpost_ship_button_seg);

    PLNTSUM::_skip_draw_planets = 0;
}

void __cdecl Summary_Screen_Print_Scanned_() {
    char msg_buffer[200];
    char suffix_buffer[100];
    char star_name_buffer[100];
    int8_t status = 0; // 0: neutral, 1: red, 2: green
    int16_t scanned_field = MOX::_scanned_field;
    int16_t planet_idx = Summary_Planet_Index_(scanned_field);

    if (PLNTSUM::_planet_summary[planet_idx].blocked == 1) {
        sprintf(msg_buffer, HAROLD::H_Message_(324 /* HESTR_144_BLACK_HOLE_BLOCKS_TRAV */), MOX::_star[scanned_field].name);
        fonts::Set_Font_Style_(1, &PLNTSUM::_red_colors);
        int16_t y = (fonts::Get_Font_Height_() / 2) + 143 /* 0x8F */;
        fonts::Print_Centered_(533 /* 0x215 */, y, msg_buffer);
        return;
    }

    if (scanned_field < 1000 || scanned_field >= (MOX::_NUM_STARS + 1000)) {
        if (PLNTSUM::_send_choice != 0 && PLNTSUM::_send_choice != 1) {
            return;
        }

        bool visited = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, scanned_field) || HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);

        if (MOX::_g_ship_move_info.moving != 0) {
            status = 2;
            if (visited) {
                if (MOX::_g_ship_move_info.turns_left == 1) {
                    sprintf(msg_buffer, HAROLD::H_Message_(105 /* HESTR_069_D_TURN_TO_S */), (int)(uint8_t)MOX::_g_ship_move_info.turns_left, MOX::_star[scanned_field].name);
                } else {
                    sprintf(msg_buffer, HAROLD::H_Message_(106 /* HESTR_06A_D_TURNS_TO_S */), (int)(uint8_t)MOX::_g_ship_move_info.turns_left, MOX::_star[scanned_field].name);
                }
            } else {
                sprintf(msg_buffer, HAROLD::H_Message_(332 /* HESTR_14C_ETA_D_TURNS */), (int)(uint8_t)MOX::_g_ship_move_info.turns_left);
            }
        } else {
            status = 1;
            strcpy(msg_buffer, HAROLD::H_Message_(328 /* HESTR_148_OUT_OF_RANGE */));
        }
    } else {
        int16_t star_id = scanned_field - 1000;
        bool visited = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_id) || HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);

        if (MOX::_star[star_id].spectral_class == 6 /* Black Hole */) {
            strcpy(msg_buffer, HAROLD::H_Message_(325 /* HESTR_145_BLACK_HOLE */));
            status = 1;
        } else if (!visited) {
            if (PLNTSUM::_send_choice == 0 || PLNTSUM::_send_choice == 1) {
                if (PLNTSUM::_can_reach_star[star_id] != 0) {
                    int16_t eta = Get_ETA_To_Star_(star_id);
                    if (eta > 0) {
                        if (eta == 1) {
                            sprintf(msg_buffer, HAROLD::H_Message_(331 /* HESTR_14B_ETA_D_TURN */), (int)eta);
                        } else {
                            sprintf(msg_buffer, HAROLD::H_Message_(332 /* HESTR_14C_ETA_D_TURNS */), (int)eta);
                        }
                    }
                    status = 2;
                } else {
                    status = 1;
                    strcpy(msg_buffer, HAROLD::H_Message_(327 /* HESTR_147_STAR_SYSTEM_UNREACHABL */));
                }
            } else {
                strcpy(msg_buffer, HAROLD::H_Message_(148 /* HESTR_094_UNEXPLORED */));
                status = 0;
            }
        } else {
            strcpy(star_name_buffer, MOX::_star[star_id].name);
            suffix_buffer[0] = '\0';
            if (PLNTSUM::_send_choice == 0 || PLNTSUM::_send_choice == 1) {
                if (PLNTSUM::_can_reach_star[star_id] == 0) {
                    strcpy(suffix_buffer, HAROLD::H_Message_(326 /* HESTR_146_CANNOT_REACH */));
                    status = 1;
                } else {
                    int16_t eta = Get_ETA_To_Star_(star_id);
                    if (eta > 0) {
                        if (eta == 1) {
                            sprintf(suffix_buffer, HAROLD::H_Message_(336 /* HESTR_150_D_TURN_TO */), (int)eta);
                        } else {
                            sprintf(suffix_buffer, HAROLD::H_Message_(337 /* HESTR_151_D_TURNS_TO */), (int)eta);
                        }
                    }
                    status = 2;
                }
            }
            sprintf(msg_buffer, "%s%s", suffix_buffer, star_name_buffer);
        }
    }

    s_colors* p_colors;
    if (status == 1) {
        p_colors = &PLNTSUM::_red_colors;
    } else if (status == 2) {
        p_colors = &PLNTSUM::_green_colors;
    } else {
        p_colors = &PLNTSUM::_hi_neutral_colors;
    }

    fonts::Set_Font_Style_(2, p_colors);
    int16_t y = ((fonts::Get_Font_Height_() + 2) / 2) + 143 /* 0x8F */;
    fonts::Print_Centered_(533 /* 0x215 */, y, msg_buffer);
}

void Planet_Summary_Screen_() {
    int16_t exit_flag = 0;
    int16_t last_scanned_status = -1;
    char warning_msg[100];

    MAINSCR::Allocate_First_Screen_Seg_(8);
    _fade_flag = 1;
    fields::Set_Field_Outline_Color_(215 /* 0xd7 */);
    EVANHELP::Set_PlntSum_Screen_Help_List_();
    _n_ships_in_summary = 0;
    _send_choice = -1;
    COLONY::_using_colony_screen_palette = 0;
    MOX::_scanned_field = -1;
    DAT_00565840 = 0;
    last_scanned_status = -1;
    _recalc_first_planet = false;
    MOX::_multi_colored_line_start = 0;
    MOX::_galaxy_star_anim_count = 0;
    _last_ship_moved = -1;
    DAT_0056583c = 0;
    DAT_005658a4 = 0;
    _skip_draw_planets = 0;
    _can_reach_scanned_planet = false;
    _draw_colony_button_status = 0;
    DESBOX::_incr_value = 1;
    _draw_outpost_button_status = 0;

    video::Set_Refresh_Full_();
    Check_For_Colony_And_Outpost_Ships_();

    if (_found_colony_ship != 0) {
        _draw_colony_button_status = 1;
    }
    if (_found_outpost_ship != 0) {
        _draw_outpost_button_status = 1;
    }

    fields::Assign_Auto_Function_(Draw_Mini_Planet_Summary_Screen_, 1);
    fields::Clear_Fields_();
    MISC::Fade_To_Black_();
    fonts::Load_Palette_(6, 0, 254 /* 0xfe */);

    Allocate_Space_For_Summary_Screen_();
    Load_Planet_Summary_Pictures_();
    Add_Plntsum_Fields_();

    _planet_summary->scan_highlight_pulse = 0;
    _planet_summary->scan_highlight_pulse_step = 1;

    Set_Planet_Explored_Flags_();
    Filter_Explored_Planets_(&MOX::_scroll_bar->total_rows);

    MOX::_scroll_bar->thumb_center_y = 407 /* 0x197 */;
    MOX::_scroll_bar->visible_rows = 8;
    MOX::_scroll_bar->track_len_px = 407 /* 0x197 */;
    MOX::_scroll_bar->thumb_height_px = SCROLL::Get_Vertical_Scroll_Bar_Height_(MOX::_scroll_bar);
    MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px - (MOX::_scroll_bar->thumb_height_px / 2);
    MOX::_scroll_bar->last_thumb_center_y = -1;
    MOX::_scroll_bar->type = 2;
    MOX::_scroll_bar->row_step = 1;

    _p_rotating_planet = (s_animation_header**)allocate::Allocate_Next_Block_(
        MOX::_screen_seg,
        sizeof(*_p_rotating_planet) * 50
    );
    Load_Planet_Pictures_();
    Determine_Button_Status_();

    do {
        int16_t prev_sort_choice = _sort_choice;
        int16_t input_id = fields::Get_Input_();
        int16_t scanned_id = fields::Scan_Input_();
        _screen->scanned_input = scanned_id;
        int16_t current_scanned = _screen->scanned_input;

        if (_screen->esc_button == input_id) {
            MOX::_current_screen = 0 /* SCREEN_MAIN */;
            exit_flag = 1;
        }

        if (_sort_choice != prev_sort_choice) {
            if (input_id == _screen->buttons[0] || input_id == _screen->buttons[1] || input_id == _screen->buttons[2]) {
                Sort_Planet_Summary_();
                MOX::_scanned_field = -1;
            }
        }

        if (input_id == _screen->buttons[3] || input_id == _screen->buttons[4] || input_id == _screen->buttons[5] ||
            input_id == _screen->buttons[6] || input_id == _screen->buttons[7]) {
            Filter_Explored_Planets_(&MOX::_scroll_bar->total_rows);
            last_scanned_status = -1;
            if (MOX::_scroll_bar->total_rows < 8) {
                MOX::_scroll_bar->first_visible_row = 0;
            } else if (MOX::_scroll_bar->total_rows < MOX::_scroll_bar->first_visible_row + 8) {
                MOX::_scroll_bar->first_visible_row = MOX::_scroll_bar->total_rows - 8;
                MOX::_scroll_bar->thumb_center_y = (short)((((MOX::_scroll_bar->first_visible_row * -1000) / (int)MOX::_scroll_bar->total_rows) * 407) / 1000) + 407;
            }

            if (Send_Colony_Button_Selectable_() || Send_Outpost_Button_Selectable_()) {
                Build_Ship_Summary_();
            }
        }

        if (input_id == _screen->up_button && MOX::_scroll_bar->total_rows > 8) {
            SCROLL::Scroll_Up_One_Item_(MOX::_scroll_bar);
            last_scanned_status = -1;
        }

        if (input_id == _screen->dn_button && MOX::_scroll_bar->total_rows > 8) {
            SCROLL::Scroll_Down_One_Item_(MOX::_scroll_bar);
            last_scanned_status = -1;
        }

        if (input_id == _plntsum_scroll_bar) {
            _recalc_first_planet = true;
            last_scanned_status = -1;
        }

        uint8_t handled_star = 0;
        for (int16_t i = 0; i < MOX::_NUM_STARS && !handled_star; ++i) {
            if (_screen->planet_icons[i] == input_id && MOX::_scroll_bar->total_rows > 8) {
                int16_t row = First_Planet_In_Summary_List_(i);
                if (row <= -1) {
                    s_star_data *p_star = &MOX::_star[i];
                    const char *msg_fmt = HAROLD::H_Message_(0x14f /* HESTR_14F_NO_PLANETS_IN_THE_S_ST */);
                    sprintf(warning_msg, msg_fmt, p_star);
                    GENDRAW::Warning_Box_(warning_msg);
                } else {
                    MOX::_scroll_bar->first_visible_row = row;
                    MOX::_scroll_bar->thumb_center_y = ((-407 / (int)MOX::_scroll_bar->total_rows) - 1) * MOX::_scroll_bar->first_visible_row + 407;
                }
                handled_star = 1;
            }
            if (_screen->planet_icons[i] == current_scanned) {
                MOX::_scanned_field = i + 1000;
                handled_star = 1;
            }
        }

        Scan_Summary_Box_Fields_(current_scanned, input_id, &last_scanned_status);

        if (input_id == _screen->buttons[8] && _draw_colony_button_status != 0) {
            if (EVENTS::Event_Check_Hyperspace_Flux_() != 0) {
                const char *flux_msg = HAROLD::H_Message_(0x13f);
                GENDRAW::Warning_Box_(flux_msg);
                _draw_colony_button_status = 1;
                _send_choice = -1;
            } else {
                if (_draw_colony_button_status == 2) {
                    _draw_colony_button_status = 1;
                    _send_choice = -1;
                } else {
                    _draw_colony_button_status = 2;
                    _draw_outpost_button_status = 1;
                    _send_choice = 0;
                }
            }
        }

        if (input_id == _screen->buttons[9] && _draw_outpost_button_status != 0) {
            if (EVENTS::Event_Check_Hyperspace_Flux_() != 0) {
                const char *flux_msg = HAROLD::H_Message_(0x13f);
                GENDRAW::Warning_Box_(flux_msg);
                _draw_outpost_button_status = 1;
                _send_choice = -1;
            } else {
                if (_draw_outpost_button_status == 2) {
                    _draw_outpost_button_status = 1;
                    _send_choice = -1;
                } else {
                    _draw_outpost_button_status = 2;
                    _draw_colony_button_status = 1;
                    _send_choice = 1;
                }
            }
        }

        if (_screen->refresh_fields) {
            fields::Clear_Fields_();
            Add_Plntsum_Fields_();
            _screen->refresh_fields = false;
        }

        if (exit_flag == 0) {
            if (_fade_flag != 0) {
                video::Set_Page_Off_();
                Draw_Mini_Planet_Summary_Screen_();
                video::Copy_Off_To_Back_();
                ERIC::Fast_Fade_In_();
                _fade_flag = 0;
            } else {
                video::Set_Page_Off_();
                Draw_Mini_Planet_Summary_Screen_();
                RUSS::Mox_Sync_Update_();
            }
        }

    } while (exit_flag == 0);

    fields::Clear_Fields_();
    fields::Disable_Draw_All_Fields_();
    video::Set_Refresh_Stencil_();
    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    MISC::Fade_To_Black_();
    fields::Enable_Draw_All_Fields_();
    MOX::_scroll_bar = nullptr;
    if (MOVEBOX::Moveable_Box_Selected_(2) != 0 && _last_ship_moved != static_cast<int16_t>(0xFFFF)) {
        MOX::_first_node_ptr_in_stack_ = -1;
    }
    MAINSCR::Draw_Mini_Main_Screen_();
    fields::Set_Field_Outline_Color_(10);
}

s_animation_header* __cdecl Load_Planet_Segment_(int16_t planet_anim_idx) {
    allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

    s_animation_header* segment = (s_animation_header*)farload::Far_Reload_Next_(
        PTR_s_PLNTSUM_LBX_00556AC0,
        (int32_t)(int16_t)(planet_anim_idx + 0x1A),
        MOX::_screen_seg
    );

    allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

    return segment;
}

void __cdecl Draw_Colony_ETA_(int16_t unused_planet_sum_idx, int16_t scroll_idx) {
    (void)unused_planet_sum_idx;
    int16_t width = animate::Get_Width_(_screen->pics[13]);
    int16_t height = animate::Get_Height_(_screen->pics[13]);

    int16_t x = 0x66 - width;
    int16_t y = (_plntscrn_delta_y[scroll_idx] - height) + 0x54;

    animate::Draw_(x, y, _screen->pics[13]);
}

int16_t __cdecl Get_ETA_To_Star_(int32_t star_idx) {
    int16_t best_ship = -1;

    if (MOX::_scroll_bar->total_rows > 0) {
        for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
            if (best_ship != -1) {
                break;
            }

            int16_t planet_idx = _planet_summary[i].explored;
            if (static_cast<int16_t>(MOX::_planet[planet_idx].star_index) == static_cast<int16_t>(star_idx)) {
                best_ship = _planet_summary[i].best_colony_ship_idx;
                if (best_ship <= _planet_summary[i].best_outpost_ship_idx) {
                    best_ship = _planet_summary[i].best_outpost_ship_idx;
                }
            }
        }
    }

    if (best_ship != -1) {
        return HAROLD::N_Turns_To_Star_(best_ship, (int16_t)star_idx);
    }

    return 0;
}

void __cdecl Draw_Outpost_ETA_(int16_t unused_planet_sum_idx, int16_t scroll_idx) {
    (void)unused_planet_sum_idx;
    int16_t height = animate::Get_Height_(_screen->pics[13]);
    int16_t y = _plntscrn_delta_y[scroll_idx] - height + 0x54;
    animate::Draw_(0x12, y, _screen->pics[13]);
}

void __cdecl Colony_Owner_Colors_(int16_t colony_idx, s_colors* colors, int8_t highlight_type) {
    uint8_t local_1;

    if (colony_idx != -1) {
        Player_Colors_(MOX::_colony[colony_idx].owner, colors, highlight_type, &local_1);
    } else {
        Player_Colors_(-1, colors, highlight_type, &local_1);
    }
}

void __cdecl Player_Colors_(int16_t player_idx, s_colors* colors_out, int8_t type, uint8_t* base_color_out) {
    if (player_idx == -1) {
        if (type == 1) {
            MOX::_mox_font_colors_offset = 0x42;
            s_colors* c = MISC::Font_Colors2_(4, 0x40, 0xFF);
            *colors_out = *c;
            *base_color_out = colors_out->colors[1];
        } else if (type == 2) {
            MOX::_mox_font_colors_offset = 0x40;
            s_colors* c = MISC::Font_Colors2_(4, 0x00, 0x8A);
            *colors_out = *c;
            *base_color_out = colors_out->colors[1];
        } else {
            MOX::_mox_font_colors_offset = 0x43;
            s_colors* c = MISC::Font_Colors2_(4, 0x40, 0x41);
            *colors_out = *c;
            *base_color_out = colors_out->colors[1];
        }
        return;
    }

    uint8_t base_color = static_cast<uint8_t>(MOX::_player[player_idx].color * 6) - 0x31;
    *base_color_out = base_color;

    if (type == 0) {
        MOX::_mox_font_colors_offset = base_color + 2;
        s_colors* c = MISC::Font_Colors2_(4, base_color, base_color + 4);
        *colors_out = *c;
    } else if (type == 1) {
        MOX::_mox_font_colors_offset = base_color + 3;
        s_colors* c = MISC::Font_Colors2_(4, base_color, base_color + 5);
        *colors_out = *c;
    } else if (type == 2) {
        MOX::_mox_font_colors_offset = base_color + 1;
        s_colors* c = MISC::Font_Colors2_(4, base_color, base_color + 3);
        *colors_out = *c;
    }
}

}