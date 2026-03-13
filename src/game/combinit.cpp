#include "pch.h"

namespace COMBINIT {

int8_t __cdecl Colony_Has_Defenses_(int16_t colony_idx) {
    s_colony* col = &MOX::_colony[colony_idx];

    if (col->buildings[BUILDING_STAR_BASE] != 0 ||
        col->buildings[BUILDING_FIGHTER_GARRISON] != 0 ||
        col->buildings[BUILDING_MISSILE_BASE] != 0 ||
        col->buildings[BUILDING_BATTLESTATION] != 0 ||
        col->buildings[BUILDING_GROUND_BATTERIES] != 0 ||
        col->buildings[BUILDING_STELLAR_CONVERTER] != 0 ||
        col->buildings[BUILDING_STAR_FORTRESS] != 0) {
        return 1;
    }

    return 0;
}

int __cdecl qsort_ship_array_by_size_(const int16_t* param_1, const int16_t* param_2) {
    char size_2 = MOX::_ship[*param_2].d.size;
    char size_1 = MOX::_ship[*param_1].d.size;

    if (size_2 == size_1) {
        return 0;
    }

    if (size_2 > size_1) {
        return 1;
    }

    return -1;
}

void __cdecl Load_Combat_Ship_(int16_t ship_idx, int16_t combat_idx) {
    s_ship_data* ship = &MOX::_ship[ship_idx];
    s_combat_data* combat = &MOX::_combat_data[combat_idx];

    strcpy(combat->name, ship->d.name);
    combat->owner = ship->owner;
    combat->previous_owner = ship->d.previous_owner;
    combat->ship_idx = ship_idx;
    combat->shield_type = ship->d.shield_type;
    combat->computer_type = ship->d.computer_type;

    if (combat->owner < 8) {
        combat->drive_destroyed = ship->d.ftl_type;
    } else {
        combat->drive_destroyed = (int8_t)INITSHIP::Best_Warp_Drive_(combat->owner);
    }

    combat->combat_speed_base = ship->d.combat_speed;
    combat->size_class = ship->d.size;
    combat->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        combat->special_device_flags[i] = ship->d.special_device_flags[i];
        combat->special_device_damage_flags[i] = ship->special_device_damage_flags[i];
    }

    for (int i = 0; i < 8; ++i) {
        combat->weapons[i].weapon_id = ship->d.ship_weapon[i].type;
        combat->weapons[i].count = ship->d.ship_weapon[i].current_count;
        combat->weapons[i].arc = ship->d.ship_weapon[i].firing_arc;
        combat->weapons[i].specials = ship->d.ship_weapon[i].specials;
        combat->weapons[i].ammo = ship->d.ship_weapon[i].ammo;

        if (combat->weapons[i].arc == 16) {
            combat->weapons[i].arc = 15;
        }

        combat->weapons[i].active = 1;
        combat->weapons[i].shots_left = combat->weapons[i].count;

        uint16_t weapon_id = combat->weapons[i].weapon_id;
        if (TECHDATA::_weapons[weapon_id].rack == 2) {
            combat->weapons[i].ammo = 0;
        } else if (TECHDATA::_weapons[weapon_id].rack != 1) {
            combat->weapons[i].ammo = TECHDATA::_weapons[weapon_id].base_ammo;
        }

        if (combat->weapons[i].weapon_id == 0x21 /* 33 */) {
            combat->weapons[i].ammo *= combat->weapons[i].count;
        }

        combat->weapons[i].status = 0;
    }

    for (int i = 0; i < 8; ++i) {
        combat->weapon_ready_flags[i] = 1;
    }

    combat->crew_quality = ship->crew_quality;

    int16_t officer_idx = ship->officer_index;
    if (officer_idx < 0 || MOX::_leaders[officer_idx].eta != 0) {
        combat->officer_idx = -1;
    } else {
        combat->officer_idx = officer_idx;
    }

    if (ship->d.ship_type == 0 && ship->owner < 9) {
        combat->marine_count = TECHDATA::_hull_data[combat->size_class].marine_count;
        if (struct_::Test_Bit_Field_(combat->special_device_flags, 0x24 /* 36 */) == 1 &&
            struct_::Test_Bit_Field_(combat->special_device_damage_flags, 0x24 /* 36 */) == 0) {
            combat->marine_count *= 2;
        }
    } else {
        combat->marine_count = -1;
    }

    combat->structure_max = INITSHIP::Get_Ship_Structure_(ship_idx);
    combat->armor_remaining = INITSHIP::Get_Ship_Armor_Hits_(ship_idx) - ship->armor_damage;
    combat->armor_type = ship->d.armor_type;
    combat->structure_damage = ship->structural_damage;

    if (combat->structure_damage > combat->structure_max) {
        combat->structure_damage = combat->structure_max - 1;
    }

    if (combat->size_class == 5 && combat->owner < 8) {
        combat->picture_num = 43 /* 0x2b */;
    } else {
        combat->picture_num = ship->d.picture_num;
    }

    combat->is_retreating = 0;
    CMBTFIR2::Init_Internal_Space_(combat);

    combat->shield_current_hits = (int16_t)((100 - ship->shield_damage_percent) * combat->shield_max_hits / 100);
    combat->drive_current_hits = (int16_t)((100 - ship->drive_damage_percent) * combat->drive_max_hits / 100);
    combat->computer_current_hits = combat->computer_max_hits - ship->computer_damage;

    combat->shield_arc_max = Calc_Combat_Shield_Max_(combat_idx);
    for (int i = 0; i < 4; ++i) {
        combat->shield_arc_current[i] = combat->shield_arc_max;
    }

    if (combat->owner > 8 && combat->drive_destroyed == 0) {
        switch (combat->owner) {
            case 9:
            case 11:
                combat->drive_destroyed = 4;
                break;
            case 10:
            case 14:
                combat->drive_destroyed = 2;
                break;
            case 12:
            case 13:
                combat->drive_destroyed = 6;
                break;
            default:
                break;
        }
    }

    Init_Special_Devices_(combat_idx);
}

int16_t __cdecl Calc_Combat_Shield_Max_(int16_t ship_idx) {
    int16_t max_shield = 0;
    s_combat_data* ship = &MOX::_combat_data[ship_idx];
    int8_t shield_type = ship->shield_type;

    if (shield_type > 0) {
        int16_t multiplier;
        int8_t size_class = ship->size_class;

        if (size_class < 5) {
            multiplier = (int16_t)size_class + 1;
        } else {
            multiplier = (int16_t)size_class + 2;
        }

        // Calculation uses base shield blocking points multiplied by ship size factor and global multiplier 5
        max_shield = TECHDATA::_shields[shield_type].points_blocked * multiplier * 5;

        // Shields are suppressed in a nebula unless the ship has specific warp/shield stabilization (Special 13)
        if (MOX::_in_nebula_flag != 0) {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 13 /* 0x0d */) == 0) {
                max_shield = 0;
            }
        }

        // Shield multipliers or hardening (Special 20) increase capacity by 50%
        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 20 /* 0x14 */) != 0) {
            max_shield = (max_shield * 3) / 2;
        }
    }

    return max_shield;
}

int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id) {
    int16_t result = 0;

    if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_flags, special_id) == 1) {
        if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_damage_flags, special_id) == 0) {
            result = 1;
        }
    }

    return result;
}

int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int32_t retreat_flag) {
    bool skip_combat = false;
    int16_t has_tractor_beam = 0;
    int16_t has_plasma_web = 0;
    int16_t has_cloaking = 0;
    int16_t has_special_26 = 0;
    int16_t has_bhg = 0;
    int16_t has_stellar_converter = 0;
    int16_t colony_pop = 0;
    int16_t mouse_list[6] = {0, 0, 0, 0, 0x27F, 0x1DF};
    void* auto_func;
    void* glass_remap_block;
    char msg_buf[80];

    int16_t combat_winner = 0;

    if (star_idx == MOX::_NUM_STARS) {
        ERIC::Play_Dimensional_Portal_Flic_();
    }

    MOX::_ai_retreat_flag = (int16_t)retreat_flag;
    MOX::_combat_turn = 0;
    MOX::_combat_clock_frame = 15;
    MOX::_bio_weapon_flag = 0;
    MOX::_last_ship_status = -2;
    MOX::_cur_combat_x = 0;
    MOX::_cur_combat_y = 0;
    MOX::_ship_status_display = 0;
    MOX::_dont_draw_ship = -1;
    MOX::_draw_active_missiles = 1;
    MOX::_order_info_flag = 1;
    MOX::_super_fast_flag = 0;
    MOX::_real_attack_flag = 0;

    MOX::_shields_flag = MOX::_settings.combat_shields_arc_flag;
    MOX::_speedx2_flag = MOX::_settings.combat_speedx2_flag;
    MOX::_saved_msl_warning = MOX::_settings.combat_missile_impact_flag;
    MOX::_msl_impact_warning_flag = MOX::_settings.combat_missile_impact_flag;
    MOX::_draw_legal_boxes_flag = MOX::_settings.combat_legal_moves_flag;
    MOX::_grid_flag = MOX::_settings.combat_grid_flag;
    MOX::_combat_init = MOX::_settings.ship_initiative;

    MOX::_take_control_flag = 0;
    MOX::_apply_damage_to_planet = 1;
    MOX::_missile_warning = 0;
    MOX::_ship_frame = 0;
    MOX::_combat_far_x = 0;
    MOX::_combat_mid_x = 0;
    MOX::_combat_near_x = 0;
    MOX::_combat_far_y = 0;
    MOX::_combat_mid_y = 0;
    MOX::_combat_near_y = 0;
    MOX::_net_flag = 0;
    MOX::_combat_total_ship_count = 1;
    MOX::_cmbt_colony_id = colony_idx;

    MOX::_combat_data = reinterpret_cast<s_combat_data*>(Allocate_First_Block_(MOX::_screen_seg, 0x100C2));
    struct_::Clear_Structure_(MOX::_combat_data, 0x100C2);

    MOX::_combat_attacker_player = MOX::_ship[plr1_ships[0]].owner;

    if (star_idx == MOX::_NUM_STARS) {
        MOX::_combat_defender_player = 8;
        colony_pop = 0;
    } else if (MOX::_cmbt_colony_id == -1) {
        MOX::_combat_defender_player = MOX::_ship[plr2_ships[0]].owner;
        colony_pop = 0;
    } else {
        MOX::_combat_defender_player = MOX::_colony[MOX::_cmbt_colony_id].owner;
        colony_pop = MOX::_colony[MOX::_cmbt_colony_id].n_pops;
    }

    if (star_idx == MOX::_NUM_STARS) {
        MOX::_in_nebula_flag = 0;
    } else {
        MOX::_in_nebula_flag = GEO::Point_Is_In_Some_Nebula_(MOX::_star[star_idx].x, MOX::_star[star_idx].y) ? 1 : 0;
    }

    _qsort(plr1_ships, plr1_ship_count, 2, (int(__cdecl*)(const void*, const void*))qsort_ship_array_by_size_);

    int16_t loaded_ships_1 = 0;
    for (int16_t i = 0; i < plr1_ship_count; ++i) {
        if (MOX::_ship[plr1_ships[i]].d.ship_type == 0 && loaded_ships_1 < 100) {
            Load_Combat_Ship_(plr1_ships[i], MOX::_combat_total_ship_count);

            for (int j = 0; j < 8; ++j) {
                switch (MOX::_combat_data[MOX::_combat_total_ship_count].weapons[j].weapon_id) {
                    case 35: // 0x23
                    case 45: // 0x2D
                        has_plasma_web = 1;
                        break;
                    case 37: // 0x25
                        has_bhg = 1;
                        break;
                    case 38: // 0x26
                        has_stellar_converter = 1;
                        break;
                    case 39: // 0x27
                        has_tractor_beam = 1;
                        break;
                }
            }

            if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 23) == 0) {
                MOX::_real_attack_flag = 1;
                if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 6) != 0) {
                    has_cloaking = 1;
                }
            } else {
                has_cloaking = 1;
            }

            Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 10);
            if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 26) != 0) {
                has_special_26 = 1;
            }

            MOX::_combat_total_ship_count++;
            loaded_ships_1++;
        }
        MOX::Mox_Update_();
    }

    if (MOX::_combat_defender_player < 8 && star_idx < MOX::_NUM_STARS) {
        if (HAROLD::Player_Has_Artemis_Net_(&MOX::_player[MOX::_combat_defender_player], &MOX::_star[star_idx])) {
            skip_combat = CMBTFIR2::Apply_Combat_Artemis_Net_(0) != 0;
            if (skip_combat == 0) {
                KEN::Ken_Get_Text_Message_(0x53, msg_buf, 80);
            } else {
                KEN::Ken_Get_Text_Message_(0x52, msg_buf, 80);
            }
            palette::Refresh_Palette_();
            GENDRAW::Message_Box_(msg_buf);
            combat_winner = (int16_t)0xFFFF;
        }
    }

    bool has_defender_ships = false;
    if (star_idx == MOX::_NUM_STARS) {
        has_defender_ships = true;
    }

    for (int16_t i = 0; i < plr2_ship_count; ++i) {
        if (MOX::_ship[plr2_ships[i]].d.ship_type == 0) {
            has_defender_ships = true;
            break;
        }
    }

    if (!has_defender_ships) {
        if (!skip_combat) {
            if (star_idx != MOX::_NUM_STARS) {
                if (MOX::_cmbt_colony_id == -1 || Colony_Has_Defenses_(colony_idx) == 0) {
                    skip_combat = true;
                }
            }
        }
    }

    if (!skip_combat) {
        MOX2::Get_Auto_Function_(&auto_func);
        graphics::Reset_Window_();
        JIM::Play_Combat_Music_();
        MOX::Mox_Update_();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        MAINSCR::Set_Reload_Fleet_Box_Palette_Flag_();
        ERIC::Fast_Fade_Out_();
        fonts::Load_Palette_(3, 0, 0xFF);
        ERIC::Set_Fade_Percent_(0);

        MOX::_missile = reinterpret_cast<s_missile*>(MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 7800));
        struct_::Clear_Structure_(MOX::_missile, 7800);
        MOX::Mox_Update_();

        MOX::_level_symbol_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x2A, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_scan_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 1, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_board_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x33, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_auto_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x32, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_flee_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x35, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_cmbt_done_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x36, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_wait_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x34, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_cmbt_info_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 10, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_cmbt_option_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 2, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_self_destruct_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 3, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_weapons_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 7, MOX::_screen_seg);
        MOX::Mox_Update_();

        for (int16_t i = 0; i < 0x70; ++i) {
            MOX::_msl_seg[i] = MOX::farload::Far_Reload_Next_(s_cmbtmisl_lbx_00551234, i, MOX::_screen_seg);
            MOX::Mox_Update_();
        }

        MOX::_combat_mouse_grid = reinterpret_cast<int16_t*>(MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x2928));
        MOX::_specials_button_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 8, MOX::_screen_seg);
        MOX::_msl_warning_on_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 4, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_warning_missile_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x25, MOX::_screen_seg);
        MOX::_warning_fighter_seg = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x26, MOX::_screen_seg);
        MOX::Mox_Update_();

        for (int16_t i = 0; i < 3; ++i) {
            MOX::_ship_cursor[i] = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, i + 0x20, MOX::_screen_seg);
            MOX::Mox_Update_();
        }

        MOX::_ship_bitmap = MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x62, 0x62));
        MOX::_draw_ship_bitmap = (uint32_t)MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x62, 0x62));
        MOX::_combat_bitmap = (uint32_t)MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x62, 0x62));
        MOX::Mox_Update_();

        MOX::_shield_div_bitmap[0] = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0xE, MOX::_screen_seg);
        MOX::_shield_div_bitmap[1] = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0xF, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::_shield_div_bitmap[2] = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0xC, MOX::_screen_seg);
        MOX::_shield_div_bitmap[3] = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0xD, MOX::_screen_seg);
        MOX::Mox_Update_();

        for (int16_t i = 0; i < 4; ++i) {
            for (int16_t j = 0; j < 4; ++j) {
                MOX::_shield_mask_bitmap[j + i * 4] = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, j + i * 4 + 0x10, MOX::_screen_seg);
            }
            MOX::Mox_Update_();
        }

        MOX::_combat_mouse_grid[2] = 0;
        MOX::_combat_mouse_grid[4] = 0x27F;
        MOX::_combat_mouse_grid[3] = 0;
        MOX::_combat_mouse_grid[5] = 0x15E;
        MOX::_combat_mouse_grid[1] = 12;
        MOX::_combat_mouse_grid[0] = 2;

        int16_t grid_y = 0;
        int16_t grid_idx = 1;
        for (int16_t row = 0; row < 32; ++row) {
            int16_t grid_x = 0;
            for (int16_t col = 0; col < 18; ++col) {
                MOX::_combat_mouse_grid[grid_idx * 6 + 2] = grid_y;
                MOX::_combat_mouse_grid[grid_idx * 6 + 4] = grid_y + 19;
                MOX::_combat_mouse_grid[grid_idx * 6 + 3] = grid_x;
                MOX::_combat_mouse_grid[grid_idx * 6 + 5] = grid_x + 19;
                MOX::_combat_mouse_grid[grid_idx * 6 + 1] = 0;
                MOX::_combat_mouse_grid[grid_idx * 6 + 0] = 0;
                grid_x += 20;
                grid_idx++;
            }
            grid_y += 20;
        }

        MOX::_combat_mouse_grid[0x1490] = 0;
        MOX::_combat_mouse_grid[0x1492] = 0x27F;
        MOX::_combat_mouse_grid[0x1491] = 0x15F;
        MOX::_combat_mouse_grid[0x1493] = 0x1DF;
        MOX::_combat_mouse_grid[0x148F] = 0;
        MOX::_combat_mouse_grid[0x148E] = 1;

        if (MOX::_game_type != 0 && MOX::_game_type != 1) {
            if (MOX::_combat_attacker_player < MOX::_NUM_PLAYERS && MOX::_player[MOX::_combat_attacker_player].objectives == 100) {
                if (MOX::_combat_defender_player < MOX::_NUM_PLAYERS && MOX::_player[MOX::_combat_defender_player].objectives == 100) {
                    MOX::_net_flag = 1;
                }
            }
        }

        if (MOX::_combat_attacker_player <= 8) {
            MOX::_fighter_beam_type[MOX::_combat_attacker_player] = INITSHIP::Best_Point_Defense_Weapon_(MOX::_combat_attacker_player);
            MOX::_fighter_bomb_type[MOX::_combat_attacker_player] = INITSHIP::Best_Bomb_Weapon_(MOX::_combat_attacker_player);
        }

        if (MOX::_combat_defender_player <= 8) {
            MOX::_fighter_beam_type[MOX::_combat_defender_player] = INITSHIP::Best_Point_Defense_Weapon_(MOX::_combat_defender_player);
            MOX::_fighter_bomb_type[MOX::_combat_defender_player] = INITSHIP::Best_Bomb_Weapon_(MOX::_combat_defender_player);
        }

        MOX::Mox_Update_();
        glass_remap_block = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        Save_Glass_Remap_Block_(glass_remap_block);
        MOX::Mox_Update_();

        if (star_idx == MOX::_NUM_STARS) {
            has_bhg = 1;
            has_special_26 = 1;
            Load_Antaran_Defense_Fleet_();
            Load_Antaran_Planet_();
        } else {
            _qsort(plr2_ships, plr2_ship_count, 2, (int(__cdecl*)(const void*, const void*))qsort_ship_array_by_size_);
            int16_t loaded_ships_2 = 0;
            for (int16_t i = 0; i < plr2_ship_count; ++i) {
                if (MOX::_ship[plr2_ships[i]].d.ship_type == 0 && loaded_ships_2 < 100) {
                    Load_Combat_Ship_(plr2_ships[i], MOX::_combat_total_ship_count);

                    for (int j = 0; j < 8; ++j) {
                        switch (MOX::_combat_data[MOX::_combat_total_ship_count].weapons[j].weapon_id) {
                            case 35: // 0x23
                            case 45: // 0x2D
                                has_plasma_web = 1;
                                break;
                            case 37: // 0x25
                                has_bhg = 1;
                                break;
                            case 38: // 0x26
                                has_stellar_converter = 1;
                                break;
                            case 39: // 0x27
                                has_tractor_beam = 1;
                                break;
                        }
                    }

                    if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 6) == 0 &&
                        Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 23) == 0) {
                        // Empty
                    } else {
                        has_cloaking = 1;
                    }

                    Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 10);
                    if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 26) != 0) {
                        has_special_26 = 1;
                    }

                    MOX::_combat_total_ship_count++;
                    loaded_ships_2++;
                }
                MOX::Mox_Update_();
            }
            Load_Colony_Defense_(colony_idx);
        }

        if ((Does_Combat_Planet_Have_Defenses_() & 4) != 0) {
            has_stellar_converter = 1;
        }

        MOX::Mox_Update_();
        Load_Combat_Ship_Palette_(MOX::_combat_attacker_player);
        MOX::Mox_Update_();
        Load_Combat_Ship_Palette_(MOX::_combat_defender_player);
        MOX::Mox_Update_();

        if (has_cloaking != 0) {
            MOX::_darken_plasma_map = MOX::farload::Far_Reload_Next_(s_combat_lbx_00551244, 0x31, MOX::_screen_seg);
            MOX::Mox_Update_();
            Create_Ship_Darken_Map_();
        }

        Load_Combat_Beam_Stuff_(has_special_26);

        MOX::_shield_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        MOX::Mox_Update_();
        int16_t shield_c = 23;
        for (int16_t i = 1; i < 16; ++i) {
            Set_Glass_Remap_Color_(i, i * 2 + 13, i * 2 + 30, 63, shield_c);
            shield_c += 3;
        }
        MOX::Mox_Update_();
        palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
        palette::Set_Palette_Changes_(0, 255);
        MOX::Mox_Update_();
        remap::Update_Glass_Remap_Colors_();
        Save_Glass_Remap_Block_(MOX::_shield_remap_colors);
        MOX::Mox_Update_();

        MOX::_pulsar_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        s_animation_header* pulsar_pal = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_cmbtsfx_lbx_00551228, 0x34, MOX::_global_cache_seg));
        animate::Draw_Palette_(pulsar_pal);
        MOX::Mox_Update_();
        int16_t pulsar_c = 53;
        for (int16_t i = 1; i < 16; ++i) {
            Set_Glass_Remap_Color_(i, s__00579591[i * 4], s__00579592[i * 4], s__00579593[i * 4], pulsar_c);
            pulsar_c += 3;
        }
        palette::Set_Palette_Changes_(0, 255);
        remap::Update_Glass_Remap_Colors_();
        Save_Glass_Remap_Block_(MOX::_pulsar_remap_colors);

        MOX::_warp_core_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        s_animation_header* warp_pal = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_sphersfx_lbx_00551218, 0, MOX::_global_cache_seg));
        MOX::Mox_Update_();
        animate::Draw_Palette_(warp_pal);
        int16_t warp_c = 53;
        for (int16_t i = 1; i < 16; ++i) {
            Set_Glass_Remap_Color_(i, s__00579591[i * 4], s__00579592[i * 4], s__00579593[i * 4], warp_c);
            warp_c += 3;
        }
        palette::Set_Palette_Changes_(0, 255);
        remap::Update_Glass_Remap_Colors_();
        Save_Glass_Remap_Block_(MOX::_warp_core_remap_colors);
        MOX::Mox_Update_();

        if (has_tractor_beam != 0) {
            MOX::_small_tractor_seg = MOX::farload::Far_Reload_Next_(s_cmbtsfx_lbx_00551228, 2, MOX::_screen_seg);
            MOX::Mox_Update_();
            MOX::_tractor_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            int16_t tractor_c = 23;
            for (int16_t i = 1; i < 16; ++i) {
                Set_Glass_Remap_Color_(i, s__00579491[(i / 2) * 4], s__00579492[(i / 2) * 4], s__00579493[(i / 2) * 4], tractor_c);
                tractor_c += 3;
            }
            palette::Set_Palette_Changes_(0, 255);
            MOX::Mox_Update_();
            remap::Update_Glass_Remap_Colors_();
            Save_Glass_Remap_Block_(MOX::_tractor_remap_colors);
        }

        MOX::_stasis_bubble_remap_colors = MOX::_shield_remap_colors;

        if (has_bhg != 0) {
            MOX::_bhg_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            s_animation_header* bhg_pal = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_cmbtsfx_lbx_00551228, 0x2E, MOX::_global_cache_seg));
            animate::Draw_Palette_(bhg_pal);
            for (int16_t i = 1; i < 16; ++i) {
                Set_Glass_Remap_Color_(i, s__00579591[i * 4], s__00579592[i * 4], s__00579593[i * 4], i * 2 + 60);
            }
            palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
            palette::Set_Palette_Changes_(0, 255);
            MOX::Mox_Update_();
            remap::Update_Glass_Remap_Colors_();
            Save_Glass_Remap_Block_(MOX::_bhg_remap_colors);
        }

        if (has_plasma_web != 0) {
            MOX::_plasma_web_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            for (int16_t i = 1; i < 16; ++i) {
                int16_t c = (i * 3 + 15) * 2;
                if (c > 100) c = 100;
                Set_Glass_Remap_Color_(i, s__00579611[i * 4], s__00579612[i * 4], s__00579613[i * 4], c);
            }
            MOX::Mox_Update_();
            palette::Set_Palette_Changes_(0, 255);
            remap::Update_Glass_Remap_Colors_();
            Save_Glass_Remap_Block_(MOX::_plasma_web_remap_colors);
        }

        if (has_stellar_converter != 0) {
            MOX::_stellar_converter_remap_colors = Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            MOX::Mox_Update_();
            s_animation_header* stellar_pal = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_cmbtsfx_lbx_00551228, 0x28, MOX::_global_cache_seg));
            animate::Draw_Palette_(stellar_pal);
            int16_t stellar_c = 53;
            for (int16_t i = 1; i < 16; ++i) {
                Set_Glass_Remap_Color_(i, s__00579591[i * 8], s__00579592[i * 8], s__00579593[i * 8], stellar_c);
                stellar_c += 3;
            }
            palette::Set_Palette_Changes_(0, 255);
            MOX::Mox_Update_();
            remap::Update_Glass_Remap_Colors_();
            Save_Glass_Remap_Block_(MOX::_stellar_converter_remap_colors);
            palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
        }

        Set_Combat_Ordnance_Values_();
        MOX::Mox_Update_();
        Load_Individual_Ship_Pictures_(MOX::_combat_attacker_player);
        MOX::Mox_Update_();
        Load_Individual_Ship_Pictures_(MOX::_combat_defender_player);
        MOX::Mox_Update_();
        Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            Init_Special_Devices_(i);
        }

        Deploy_Ships_();
        MOX::Mox_Update_();

        MOX::_auto_combat_flag[0] = 0;
        MOX::_auto_combat_flag[1] = 0;
        MOX::_skan_flag = 0;

        COMBAT1::Add_Tactical_Combat_Fields_();
        MOX::Mox_Update_();

        combat_winner = Check_For_Winner_();
        fields::Assign_Auto_Function_(CMBTDRW1::Draw_Main_Combat_Screen_, 1);
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);

        int16_t turn_plr1 = MOX::_combat_attacker_player;
        int16_t turn_plr2 = MOX::_combat_defender_player;
        if (MOX::_combat_defender_player == 8) {
            turn_plr1 = 8;
            turn_plr2 = MOX::_combat_attacker_player;
        }

        if (MOX::_net_flag != 0) {
            struct_::Clear_Structure_(&MOX::_new_message, sizeof(MOX::_new_message));
            int16_t net_plr = MOX::_combat_attacker_player;
            if (MOX::_PLAYER_NUM == MOX::_combat_attacker_player) {
                net_plr = MOX::_combat_defender_player;
            }
            NETMOX::Mox_Send_Message_(MOX::_player[net_plr].network_player_id, 1005, &MOX::_combat_init, 2);
            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(0);
        }
        
        MOX::Mox_Update_();

        while (combat_winner == 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (i != 0 || MOX::_cmbt_colony_id != -1) {
                    MOX::_combat_data[i].current_speed = Calc_Current_Speed_(i);
                    MOX::_combat_data[i].tac_val_attack = Offensive_Combat_Bonus_(i);
                    MOX::_combat_data[i].tac_val_defense = Defensive_Combat_Bonus_(i, 0);
                    MOX::_combat_data[i].tac_val_evade = Defensive_Combat_Bonus_(i, 1);
                }
            }
            MOX::Mox_Update_();

            for (int16_t i = 0; i < 300; ++i) {
                if (MOX::_missile[i].type > 0) {
                    if (MOX::_missile[i].travel_dist <= 0) {
                        MOX::_missile[i].travel_dist = MOX::_missile[i].speed * 20;
                    }
                    MOX::_missile[i].is_active = 1;
                }
            }
            MOX::Mox_Update_();

            MOX::_fleet_target = -1;

            if (MOX::_combat_init == 0) {
                COMBAT1::Do_Combat_Turn_(turn_plr1, 0);
                Check_For_Winner_();
                Plasma_Web_Damage_(turn_plr2);
                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();

                combat_winner = Check_For_Winner_();
                if (combat_winner == 0) {
                    MOX::_fleet_target = -1;
                    COMBAT1::Do_Combat_Turn_(turn_plr2, 0);
                    Check_For_Winner_();
                }

                Plasma_Web_Damage_(turn_plr1);
                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();

                combat_winner = Check_For_Winner_();
                if (combat_winner == 0) {
                    MOX::_fleet_target = -1;
                    COMBAT1::Do_Combat_Turn_(turn_plr1, 1);
                    Increment_Bhg_(turn_plr1);
                }

                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();

                combat_winner = Check_For_Winner_();
                if (combat_winner == 0) {
                    MOX::_fleet_target = -1;
                    COMBAT1::Do_Combat_Turn_(turn_plr2, 1);
                    Increment_Bhg_(turn_plr2);
                }

                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();
                combat_winner = Check_For_Winner_();
            } else {
                COMBAT1::Do_Combat_Turn_(0, 0);
                Plasma_Web_Damage_(turn_plr1);
                Plasma_Web_Damage_(turn_plr2);
                COMBAT1::Do_Combat_Turn_(0, 1);
                combat_winner = Check_For_Winner_();
                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();
            }

            MOX::_combat_turn++;
            End_Of_Turn_Bookeeping_();
        }

        if (MOX::_cmbt_colony_id != -1) {
            colony_pop -= MOX::_colony[MOX::_cmbt_colony_id].n_pops;
        }
    }

    End_Of_Combat_(colony_idx, colony_pop, combat_winner);

    if (combat_winner == 1 || combat_winner == 0x29A) {
        if (MOX::_combat_attacker_player < 8 && MOX::_combat_defender_player >= 10 && MOX::_ship[plr2_ships[0]].d.ftl_type > 0) {
            NETMOX::Russ_Event_Monster_Update_(MOX::_combat_defender_player, -1, MOX::_combat_attacker_player);
        }
    }

    if (combat_winner == -1 || combat_winner == 0x29A) {
        if (MOX::_combat_defender_player < 8 && MOX::_combat_attacker_player >= 10 && MOX::_ship[plr1_ships[0]].d.ftl_type > 0) {
            NETMOX::Russ_Event_Monster_Update_(MOX::_combat_attacker_player, -1, MOX::_combat_defender_player);
        }
    }

    if (!skip_combat) {
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        fields::Deactivate_Help_List_();
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        ERIC::Fast_Fade_Out_();
        palette::Set_Palette_Gradient_(160, 95, 0, 0, 0, 0, 0, 0);
        MOX2::Put_Auto_Function_(&auto_func);
        sound::Fadeout_Sound_(-1, 30);
        JIM::Play_Background_Music_();
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
        Restore_Glass_Remap_Block_(glass_remap_block);
        MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
    }

    MAINSCR::Allocate_First_Screen_Seg_(5);

    MOX::_settings.combat_shields_arc_flag = static_cast<char>(MOX::_shields_flag);
    MOX::_settings.combat_speedx2_flag = static_cast<char>(MOX::_speedx2_flag);
    MOX::_settings.combat_missile_impact_flag = static_cast<char>(MOX::_msl_impact_warning_flag);
    MOX::_settings.combat_legal_moves_flag = static_cast<char>(MOX::_draw_legal_boxes_flag);
    MOX::_settings.combat_grid_flag = static_cast<char>(MOX::_grid_flag);

    if (combat_winner == 0x29A) {
        combat_winner = 0;
    }
    
    MOX::_FULL_DRAW_SCREEN_FLAG = MOX::_net_flag;

    return combat_winner;
}

}