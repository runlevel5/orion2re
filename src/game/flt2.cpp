#include "pch.h"

namespace FLT2 {
void __cdecl Get_Small_Icon_Animation_Size_(int16_t icon_idx, int16_t* width_out, int16_t* height_out)
{
    int16_t node_idx = MOX::_ship_icon[icon_idx].node_idx;
    int16_t owner = HACCESS::Ship_Stack_Owner_(node_idx);

    s_animation_header* anim;
    if (owner < 8) {
        anim = MOX::_fltscrn_small_ship_icon_seg[MOX::_player[owner].color];
    }
    else {
        int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
        anim = MOX::_shield_div_bitmap[MOX::_ship[ship_idx].owner];
    }

    *width_out = animate::Get_Width_(anim);
    *height_out = animate::Get_Height_(anim);
}

    void __cdecl Add_Fltscrn_Small_Icon_Fields_() {
        int16_t width;
        int16_t height;

        for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
            Get_Small_Icon_Animation_Size_(i, &width, &height);

            MOX::_ship_icon[i].stack_id = fields::Add_Hidden_Field_(
                MOX::_ship_icon[i].x,
                MOX::_ship_icon[i].y,
                MOX::_ship_icon[i].x + width,
                MOX::_ship_icon[i].y + height,
                const_cast<char*>(s__00551100),
                0x29
            );
        }
    }


void __cdecl Move_Ship_To_Front_Of_List_(int16_t icon_idx) {
    if (MOX::_n_fltscrn_big_icons > 1) {
        s_big_icon temp = MOX::_fltscrn_big_icon[0];
        MOX::_fltscrn_big_icon[0] = MOX::_fltscrn_big_icon[icon_idx];
        MOX::_fltscrn_big_icon[icon_idx] = temp;
    }

    OFFICER::_fleet_icon_ship_id_to_move = -1;
}

void __cdecl Set_Initial_Stack_Pointer_() {
    FLT1::Set_Small_Ship_Stack_Pointer_(true);
}

void Fleet_Screen_Big_Icon_Fields_(int16_t* button_id_ptr, int16_t x, int16_t y, int16_t w, int16_t h) {
    if (MOX::_small_ship_stack_ptr > -1 && 
        (MOX::_old_small_ship_stack_ptr != MOX::_small_ship_stack_ptr || 
         MOX::_scroll_bar->first_visible_row != MOX::_old_first_fltscrn_icon || 
         MOX::_scroll_bar->first_visible_row == -1)) 
    {
        MOX::_old_small_ship_stack_ptr = MOX::_small_ship_stack_ptr;
        MOX::_old_first_fltscrn_icon = MOX::_scroll_bar->first_visible_row;
        FLT2::Build_Fltscrn_Big_Icon_Fields_(MOX::_small_ship_stack_ptr, MOX::_scroll_bar->first_visible_row);
        
        int16_t ship_idx = MOX::_ship_stack_start[MOX::_small_ship_stack_ptr];
        int16_t owner = HACCESS::Ship_Stack_Owner_(ship_idx);
        KEN::Load_Player_Ship_Palette_(owner);
    } else {
        if (OFFICER::_fleet_icon_ship_id_to_move > -1 && MOX::_n_fltscrn_big_icons > 1) {
            bool moved = false;
            for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons && !moved; ++i) {
                if (MOX::_fltscrn_big_icon[i].ship_idx == OFFICER::_fleet_icon_ship_id_to_move) {
                    FLT2::Move_Ship_To_Front_Of_List_(i);
                    moved = true;
                }
            }
        }
    }

    if (MOX::_n_fltscrn_big_icons > 0) {
        MOX::_n_fltscrn_rows = (int8_t)(MOX::_n_fltscrn_big_icons / MOX::_big_icon_display_columns);
        if ((MOX::_n_fltscrn_big_icons % MOX::_big_icon_display_columns) > 0) {
            MOX::_n_fltscrn_rows++;
        }

        if (MOX::_scroll_bar->first_visible_row < 0 || MOX::_scroll_bar->first_visible_row >= 500) {
            MOX::_scroll_bar->first_visible_row = 0;
        }

        FLT2::Add_Fltscrn_Big_Icon_Fields_(MOX::_scroll_bar->first_visible_row * MOX::_big_icon_display_columns);
    } else {
        MOX::_scroll_bar->first_visible_row = 0;
    }

    if (MOX::_n_fltscrn_rows <= MOX::_scroll_bar->first_visible_row) {
        MOX::_scroll_bar->first_visible_row = 0;
    }

    *button_id_ptr = -1000;
    if (MOX::_big_icon_display_columns < MOX::_n_fltscrn_big_icons) {
        *button_id_ptr = fields::Add_Scroll_Field_(x, y, 0, h, 0, h, w, h, &MOX::_scroll_bar->thumb_center_y, s__00551100, 0);
    }
}

void __cdecl Clear_All_Selection_Status_() {
    for (int i = 0; i < (int)MOX::_n_fltscrn_big_icons; ++i) {
        MOX::_fltscrn_big_icon[i].selected = 0;
    }
}

void __cdecl Get_Fltscrn_Big_Icon_XY_(int16_t icon_idx, int16_t* out_x, int16_t* out_y) {
    // This comparison is present in the original binary but its result is not used.
    (void)(MOX::_current_screen == 4);

    int16_t columns = MOX::_big_icon_display_columns;

    // The original code performs division/modulo operations twice with 16-bit intermediates.
    int16_t remainder = (int16_t)(icon_idx % columns);
    *out_x = (int16_t)((int16_t)(remainder * 62) + MOX::_big_icon_x_offset);

    int16_t quotient = (int16_t)(icon_idx / columns);
    *out_y = (int16_t)((int16_t)(quotient * 60) + MOX::_big_icon_y_offset);
}

int16_t __cdecl Get_Filter_Button_Status_()
{
    if (MOX::_current_screen == 0x1D ||
        (MOX::_fltscrn_combat_button_status == 1 && MOX::_fltscrn_support_button_status == 1)) {
        return 0;
    }

    if (MOX::_fltscrn_combat_button_status == 1) {
        return 2;
    }

    if (MOX::_fltscrn_support_button_status == 1) {
        return 1;
    }

    return -1;
}

int16_t __cdecl Fltscrn_Ships_To_Be_Displayed_(int16_t ship_stack_idx) {
    int16_t x = HACCESS::Ship_Stack_X_(ship_stack_idx);
    int16_t y = HACCESS::Ship_Stack_Y_(ship_stack_idx);
    int16_t owner = HACCESS::Ship_Stack_Owner_(ship_stack_idx);

    int16_t total_ships;
    int16_t bases_count;
    int16_t colony_ships_count;
    int16_t transports_count;
    int16_t ship_sizes[6];

    SYS::N_Ships_By_Type_At_Location_(x, y, owner, &total_ships, &bases_count, &colony_ships_count, &transports_count, ship_sizes);

    int16_t filter_status = FLT2::Get_Filter_Button_Status_();

    if (filter_status == 0) {
        return total_ships;
    }

    if (filter_status == 1) {
        return bases_count + colony_ships_count + transports_count;
    }

    if (filter_status == 2) {
        return total_ships - (bases_count + colony_ships_count + transports_count);
    }

    return 0;
}

bool __cdecl Ok_To_Add_Ship_(int16_t ship_idx) {
    int16_t ship_type = (int8_t)MOX::_ship[ship_idx].d.ship_type;
    int16_t filter_status = Get_Filter_Button_Status_();

    if (filter_status == 0) {
        return true;
    }

    if (filter_status == 1) {
        if (ship_type == 2 || ship_type == 1 || ship_type == 4) {
            return true;
        }
        return false;
    }

    if (filter_status == 2) {
        return ship_type == 0;
    }

    return false;
}

void __cdecl Fix_Palette_(int16_t player_idx) {
    KEN::Load_Player_Ship_Palette_(player_idx);

    MOX::_skip_draw_ships = 1;

    video::Set_Page_Off_();
    FLT1::Draw_Fleet_Screen_();
    RUSS::Mox_Sync_Update_();
    palette::Refresh_Palette_();
}

void __cdecl Build_Fltscrn_Big_Icon_Fields_(int16_t stack_idx, int16_t first_visible_row) {
    (void)first_visible_row;

    int16_t ship_idx_list[250];
    int16_t selected_count = 0;
    int16_t target_icon_idx = -1;
    int16_t next_node_idx = MOX::_ship_stack_start[stack_idx];

    MOX::_n_fltscrn_big_icons_added = 0;

    memset(ship_idx_list, 0xff, sizeof(ship_idx_list));

    if (MOX::_n_fltscrn_big_icons > 0) {
        for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; ++i) {
            if (MOX::_fltscrn_big_icon[i].selected != 0) {
                ship_idx_list[selected_count] = MOX::_fltscrn_big_icon[i].ship_idx;
                selected_count++;
            }
        }
    }

    memset(MOX::_fltscrn_big_icon, 0, 250 * sizeof(s_big_icon));

    for (int i = 0; i < 250; ++i) {
        MOX::_fltscrn_big_icon[i].button_id = -1000;
    }

    MOX::_n_fltscrn_big_icons = 0;

    while (next_node_idx != -1 && MOX::_n_fltscrn_big_icons < 250) {
        int16_t ship_idx = MOX::_ship_node[next_node_idx].ship_idx;

        if (FLT2::Ok_To_Add_Ship_(ship_idx) && MOX::_ship[ship_idx].status < 3) {
            MOX::_fltscrn_big_icon[MOX::_n_fltscrn_big_icons].ship_idx = ship_idx;

            if (OFFICER::_fleet_icon_ship_id_to_move != -1 &&
                OFFICER::_fleet_icon_ship_id_to_move == ship_idx) {
                target_icon_idx = MOX::_n_fltscrn_big_icons;
            }
            MOX::_n_fltscrn_big_icons++;
        }
        next_node_idx = MOX::_ship_node[next_node_idx].next_node;
    }

    if (MOX::_n_fltscrn_big_icons > 0) {
        for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; ++i) {
            if (ERIC::Sint_Is_In_List_(MOX::_fltscrn_big_icon[i].ship_idx, ship_idx_list, selected_count)) {
                MOX::_fltscrn_big_icon[i].selected = 1;
            }
        }
    }

    if (target_icon_idx != -1 && MOX::_n_fltscrn_big_icons > 1) {
        FLT2::Move_Ship_To_Front_Of_List_(target_icon_idx);
    }

    FLT2::Set_Fleet_Screen_Stack_Owner_();
}

void __cdecl Set_Fleet_Screen_Stack_Owner_() {
    MOX::_fltscrn_stack_owner = -1;
    if (MOX::_small_ship_stack_ptr > -1) {
        MOX::_fltscrn_stack_owner = HACCESS::Ship_Owner_(
            MOX::_ship_node[MOX::_ship_stack_start[MOX::_small_ship_stack_ptr]].ship_idx
        );
    }
}

void __cdecl Add_Fltscrn_Big_Icon_Fields_(int16_t base_index) {
    int16_t height_offset;
    int16_t width_offset;
    int16_t search_idx;
    int16_t owner;
    int16_t x;
    int16_t y;
    int16_t icon_idx;

    if (MOX::_current_screen == 4 /* SCREEN_GALAXY */) {
        width_offset = 0x3a;
        height_offset = 0x39;
    } else {
        width_offset = 0x39;
        height_offset = 0x39;
    }

    search_idx = 0;
    while (MOX::_fltscrn_big_icon[base_index + search_idx].ship_idx == -1) {
        search_idx++;
    }

    owner = HACCESS::Ship_Owner_(MOX::_fltscrn_big_icon[base_index + search_idx].ship_idx);
    MOX::_n_fltscrn_big_icons_added = 0;

    if (MOX::_big_icon_display_max_icons > 0) {
        do {
            if (MOX::_n_fltscrn_big_icons_added >= MOX::_n_fltscrn_big_icons) {
                break;
            }
            if (MOX::_n_fltscrn_big_icons_added + base_index >= MOX::_n_fltscrn_big_icons) {
                break;
            }

            FLT2::Get_Fltscrn_Big_Icon_XY_(MOX::_n_fltscrn_big_icons_added, &x, &y);

            icon_idx = base_index + MOX::_n_fltscrn_big_icons_added;
            MOX::_fltscrn_big_icon[icon_idx].screen_x = x;
            MOX::_fltscrn_big_icon[icon_idx].screen_y = y;

            if (MOX::_PLAYER_NUM == owner) {
                MOX::_fltscrn_big_icon[icon_idx].button_id = fields::Add_Hidden_Field_(
                    x,
                    y,
                    x + width_offset,
                    y + height_offset,
                    FLT2::s__00551100,
                    0x29
                );
            } else {
                MOX::_fltscrn_big_icon[icon_idx].button_id = -1000;
            }

            MOX::_n_fltscrn_big_icons_added++;

        } while (MOX::_n_fltscrn_big_icons_added < MOX::_big_icon_display_max_icons);
    }
}

void __cdecl Print_Fltscrn_Scanned_Star_Name_() {
    char local_50[80];
    s_colors local_58;
    int16_t local_5a = 0;
    uint8_t visited;
    uint8_t has_moving_ships = 0;
    int16_t status = -1;
    int16_t owner = -1;
    uint16_t val = 0;

    visited = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, MOX::_galaxy_map_scanned_star);

    for (int i = 0; i < 250; ++i) {
        MOX::_fltscrn_ship_icon_selection_list[i] = -1;
    }

    local_58 = _normal_colors;

    if (Build_Fltscrn_Ship_List_(MOX::_fltscrn_ship_icon_selection_list) != 0) {
        has_moving_ships = 1;
        SHIPMOVE::Ships_Try_To_Move_To_(
            MOX::_fltscrn_ship_icon_selection_list,
            MOX::_galaxy_map_scanned_star,
            &MOX::_g_ship_move_info
        );
    } else {
        MOX::_g_ship_move_info.blackhole_blocks = 0;
        MOX::_g_ship_move_info.insufficient_fuel = 0;
        MOX::_g_ship_move_info.immobile = 0;
        MOX::_g_ship_move_info.pad = 0;
        MOX::_g_ship_move_info.turns_left = 0;
        MOX::_g_ship_move_info.moving = 0;
        MOX::_g_ship_move_info.out_of_range = 0;
        MOX::_g_ship_move_info.has_navigator = 0;
        MOX::_g_ship_move_info.traveling_speed = 0;
        MOX::_g_ship_move_info.parsecs = 0;
        MOX::_g_ship_move_info.uses_wormhole = 0;
        MOX::_g_ship_move_info.has_jumpgate_route = 0;
        MOX::_g_ship_move_info.has_stargate_route = 0;
        MOX::_g_ship_move_info.hyperspace_flux = 0;

        if (visited == 0 &&
            MOX::_player[MOX::_PLAYER_NUM].traits[27] == 0 &&
            OFFICER::One_Leader_With_Galactic_Lore_(MOX::_PLAYER_NUM) == 0) {

            if (HAROLD::Contact_With_One_Colony_(MOX::_galaxy_map_scanned_star) == 0) {
                status = 8;
            } else {
                status = 7;
                val = MOX::_galaxy_map_scanned_star;
            }
        } else {
            status = 7;
            val = MOX::_galaxy_map_scanned_star;
        }
    }

    if (MOX::_g_ship_move_info.blackhole_blocks != 0) {
        status = 3;
    } else if (MOX::_g_ship_move_info.immobile != 0) {
        status = 4;
    } else if (MOX::_g_ship_move_info.out_of_range != 0) {
        if (visited != 0) {
            status = 5;
            val = MOX::_g_ship_move_info.parsecs;
            local_5a = MOX::_galaxy_map_scanned_star;
        } else {
            status = 6;
            val = MOX::_g_ship_move_info.parsecs;
        }
    } else if (MOX::_g_ship_move_info.hyperspace_flux != 0) {
        status = 9;
    } else if (MOX::_g_ship_move_info.moving != 0) {
        if (MOX::_star[MOX::_galaxy_map_scanned_star].spectral_class == 6) {
            status = 3;
        } else {
            owner = MOX::_star[MOX::_galaxy_map_scanned_star].owner;
            if (MOX::_g_ship_move_info.turns_left == 0) {
                status = 0;
                val = MOX::_galaxy_map_scanned_star;
            } else {
                if (visited != 0) {
                    status = 1;
                    val = MOX::_g_ship_move_info.turns_left;
                    local_5a = MOX::_galaxy_map_scanned_star;
                } else {
                    status = 2;
                    val = MOX::_g_ship_move_info.turns_left;
                }
            }
        }
    }

    switch (status) {
    case 0:
        sprintf(local_50, HAROLD::H_Message_(0x67), MOX::_star[val].name);
        local_58 = _normal_colors;
        break;
    case 1:
        if (val == 1) {
            sprintf(local_50, HAROLD::H_Message_(0x69), val, MOX::_star[local_5a].name);
        } else {
            sprintf(local_50, HAROLD::H_Message_(0x6A), val, MOX::_star[local_5a].name);
        }
        local_58 = _normal_colors;
        break;
    case 2:
        if (val == 1) {
            sprintf(local_50, HAROLD::H_Message_(0x6B), val);
        } else {
            sprintf(local_50, HAROLD::H_Message_(0x6C), val);
        }
        local_58 = _normal_colors;
        break;
    case 3:
        strcpy(local_50, HAROLD::H_Message_(0x21));
        local_58 = _neutral_colors;
        break;
    case 4:
        strcpy(local_50, HAROLD::H_Message_(0x6E));
        local_58 = _neutral_colors;
        break;
    case 5:
        if (val == 1) {
            sprintf(local_50, HAROLD::H_Message_(0x6F), val, MOX::_star[local_5a].name);
        } else {
            sprintf(local_50, HAROLD::H_Message_(0x70), val, MOX::_star[local_5a].name);
        }
        local_58 = _neutral_colors;
        break;
    case 6:
        sprintf(local_50, HAROLD::H_Message_(0xEB), val);
        local_58 = _neutral_colors;
        break;
    case 7:
        owner = MOX::_star[MOX::_galaxy_map_scanned_star].owner;
        if (owner < 0 || owner >= 8) {
            owner = -1;
            local_58 = _neutral_colors;
        }
        {
            int8_t leader_idx = static_cast<int8_t>(MOX::_star[val].officer_index[MOX::_PLAYER_NUM]);
            if (leader_idx == -1 || HAROLD::Player_Has_Colony_(MOX::_PLAYER_NUM, val) == 0) {
                strcpy(local_50, MOX::_star[val].name);
            } else {
                s_leader_data* leader = &MOX::_leaders[leader_idx];
                if (leader->eta <= 0) {
                    sprintf(local_50, s_s_s_005569c0, MOX::_star[val].name, leader->name);
                } else if (leader->eta == 1) {
                    sprintf(local_50, HAROLD::H_Message_(0x92), MOX::_star[val].name, leader->name, leader->eta);
                } else {
                    sprintf(local_50, HAROLD::H_Message_(0x93), MOX::_star[val].name, leader->name, leader->eta);
                }
            }
        }
        break;
    case 8:
        strcpy(local_50, HAROLD::H_Message_(0x94));
        local_58 = _neutral_colors;
        break;
    case 9:
        strcpy(local_50, HAROLD::H_Message_(0x73));
        local_58 = _neutral_colors;
        break;
    default:
        break;
    }

    MISC::Set_Fitted_Font_Style_(local_50, &local_58, 3, 2, 0, 0xBD, 1);

    if (owner != -1 && has_moving_ships == 0) {
        uint8_t color_idx = MOX::_main_palette_player_colors[static_cast<uint8_t>(MOX::_player[owner].color)];
        if (color_idx == 0x20) {
            color_idx = 0xF;
        }
        s_colors* temp_colors = MISC::Font_Colors2_(fonts::_current_font_style, color_idx, color_idx + 6);
        local_58 = *temp_colors;
        MISC::Set_Font_Colors_(&local_58);
    }

    int16_t str_w = fonts::Get_String_Width_(local_50);
    int16_t h = fonts::Get_Font_Height_();

    fonts::Print_(169 - (str_w / 2), ((23 - h) / 2) + 248, local_50);
}

void __cdecl Print_Scanned_Ship_Data_(int16_t ship_idx) {
    int16_t defense_bonus;
    int16_t missile_defense_bonus;
    int16_t attack_bonus;
    char str_9a[20];
    char buffer[120];
    char unused_msg_95[20];
    char unused_msg_96[20];
    char unused_msg_97[20];
    char unused_msg_98[20];

    uint8_t ship_type = MOX::_ship[ship_idx].d.ship_type;
    bool is_ship_type_2 = (ship_type == 2);
    bool is_ship_type_1 = (ship_type == 1);
    bool is_ship_type_4 = (ship_type == 4);

    // This code seems to be no-op but it is executed in the original assembly
    strcpy(unused_msg_95, HAROLD::H_Message_(0x95));
    strcpy(unused_msg_96, HAROLD::H_Message_(0x96));
    strcpy(unused_msg_97, HAROLD::H_Message_(0x97));
    strcpy(unused_msg_98, HAROLD::H_Message_(0x98));

    INITSHIP::Get_Ship_Combat_Bonuses_(ship_idx, &defense_bonus, &missile_defense_bonus, &attack_bonus);

    if (is_ship_type_2 || is_ship_type_1 || is_ship_type_4) {
        allocate::Push_Block_(MOX::_screen_seg);
        void* block_10000 = allocate::Allocate_Next_Block_(MOX::_screen_seg, 10000);
        allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x57B);
        
        int16_t help_idx = 0;
        if (ship_type == 1) {
            help_idx = 0x29;
        } else if (ship_type == 2) {
            help_idx = 0xBD;
        } else if (ship_type == 4) {
            help_idx = 0x6D;
        }

        char lbx_name[20];
        TEXTBOX::Get_Help_Lbx_Name_(lbx_name);
        
        void* lbx_data = farload::Far_Reload_Next_Data_(lbx_name, 0, block_10000, help_idx, 1, 0x57B);
        MOX::_mox_font_colors_offset = 0x72;
        
        s_colors temp_colors;
        MISC::Get_Mox_Font_Colors_(3, 111, 116, &temp_colors);
        
        fonts::Set_Font_Style_(3, &temp_colors);
        ERIC::Print_Paragraph_Centered_Vertically_(0x12, 0x11A, 0x12B, 0xB7, (char*)lbx_data + 0x67, 0);
        
        allocate::Pop_Block_(MOX::_screen_seg);
        return;
    }

    bool has_captain = FLT2::Draw_Captains_Picture_(ship_idx) != 0;

    fonts::Set_Font_Style_(3, &FLT2::_normal_colors);
    fonts::Print_(0x12, 0x11F, MOX::_ship[ship_idx].d.name);
    
    fonts::Set_Font_Style_(2, &FLT2::_normal_colors);
    int16_t y_cursor = fonts::Get_Font_Height_() + 2;
    
    fonts::Set_Font_Style_(2, &FLT2::_normal_colors);
    fonts::Set_Font_Spacing_(1);
    
    char* crew_desc = FLT2::Crew_Description_String_(ship_idx);
    int16_t exp = MOX::_ship[ship_idx].crew_experience;
    sprintf(buffer, s_fmt_s_d_EP_00557e18, crew_desc, exp);
    
    fonts::Get_String_Width_(buffer);
    fonts::Print_(0x12, y_cursor + 0x11F, buffer);
    
    y_cursor = fonts::Get_Font_Height_() + y_cursor + 2;
    
    int16_t shield_idx = MOX::_ship[ship_idx].d.shield_type;
    fonts::Print_(0x12, y_cursor + 0x11F, TECHDATA::_shields[shield_idx].name);
    
    int16_t fh = fonts::Get_Font_Height_();
    int16_t EBP_y = fh + y_cursor + 0x121;
    y_cursor = fh + y_cursor + 2;
    
    fonts::Print_(0x12, EBP_y, HAROLD::H_Message_(0x99));
    
    const char* sign1 = (attack_bonus >= 0) ? s_plus_00551118 : s__00551100;
    sprintf(buffer, s__s_d_00556134, sign1, attack_bonus);
    fonts::Print_Right_(0x85, EBP_y, buffer);
    
    strcpy(str_9a, HAROLD::H_Message_(0x9A));
    
    int16_t w;
    if (!has_captain) {
        w = fonts::Get_String_Width_(str_9a);
        if (w + 0x9B < 0x10E) {
            w = 0x73;
        }
        fonts::Print_(0xAD, EBP_y, str_9a);
        
        const char* sign2 = (defense_bonus >= 0) ? s_plus_00551118 : s__00551100;
        sprintf(buffer, s__s_d_00556134, sign2, defense_bonus);
        fonts::Print_(w + 0xAD, EBP_y, buffer);
    } else {
        w = fonts::Get_String_Width_(str_9a);
        
        fh = fonts::Get_Font_Height_();
        EBP_y = fh + y_cursor + 0x121;
        y_cursor = fh + y_cursor + 2;
        
        fonts::Print_(0x12, EBP_y, str_9a);
        
        const char* sign2 = (defense_bonus >= 0) ? s_plus_00551118 : s__00551100;
        sprintf(buffer, s__s_d_00556134, sign2, defense_bonus);
        
        if (w < 0x73) {
            w = 0x73;
        }
        w += 0x12;
        fonts::Print_(w, EBP_y, buffer);
    }
    
    y_cursor = fonts::Get_Font_Height_() + y_cursor + 2;
    
    int16_t loc = MOX::_ship[ship_idx].location;
    if (loc >= 500 && loc <= MOX::_NUM_STARS + 1000) {
        int16_t star_idx = HAROLD::Absolute_Location_(loc);
        bool visited = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx);
        
        s_colors temp_colors;
        for (int i = 0; i < 8; ++i) {
            temp_colors.colors[i] = FLT2::_normal_colors.colors[i] + 3;
        }
        fonts::Set_Font_Style_(2, &temp_colors);
        
        bool has_info = false;
        if (visited || MOX::_player[MOX::_PLAYER_NUM].traits[27] != 0 || OFFICER::One_Leader_With_Galactic_Lore_(MOX::_PLAYER_NUM)) {
            has_info = true;
        } else {
            has_info = HAROLD::Contact_With_One_Colony_(star_idx);
        }
        
        const char* loc_str;
        if (MOX::_NUM_STARS == star_idx) {
            loc_str = HAROLD::H_Message_(0x68);
        } else {
            if (!has_info) {
                loc_str = HAROLD::H_Message_(0x9C);
            } else {
                sprintf(buffer, (const char*)HAROLD::H_Message_(0x9B), MOX::_star[star_idx].name);
                fonts::Get_String_Width_(buffer);
                loc_str = buffer;
            }
        }
        fonts::Print_(0x12, y_cursor + 0x11F, loc_str);
        fonts::Set_Font_Style_(2, &FLT2::_normal_colors);
    }
    
    fh = fonts::Get_Font_Height_();
    int16_t ESI_y = fh + y_cursor + 0x121;
    y_cursor = fh + y_cursor + 2;
    
    fonts::Print_(0x12, ESI_y, HAROLD::H_Message_(0x9D));
    fonts::Print_(0xAD, ESI_y, HAROLD::H_Message_(0x9E));
    
    int16_t base_y = fonts::Get_Font_Height_() + y_cursor + 1;
    int16_t wep_y = base_y;
    int16_t spec_y = base_y;
    
    fonts::Set_Font_Style_(1, &FLT2::_normal_colors);
    fonts::Set_Font_Spacing_(1);
    
    bool no_weapons = false;
    int16_t weapons_printed = 0;
    
    for (int i = 0; i < 8; ++i) {
        if (no_weapons) {
            break;
        }
        if (MOX::_ship[ship_idx].d.ship_weapon[i].type < 0 || MOX::_ship[ship_idx].d.ship_weapon[i].count < 1) {
            no_weapons = true;
        } else {
            char arc_str[20];
            DESIGN::Weapon_Arc_String_(MOX::_ship[ship_idx].d.ship_weapon[i].firing_arc, arc_str);
            int16_t wep_type = MOX::_ship[ship_idx].d.ship_weapon[i].type;
            int8_t count = MOX::_ship[ship_idx].d.ship_weapon[i].count;
            
            const char* wep_name;
            if (count == 1) {
                wep_name = TECHDATA::_weapons[wep_type].name;
            } else {
                wep_name = TECHDATA::_weapons[wep_type].name_plural;
            }
            
            sprintf(buffer, s_fmt_d_s_s_00557e0c, count, wep_name, arc_str);
            fonts::Print_(0x17, wep_y + 0x11F, buffer);
            wep_y += fonts::Get_Font_Height_() + 1;
            weapons_printed++;
        }
    }
    
    if (weapons_printed == 0) {
        fonts::Print_(0x17, wep_y + 0x11F, HAROLD::H_Message_(0x9F));
    }
    
    int16_t specials_printed = 0;
    
    for (int i = 0; i < 39; ++i) {
        if (struct_::Test_Bit_Field_(MOX::_ship[ship_idx].d.special_device_flags, i) != 0) {
            s_colors* colors;
            if (struct_::Test_Bit_Field_(MOX::_ship[ship_idx].special_device_damage_flags, i) == 0) {
                colors = &FLT2::_normal_colors;
            } else {
                colors = &FLT2::_red_colors;
            }
            fonts::Set_Font_Style_(1, colors);
            fonts::Set_Font_Spacing_(1);
            fonts::Print_(0xBC, spec_y + 0x11F, TECHDATA::_specials[i].name);
            spec_y += fonts::Get_Font_Height_() + 1;
            specials_printed++;
        }
    }
    
    if (specials_printed == 0) {
        fonts::Print_(0xBC, spec_y + 0x11F, HAROLD::H_Message_(0x9F));
    }
}

char* __cdecl Crew_Description_String_(int16_t ship_idx) {
    MOX::_temp_string[0] = '\0';

    switch (MOX::_ship[ship_idx].crew_quality) {
        case 0:
            strcpy(MOX::_temp_string, HAROLD::H_Message_(0x8A));
            break;
        case 1:
            strcpy(MOX::_temp_string, HAROLD::H_Message_(0x8B));
            break;
        case 2:
            strcpy(MOX::_temp_string, HAROLD::H_Message_(0x8C));
            break;
        case 3:
            strcpy(MOX::_temp_string, HAROLD::H_Message_(0x8D));
            break;
        case 4:
            strcpy(MOX::_temp_string, HAROLD::H_Message_(0x8E));
            break;
        default:
            break;
    }

    return MOX::_temp_string;
}

void __cdecl Fltscrn_Move_Ships_(int16_t target_star) {
    char msg_buf[200];
    char msg_buf2[88];

    memset(MOX::_fltscrn_ship_icon_selection_list, 0xFF, 0x7D * 4);

    if (Build_Fltscrn_Ship_List_(MOX::_fltscrn_ship_icon_selection_list) != 0) {
        SHIPMOVE::Ships_Try_To_Move_To_(MOX::_fltscrn_ship_icon_selection_list, target_star, &MOX::_g_ship_move_info);

        if (MOX::_g_ship_move_info.moving != 0) {
            SHIPMOVE::Make_Ships_Move_To_(MOX::_fltscrn_ship_icon_selection_list, target_star, &MOX::_g_ship_move_info);
            SHIPS::Build_Ship_Icons_();
            FLT::Set_Fltscrn_Small_Ship_Icon_XYs_(0xF, 0x34, 0x131, 0xB6);
            MOX::_draw_fleet_screen_to_back = 1;
            MOX::_old_small_ship_stack_ptr = -1;
            FLT1::Set_Fltscrn_Big_Icons_(0);
            return;
        }

        if (MOX::_g_ship_move_info.blackhole_blocks != 0) {
            strcpy(msg_buf2, HAROLD::H_Message_(0x21));
            sprintf(msg_buf, s_fmt_to_00557e00, msg_buf2, MOX::_star[target_star].name);
            HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 3);
            return;
        }

        if (MOX::_g_ship_move_info.out_of_range != 0) {
            if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, target_star) ||
                HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM)) {
                sprintf(msg_buf, HAROLD::H_Message_(0x90), MOX::_star[target_star].name);
            } else {
                strcpy(msg_buf, HAROLD::H_Message_(0x91));
            }
            HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 3);
            return;
        }

        if (MOX::_g_ship_move_info.hyperspace_flux != 0) {
            strcpy(msg_buf, HAROLD::H_Message_(0x24));
            HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 3);
        }
    }
}

uint8_t __cdecl Draw_Captains_Picture_(int16_t param_1) {
    int16_t leader_idx = 0;
    s_colors auStack_58;
    char local_50[80];

    auStack_58.colors[0] = 0x4C; /* 'L' */
    auStack_58.colors[1] = 0x4D; /* 'M' */
    auStack_58.colors[2] = 0x4E; /* 'N' */
    auStack_58.colors[3] = 0x4F; /* 'O' */
    auStack_58.colors[4] = 0x3F; /* '?' */
    auStack_58.colors[5] = 0x40; /* '@' */
    auStack_58.colors[6] = 0x41; /* 'A' */
    auStack_58.colors[7] = 0x42; /* 'B' */

    int16_t valid_captain_idx = 0;
    uint8_t found = 0;

    for (int16_t i = 0; i < 67; i++) {
        if (MOX::_leaders[i].player_index == MOX::_PLAYER_NUM &&
            MOX::_leaders[i].status == 1 &&
            MOX::_leaders[i].type == 0) {

            if (MOX::_leaders[i].location == param_1) {
                leader_idx = i;
                found = 1;
                break;
            }
            valid_captain_idx++;
        }
    }

    if (found) {
        s_animation_header* pict = MOX::_temp_officers_pict[valid_captain_idx];
        int16_t width = animate::Get_Width_(pict);
        int16_t height = animate::Get_Height_(pict);

        char* leader_name = OFFICER::Leader_Name_(leader_idx, 1);
        sprintf(local_50, s__s_00551130, leader_name);

        fonts::Set_Font_Style_(0, &_normal_colors);
        ERIC::Print_Paragraph_Centered_Vertically_(0xAD, 0x11F, 0x42, height, local_50, 2);
        animate::Draw_(0xF2, 0x11C, pict);

        if (MOX::_leaders[leader_idx].eta > 0) {
            OFFICER::Print_ETA_On_Officer_Picture_(leader_idx, 0xF2, 0x11C, width);
            width += 2;
            height += 2;
            MISC::Draw_Highlight_Box_(0xF0, 0x11A, width, height, &auStack_58.colors[0]);
        } else {
            width += 2;
            height += 2;
            MISC::Draw_Highlight_Box_(0xF0, 0x11A, width, height, &auStack_58.colors[4]);
        }
    }

    return found;
}

int16_t __cdecl Build_Fltscrn_Ship_List_(int16_t* ship_list) {
    int16_t retval = 1;
    int16_t error_flag = 0;
    int16_t count = 0;

    if (MOX::_n_fltscrn_big_icons > 0) {
        for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; i++) {
            if (error_flag != 0) {
                break;
            }
            if (MOX::_fltscrn_big_icon[i].selected != 0) {
                ship_list[count] = MOX::_fltscrn_big_icon[i].ship_idx;
                count++;
                if (count >= 250) {
                    HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, s_Too_many_ships_in_list_00557de8, 0);
                    error_flag = 1;
                    retval = 0;
                }
            }
        }
    }

    if (count == 0) {
        retval = 0;
    }

    return retval;
}

int16_t __cdecl Scan_Fltscrn_Big_Icons_(int16_t btn_id1, int16_t btn_id2, int16_t* out_idx1, int16_t* out_idx2) {
    int16_t first_icon = MOX::_scroll_bar->first_visible_row * MOX::_big_icon_display_columns;
    int16_t last_icon = first_icon + 0x14;

    if (last_icon >= MOX::_n_fltscrn_big_icons) {
        last_icon = MOX::_n_fltscrn_big_icons;
    }

    bool stop_flag = false;
    *out_idx2 = -1;
    *out_idx1 = -1;
    int16_t result = -1;

    if (first_icon < 0 || last_icon < 0) {
        stop_flag = true;
    }

    for (int16_t i = first_icon; i < last_icon; ++i) {
        if (stop_flag) {
            break;
        }

        if (btn_id2 == MOX::_fltscrn_big_icon[i].button_id) {
            result = 0;
            stop_flag = true;
            *out_idx2 = i;
            *out_idx1 = i;
        } else if (btn_id2 + MOX::_fltscrn_big_icon[i].button_id == 0) {
            result = 1;
            stop_flag = true;
            *out_idx2 = i;
            *out_idx1 = i;
        }
    }

    if (result == -1) {
        for (int16_t i = first_icon; i < last_icon; ++i) {
            if (stop_flag) {
                break;
            }

            if (MOX::_fltscrn_big_icon[i].button_id == btn_id1) {
                result = 4;
                *out_idx1 = i;
            }
        }
    }

    return result;
}

} // namespace FLT2