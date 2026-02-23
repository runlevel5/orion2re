#include "deps.h"

namespace aidesign {
#pragma pack(push, 1)
    struct s_spcl_item {
        int16_t id;
        int16_t type; // 0 = Special Device, non-zero = Weapon
    };
#pragma pack(pop)

    int16_t __cdecl Add_Specials_To_Design_(struct s_spcl* list, int count, int16_t player_idx, struct s_ship_design* design, int16_t space_available, int16_t* weapon_slot_idx_ptr) {
        int16_t total_space_used = 0;
        int16_t already_has_fighter_bay = 0;
        s_spcl_item* spcl = reinterpret_cast<s_spcl_item*>(list);

        for (int i = 0; i < count; ++i) {
            int16_t item_id = spcl[i].id;

            if (spcl[i].type == 0) {
                // Processing Special Device
                int16_t tech_app_id = TECHDATA::_specials[item_id].tech_application_id;

                if (MOX::_player[player_idx].tech_applications[tech_app_id] == 3 /* LEARNED */) {
                    if (Special_Legal_For_Design_(design, item_id)) {
                        // Check if player has Stealth trait, if so skip Stealth Field device
                        if (MOX::_player[player_idx].traits[TRAIT_STEALTHY_SHIPS] == 0 || item_id != 31 /* SPCL_STEALTH_FIELD */) {
                            int8_t mini_cat = DESIGN::Miniaturization_Category_(tech_app_id);
                            int16_t tech_lvl = DESIGN::Tech_Level_(player_idx, tech_app_id);
                            int16_t base_space = TECHDATA::_specials[item_id].space_per_ship_type[design->size];
                            int16_t actual_space = (int16_t)DESIGN::Space_Given_M_Level_(base_space, tech_lvl, mini_cat);

                            if (actual_space < space_available) {
                                struct_::Set_Bit_Field_(design->special_device_flags, item_id);
                                total_space_used += actual_space;
                                space_available -= actual_space;
                            }
                        }
                    }
                }
            } else {
                // Processing Weapon
                if (*weapon_slot_idx_ptr < 8) {
                    if (item_id == 0x21 /* WPN_FIGHTER_BAY */) {
                        already_has_fighter_bay = 0;
                        for (int16_t slot = 0; slot < *weapon_slot_idx_ptr; ++slot) {
                            if (design->ship_weapon[slot].type == 0x21 /* WPN_FIGHTER_BAY */) {
                                already_has_fighter_bay = 1;
                                break;
                            }
                        }
                    }

                    int16_t weapon_tech_app_id = TECHDATA::_weapons[item_id].tech_application_id;
                    if (MOX::_player[player_idx].tech_applications[weapon_tech_app_id] == 3 /* LEARNED */ && !already_has_fighter_bay) {
                        int16_t weapon_unit_space = (int16_t)DESIGN::Weapon_Space_(player_idx, item_id, 1, 16 /* ARC_360 */, 1, 0);
                        int16_t max_count = space_available / weapon_unit_space;

                        if (item_id == 0x21 /* WPN_FIGHTER_BAY */) {
                            if (max_count > (int16_t)design->size * 10) {
                                max_count = (int16_t)design->size * 10;
                            }
                        }

                        if (max_count > 0) {
                            int16_t items_this_type = 0;

                            // Split weapons into multiple slots if count > 99
                            while (max_count > 0 && *weapon_slot_idx_ptr < 8) {
                                int16_t count_in_slot = (max_count > 99) ? 99 : max_count;

                                design->ship_weapon[*weapon_slot_idx_ptr].type = item_id;
                                design->ship_weapon[*weapon_slot_idx_ptr].count = (int8_t)count_in_slot;
                                design->ship_weapon[*weapon_slot_idx_ptr].current_count = (int8_t)count_in_slot;
                                design->ship_weapon[*weapon_slot_idx_ptr].firing_arc = 16;
                                design->ship_weapon[*weapon_slot_idx_ptr].specials = 0;
                                design->ship_weapon[*weapon_slot_idx_ptr].ammo = 1;

                                items_this_type += count_in_slot;
                                max_count -= count_in_slot;
                                (*weapon_slot_idx_ptr)++;
                            }

                            int16_t space_calc = (int16_t)DESIGN::Weapon_Space_(player_idx, item_id, 1, 16 /* ARC_360 */, 1, 0);
                            total_space_used += space_calc * items_this_type;
                            space_available -= space_calc * items_this_type;
                        }
                    }
                }
            }
        }

        return total_space_used;
    }

    int16_t __cdecl Add_Beam_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space, int16_t weapon_class) {
        int16_t types[4] = {0};
        int16_t specials[4] = {0};
        int16_t arcs[4] = {0};
        int16_t total_added = 0;

        Determine_Beam_Weapon_Modifications_(
            player_idx,
            static_cast<int16_t>(design->size),
            &types[0], &specials[0], &arcs[0],
            &types[1], &specials[1], &arcs[1],
            &types[2], &specials[2], &arcs[2]
        );

        specials[0] |= 0x4;
        specials[2] |= 0x2;

        int16_t weapon_type = types[weapon_class];
        int16_t weapon_specials = specials[weapon_class];
        int16_t weapon_arc = arcs[weapon_class];

        int32_t unit_space = DESIGN::Weapon_Space_(player_idx, weapon_type, 1, weapon_specials, (uint16_t)weapon_arc, (uint16_t)design->size);
        if (unit_space == 0) {
            return 0;
        }

        int16_t quantity = static_cast<int16_t>(space / unit_space);

        if (quantity <= 0 || *weapon_index_ptr >= 8 || weapon_type == 0) {
            return 0;
        }

        design->ship_weapon[*weapon_index_ptr].type = weapon_type;
        design->ship_weapon[*weapon_index_ptr].firing_arc = static_cast<int8_t>(weapon_arc);
        design->ship_weapon[*weapon_index_ptr].specials = weapon_specials;
        design->ship_weapon[*weapon_index_ptr].ammo = 0;

        while (quantity > 99) {
            if (*weapon_index_ptr >= 8) {
                break;
            }

            design->ship_weapon[*weapon_index_ptr].count = 99;
            design->ship_weapon[*weapon_index_ptr].current_count = 99;
            total_added += 99;
            quantity -= 99;

            (*weapon_index_ptr)++;

            if (*weapon_index_ptr < 8) {
                design->ship_weapon[*weapon_index_ptr].type = weapon_type;
                design->ship_weapon[*weapon_index_ptr].firing_arc = static_cast<int8_t>(weapon_arc);
                design->ship_weapon[*weapon_index_ptr].specials = weapon_specials;
                design->ship_weapon[*weapon_index_ptr].ammo = 0;
            }
        }

        if (*weapon_index_ptr < 8) {
            design->ship_weapon[*weapon_index_ptr].count = static_cast<int8_t>(quantity);
            design->ship_weapon[*weapon_index_ptr].current_count = static_cast<int8_t>(quantity);
            total_added += quantity;
        }

        (*weapon_index_ptr)++;

        int32_t final_unit_space = DESIGN::Weapon_Space_(player_idx, weapon_type, 1, weapon_specials, (uint16_t)weapon_arc, (uint16_t)design->size);
        return static_cast<int16_t>(final_unit_space * total_added);
    }

    int16_t __cdecl Get_Ship_Class_(int16_t player_idx) {
        int16_t fighters_avail = Fighters_Available_(player_idx);
        int16_t special_weapons_avail = Special_Weapons_Available_(player_idx);
        int16_t special_devices_avail = Special_Devices_Available_(player_idx);

        if (MOX::_cur_combat_x == -1000) {
            return 7;
        }

        s_player* player = &MOX::_player[player_idx];

        auto get_default_class = [&]() -> int16_t {
            return (player->tech_applications[TECH_APP_NUCLEAR_DRIVE] == 3) ? 0 : 3;
        };

        auto get_fighters_fallback = [&]() -> int16_t {
            return (fighters_avail >= 1) ? 1 : 2;
        };

        int16_t theme = player->ship_design_theme;
        int16_t r = random::Random_(20);

        switch (theme) {
            case 0:
            case 100:
                if (r <= 13) return get_default_class();
                if (r == 14) return 3;
                if (r == 15) return fighters_avail ? 4 : get_default_class();
                if (r == 16) return fighters_avail ? 1 : get_default_class();
                if (r == 17) return 2;
                if (r <= 19) return special_weapons_avail ? 6 : get_default_class();
                return special_devices_avail ? 5 : get_default_class();

            case 1:
                if (r == 1) return get_default_class();
                if (r == 2) return 3;
                if (r <= 15) return fighters_avail ? 4 : get_default_class();
                if (r <= 18) return get_fighters_fallback();
                if (r == 19) return special_weapons_avail ? 6 : get_fighters_fallback();
                return special_devices_avail ? 5 : get_fighters_fallback();

            case 2:
                if (r == 1) return get_default_class();
                if (r <= 14) return 3;
                if (r == 15) return fighters_avail ? 4 : 3;
                if (r <= 18) return 2;
                if (r == 19) return special_weapons_avail ? 6 : 2;
                return special_devices_avail ? 5 : 2;

            case 3:
                if (r == 1) return get_default_class();
                if (r == 2) return 3;
                if (r == 3) return fighters_avail ? 4 : get_default_class();
                if (r <= 5) return get_fighters_fallback();
                if (r <= 7) return 2;
                if (r <= 11) return special_weapons_avail ? 6 : get_fighters_fallback();
                if (r <= 15) return special_weapons_avail ? 6 : 2;
                if (r <= 17) return special_devices_avail ? 5 : get_fighters_fallback();
                return special_devices_avail ? 5 : 2;

            default:
                return 0;
        }
    }

    uint16_t __cdecl Get_Competition_Player_Tech_Values_(uint32_t player_idx, uint32_t tech_type) {
        uint16_t max_ai_group = 0;
        int16_t i = 0;

        do {
            auto& app = TECHDATA::_technology_applications[i];

            if (app.ai_group == (uint8_t)tech_type) {
                if (MOX::_player[player_idx].tech_applications[i] == 3) {
                    int16_t field_id = app.tech_field_id;
                    uint8_t field_group = TECHDATA::_technology_fields[field_id].ai_group;

                    if (max_ai_group < (uint16_t)field_group) {
                        max_ai_group = (uint16_t)field_group;
                    }
                }
            }
            i++;
        } while (i < 212);

        return max_ai_group;
    }

    bool __cdecl Human_Player_Best_Weapons_Are_Beams_(void) {
        uint32_t player_idx = 0;

        while (MOX::_player[player_idx].eliminated != 0) {
            player_idx++;
        }

        uint16_t beam_tech_value = Get_Competition_Player_Tech_Values_(player_idx, 0x15);
        uint16_t comparison_tech_value = Get_Competition_Player_Tech_Values_(player_idx, 0x1a);

        return beam_tech_value <= comparison_tech_value;
    }

    int16_t __cdecl Add_Theme_Special_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t space, int16_t* weapon_index_ptr) {
        int16_t result = 0;
        uint8_t theme = (uint8_t)MOX::_player[player_idx].ship_special_theme;

        switch (theme) {
            case 0:
                result = Add_Specials_To_Design_(_beam_defense_spcl, 4, player_idx, design, space, weapon_index_ptr);
                break;
            case 1:
                result = Add_Specials_To_Design_(_capture_spcl, 5, player_idx, design, space, weapon_index_ptr);
                break;
            case 2:
                result = Add_Specials_To_Design_(_bio_weapon_spcl, 5, player_idx, design, space, weapon_index_ptr);
                break;
            case 3:
                result = Add_Specials_To_Design_(_missile_defense_spcl, 8, player_idx, design, space, weapon_index_ptr);
                break;
            case 4:
                result = Add_Specials_To_Design_(_cloaking_spcl, 3, player_idx, design, space, weapon_index_ptr);
                break;
            case 5:
                result = Add_Specials_To_Design_(_armor_spcl, 3, player_idx, design, space, weapon_index_ptr);
                break;
            case 6:
                result = Add_Specials_To_Design_(_shield_spcl, 3, player_idx, design, space, weapon_index_ptr);
                break;
            case 100:
                result = Add_Specials_To_Design_(_missile_defense_spcl, 8, player_idx, design, space, weapon_index_ptr);
                break;
            default:
                break;
        }

        return result;
    }

    int16_t __cdecl Special_Weapons_Available_(int16_t player_idx) {
        auto& player = MOX::_player[player_idx];

        if (player.tech_applications[TECH_APP_GYRO_DESTABILIZER] == 3 ||
            player.tech_applications[TECH_APP_ENERGY_ABSORBER] == 3 ||
            player.tech_applications[TECH_APP_BLACK_HOLE_GENERATOR] == 3 ||
            player.tech_applications[TECH_APP_STELLAR_CONVERTER] == 3 ||
            player.tech_applications[TECH_APP_PLASMA_WEB] == 3) {
            return 1;
        }

        return 0;
    }

    int16_t __cdecl Special_Devices_Available_(int16_t player_idx) {
        int16_t result = 0;

        for (int i = 0; i < 12; ++i) {
            if (result != 0) {
                break;
            }

            int tech_app_id;
            int id_lookup = *reinterpret_cast<const int16_t*>(&_special_device_spcl[i].id);
            int16_t type_lookup = *reinterpret_cast<const int16_t*>(&_special_device_spcl[i].type);

            if (type_lookup == 0) {
                tech_app_id = TECHDATA::_specials[id_lookup].tech_application_id;
            } else {
                tech_app_id = TECHDATA::_weapons[id_lookup].tech_application_id;
            }

            if (MOX::_player[player_idx].tech_applications[tech_app_id] == 3) {
                result = 1;
            }
        }

        return result;
    }

    int16_t __cdecl Fighters_Available_(int16_t player_idx) {
        const auto& player = MOX::_player[player_idx];

        if (player.tech_applications[TECH_APP_FIGHTER_BAYS] == 3 ||
            player.tech_applications[TECH_APP_BOMBER_BAYS] == 3 ||
            player.tech_applications[TECH_APP_HEAVY_FIGHTER_BAYS] == 3) {
            return 1;
        }

        return 0;
    }

    int16_t __cdecl Add_Fighters_To_Design_(int16_t player_idx, s_ship_design* design, int16_t* weapon_slot_idx, int16_t available_space) {
        int16_t weapon_type;
        int space_per_weapon;
        int16_t total_fighters = 0;
        int16_t total_space_used = 0;

        if (MOX::_player[player_idx].tech_applications[TECH_APP_HEAVY_FIGHTER_BAYS] == 3) {
            space_per_weapon = DESIGN::Weapon_Space_(player_idx, WEAPON_HEAVY_FIGHTER, 1, 0x10, 1, 0);
            if (available_space / space_per_weapon > 0) {
                weapon_type = WEAPON_HEAVY_FIGHTER;
                goto calculate_fighters;
            }
        }

        if (MOX::_player[player_idx].tech_applications[TECH_APP_BOMBER_BAYS] == 3) {
            space_per_weapon = DESIGN::Weapon_Space_(player_idx, WEAPON_BOMBER, 1, 0x10, 1, 0);
            if (available_space / space_per_weapon > 0) {
                weapon_type = WEAPON_BOMBER;
                goto calculate_fighters;
            }
        }

        weapon_type = WEAPON_INTERCEPTOR;

    calculate_fighters:
        space_per_weapon = DESIGN::Weapon_Space_(player_idx, weapon_type, 1, 0x10, 1, 0);
        int num_fighters = (int)available_space / space_per_weapon;

        if (num_fighters > 0 && weapon_type != 0 && *weapon_slot_idx < 8) {
            int16_t current_idx = *weapon_slot_idx;

            design->ship_weapon[current_idx].type = weapon_type;
            design->ship_weapon[current_idx].firing_arc = 0x10;
            design->ship_weapon[current_idx].specials = 0;
            design->ship_weapon[current_idx].ammo = 1;

            while (num_fighters > 20) {
                if (current_idx >= 8) goto finalize_count;

                design->ship_weapon[current_idx].count = 20;
                design->ship_weapon[current_idx].current_count = 20;
                total_fighters += 20;
                num_fighters -= 20;

                current_idx++;
                *weapon_slot_idx = current_idx;

                if (current_idx < 8) {
                    design->ship_weapon[current_idx].type = weapon_type;
                    design->ship_weapon[current_idx].firing_arc = 0x10;
                    design->ship_weapon[current_idx].specials = 0;
                    design->ship_weapon[current_idx].ammo = 1;
                } else {
                    goto finalize_count;
                }
            }

            if (current_idx < 8) {
                design->ship_weapon[current_idx].count = (int8_t)num_fighters;
                design->ship_weapon[current_idx].current_count = (int8_t)num_fighters;
                total_fighters += (int16_t)num_fighters;
            }

        finalize_count:
            *weapon_slot_idx = *weapon_slot_idx + 1;

            int fighter_unit_space = DESIGN::Weapon_Space_(player_idx, WEAPON_INTERCEPTOR, 1, 0x10, 1, 0);
            total_space_used = (int16_t)((int16_t)fighter_unit_space * total_fighters);
        }

        return total_space_used;
    }

    void __cdecl Determine_Bomb_Weapon_(int16_t player_idx, int16_t* out_weapon_idx) {
        int16_t candidates[40];
        int16_t scores[40];
        int num_candidates = 0;

        for (int i = 1; i < 40; ++i) {
            if (TECHDATA::_weapons[i].rack == 3 && (TECHDATA::_weapons[i].flags & 0x80) == 0) {
                int16_t tech_app_id = TECHDATA::_weapons[i].tech_application_id;
                if (MOX::_player[player_idx].tech_applications[tech_app_id] == 3) {
                    candidates[num_candidates] = i;
                    int16_t field_id = TECHDATA::_technology_applications[tech_app_id].tech_field_id;
                    scores[num_candidates] = TECHDATA::_technology_fields[field_id].ai_group;
                    num_candidates++;
                }
            }
        }

        if (num_candidates == 0) {
            *out_weapon_idx = 0;
            return;
        }

        bool swapped;
        do {
            swapped = false;
            for (int i = 0; i < num_candidates - 1; ++i) {
                if (scores[i + 1] < scores[i]) {
                    int16_t temp_score = scores[i];
                    scores[i] = scores[i + 1];
                    scores[i + 1] = temp_score;

                    int16_t temp_cand = candidates[i];
                    candidates[i] = candidates[i + 1];
                    candidates[i + 1] = temp_cand;
                    swapped = true;
                }
            }
        } while (swapped);

        int comp_idx = 0;
        while (MOX::_player[comp_idx].eliminated) {
            comp_idx++;
        }

        uint16_t val_mini = Get_Competition_Player_Tech_Values_(comp_idx, 0x21 /* AI_GROUP_MINIATURIZATION */);
        uint16_t val_ff = Get_Competition_Player_Tech_Values_(comp_idx, 10 /* AI_GROUP_FORCE_FIELDS */);
        int threshold = (val_mini * 2 + 2) / 3 + val_ff;

        int i = 0;
        for (; i < num_candidates; ++i) {
            int16_t tech_app_id = TECHDATA::_weapons[candidates[i]].tech_application_id;
            int16_t min_lvl = DESIGN::Miniaturization_Level_(player_idx, tech_app_id);
            if (min_lvl > 5) min_lvl = 5;

            if (scores[i] + min_lvl >= threshold) {
                break;
            }
        }

        if (i == num_candidates) {
            *out_weapon_idx = candidates[num_candidates - 1];
        } else {
            *out_weapon_idx = candidates[i];
        }
    }

    int16_t __cdecl Add_Bomb_Weapon_To_Design_(int16_t player_idx, s_ship_design* design, int16_t* weapon_slot_idx, int16_t space_available) {
        int16_t bomb_type = 0;
        int16_t total_bombs_added = 0;

        Determine_Bomb_Weapon_(player_idx, &bomb_type);

        int32_t space_per_bomb = DESIGN::Weapon_Space_(player_idx, bomb_type, 1, 1, 0, 0);

        if (space_per_bomb == 0) {
            return 0;
        }

        int16_t num_bombs = (int16_t)(space_available / space_per_bomb);

        if (num_bombs > 0 && *weapon_slot_idx < 8 && bomb_type != 0) {
            design->ship_weapon[*weapon_slot_idx].type = bomb_type;
            design->ship_weapon[*weapon_slot_idx].firing_arc = 16;
            design->ship_weapon[*weapon_slot_idx].specials = 0;
            design->ship_weapon[*weapon_slot_idx].ammo = 10;

            while (num_bombs > 99) {
                if (*weapon_slot_idx >= 8) {
                    break;
                }

                num_bombs -= 99;
                design->ship_weapon[*weapon_slot_idx].count = 99;
                design->ship_weapon[*weapon_slot_idx].current_count = 99;
                total_bombs_added += 99;

                *weapon_slot_idx += 1;

                if (*weapon_slot_idx < 8) {
                    design->ship_weapon[*weapon_slot_idx].type = bomb_type;
                    design->ship_weapon[*weapon_slot_idx].firing_arc = 16;
                    design->ship_weapon[*weapon_slot_idx].specials = 0;
                    design->ship_weapon[*weapon_slot_idx].ammo = 10;
                }
            }

            if (*weapon_slot_idx < 8) {
                total_bombs_added += num_bombs;
                design->ship_weapon[*weapon_slot_idx].count = (int8_t)num_bombs;
                design->ship_weapon[*weapon_slot_idx].current_count = (int8_t)num_bombs;
            }

            *weapon_slot_idx += 1;
        }

        int32_t final_space_per_bomb = DESIGN::Weapon_Space_(player_idx, bomb_type, 1, 1, 0, 0);

        return (int16_t)(final_space_per_bomb * total_bombs_added);
    }

    int16_t __cdecl Add_Missile_Weapon_To_Design_(int16_t player_idx, s_ship_design* design, int16_t* weapon_index_ptr, int16_t space_limit, int16_t ammo) {
        uint16_t weapon_type = 0;
        uint16_t specials = 0;
        int16_t total_missiles_added = 0;
        int16_t modified_ammo = ammo;

        Determine_Missile_Weapon_Modifications_(player_idx, &weapon_type, &specials, (int16_t)design->size, space_limit, &modified_ammo);

        if (weapon_type == 0) {
            return 0;
        }

        int32_t space_per_missile = DESIGN::Weapon_Space_(player_idx, (int16_t)weapon_type, 1, 1, (uint16_t)modified_ammo, specials);
        if (space_per_missile <= 0) {
            return 0;
        }

        int16_t total_possible = (int16_t)(space_limit / space_per_missile);

        if (total_possible <= 0 || *weapon_index_ptr >= 8) {
            return 0;
        }

        int16_t current_idx = *weapon_index_ptr;
        design->ship_weapon[current_idx].type = (int16_t)weapon_type;
        design->ship_weapon[current_idx].firing_arc = 0x10;
        design->ship_weapon[current_idx].specials = (int16_t)specials;
        design->ship_weapon[current_idx].ammo = (int8_t)modified_ammo;

        while (total_possible > 20) {
            if (*weapon_index_ptr >= 8) {
                goto finalize_index;
            }

            current_idx = *weapon_index_ptr;
            design->ship_weapon[current_idx].count = 20;
            design->ship_weapon[current_idx].current_count = 20;
            total_missiles_added += 20;
            total_possible -= 20;

            (*weapon_index_ptr)++;
            current_idx = *weapon_index_ptr;

            if (current_idx < 8) {
                design->ship_weapon[current_idx].type = (int16_t)weapon_type;
                design->ship_weapon[current_idx].firing_arc = 0x10;
                design->ship_weapon[current_idx].specials = (int16_t)specials;
                design->ship_weapon[current_idx].ammo = (int8_t)modified_ammo;
            }
        }

        if (*weapon_index_ptr < 8) {
            current_idx = *weapon_index_ptr;
            total_missiles_added += total_possible;
            design->ship_weapon[current_idx].count = (int8_t)total_possible;
            design->ship_weapon[current_idx].current_count = (int8_t)total_possible;
        }

    finalize_index:
        (*weapon_index_ptr)++;

        int32_t final_space_coeff = DESIGN::Weapon_Space_(player_idx, (int16_t)weapon_type, 1, 1, (uint16_t)modified_ammo, specials);
        return (int16_t)(final_space_coeff * total_missiles_added);
    }

    void __cdecl Auto_Design_Ship_(int16_t player_idx, s_ship_design* design, int16_t hull_idx) {
        int16_t ship_class;
        int16_t remaining_space;
        int16_t weapon_slot_idx = 0;
        int16_t current_budget;
        char temp_name[16];
        s_player* player = &MOX::_player[player_idx];

        ship_class = Get_Ship_Class_(player_idx);

        // Check for Stellar Converter
        if (hull_idx == 5 && (player->tech_applications[TECH_APP_STELLAR_CONVERTER] == 3)) {
            if (random::Random_(4) < 4) {
                ship_class = 6;
            }
        }

        // Check for Black Hole Generator
        if (player->tech_applications[TECH_APP_BLACK_HOLE_GENERATOR] == 3 && hull_idx > 3) {
            if (random::Random_(4) < 4) {
                ship_class = 6;
            }
        }

        struct_::Clear_Structure_(design, 99);

        if (hull_idx == 5) {
            design->picture_num = 0x2b; /* '+' */
            strcpy(design->name, TECHDATA::_hull_data[5].name);
        } else {
            design->picture_num = (int8_t)random::Random_(8) + (int8_t)hull_idx * 8 - 1;
            int16_t player_race = player->race;
            DESIGN::Get_Ship_Design_Name_(player_race, hull_idx, (int16_t)(design->picture_num % 8), design->name);
        }

        if (ship_class == 7) {
            KEN::Ken_Get_Text_Message_(0x54, temp_name, 0xf);
            strcpy(design->name, temp_name);
        }

        design->ship_type = 0;
        design->size = (int8_t)hull_idx;
        design->shield_type = (int8_t)INITSHIP::Best_Ship_Shield_(player_idx);
        design->ftl_type = (int8_t)INITSHIP::Best_Warp_Drive_(player_idx);
        design->speed = (int8_t)INITSHIP::Get_FTL_Speed_(player_idx, (int16_t)design->ftl_type);
        design->computer_type = (int8_t)INITSHIP::Best_Computer_(player_idx);
        design->armor_type = (int8_t)INITSHIP::Best_Armor_(player_idx);

        remaining_space = TECHDATA::_hull_data[hull_idx].hull_size;

        // Check for Battle Pods
        if (player->tech_applications[TECH_APP_BATTLE_PODS] == 3) {
            remaining_space += remaining_space / 2;
            struct_::Set_Bit_Field_(design->special_device_flags, 4);
        }

        remaining_space -= DESIGN::Shield_Space_Given_M_Level_((int16_t)design->shield_type, (int16_t)design->size, player_idx);

        const s_aidesign_ship_design* table = &_ship_design_tables[ship_class][hull_idx];
        current_budget = 0;

        switch (ship_class) {
            case 1:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->fighters != -1) {
                    current_budget += (remaining_space * table->fighters) / 100;
                    current_budget -= Add_Fighters_To_Design_(player_idx, design, &weapon_slot_idx, current_budget);
                }
                if (table->heavy_beams != -1) {
                    current_budget += (remaining_space * table->heavy_beams) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 2);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                break;

            case 2:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->missile_weapon != -1) {
                    current_budget += (remaining_space * table->missile_weapon) / 100;
                    current_budget -= Add_Missile_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 5);
                }
                if (table->heavy_beams != -1) {
                    current_budget += (remaining_space * table->heavy_beams) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 2);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                break;

            case 3:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->missile_special != -1) {
                    current_budget += (remaining_space * table->missile_special) / 100;
                    current_budget -= Add_Specials_To_Design_(_missile_offense_spcl, 2, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->missile_weapon != -1) {
                    current_budget += (remaining_space * table->missile_weapon) / 100;
                    current_budget -= Add_Missile_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 10);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                if (table->point_defense_beams != -1) {
                    current_budget += (remaining_space * table->point_defense_beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 0);
                }
                break;

            case 4:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->fighters != -1) {
                    current_budget += (remaining_space * table->fighters) / 100;
                    current_budget -= Add_Fighters_To_Design_(player_idx, design, &weapon_slot_idx, current_budget);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                if (table->point_defense_beams != -1) {
                    current_budget += (remaining_space * table->point_defense_beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 0);
                }
                break;

            case 5:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->special_device != -1) {
                    current_budget += (remaining_space * table->special_device) / 100;
                    current_budget -= Add_Specials_To_Design_(_special_device_spcl, 12, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->heavy_beams != -1) {
                    current_budget += (remaining_space * table->heavy_beams) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 2);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                break;

            case 6:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->special_weapon != -1) {
                    current_budget += (remaining_space * table->special_weapon) / 100;
                    current_budget -= Add_Specials_To_Design_(_special_weapons_spcl, 5, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                if (table->heavy_beams != -1) {
                    current_budget += (remaining_space * table->heavy_beams) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 2);
                }
                if (table->point_defense_beams != -1) {
                    current_budget += (remaining_space * table->point_defense_beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 0);
                }
                break;

            case 7:
                // Check for Extended Fuel Tanks
                if (player->tech_applications[TECH_APP_EXTENDED_FUEL_TANKS] == 3) {
                    struct_::Set_Bit_Field_(design->special_device_flags, 11);
                    int8_t cat = DESIGN::Miniaturization_Category_(63);
                    int16_t m_level = DESIGN::Miniaturization_Level_(player_idx, 63);
                    int16_t fuel_tank_base_space = TECHDATA::_specials[11].space_per_ship_type[0];
                    remaining_space -= DESIGN::Space_Given_M_Level_(fuel_tank_base_space, m_level, cat);
                }
                /* fallthrough */
            case 0:
                if (table->theme_special != -1) {
                    current_budget = (remaining_space * table->theme_special) / 100;
                    current_budget -= Add_Theme_Special_To_Design_(player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->defense_special != -1) {
                    current_budget += (remaining_space * table->defense_special) / 100;
                    s_spcl* def_tbl = Human_Player_Best_Weapons_Are_Beams_() ? _defense_specials_t_spcl : _defense_specials_f_spcl;
                    current_budget -= Add_Specials_To_Design_(def_tbl, 19, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->beam_special != -1) {
                    current_budget += (remaining_space * table->beam_special) / 100;
                    current_budget -= Add_Specials_To_Design_(_beam_offense_spcl, 7, player_idx, design, current_budget, &weapon_slot_idx);
                }
                if (table->missile_weapon != -1) {
                    current_budget += (remaining_space * table->missile_weapon) / 100;
                    current_budget -= Add_Missile_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 2);
                }
                if (table->heavy_beams != -1) {
                    current_budget += (remaining_space * table->heavy_beams) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 2);
                }
                if (table->beams != -1) {
                    current_budget += (remaining_space * table->beams + 50) / 100;
                    current_budget -= Add_Beam_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget, 1);
                }
                break;
        }

        if (table->bombs != -1) {
            Add_Bomb_Weapon_To_Design_(player_idx, design, &weapon_slot_idx, current_budget + (int16_t)((remaining_space * table->bombs) / 100));
        }

        design->combat_speed = (int8_t)TECHDATA::_drives[design->ftl_type].combat_speed_modifier[hull_idx];
        design->previous_owner = (int8_t)player_idx;
        design->cost = DESIGN::Cost_Of_Design_(player_idx, design);
        design->date_of_design = (uint16_t)MOX::_stardate;
    }

    int16_t __cdecl Cost_Reduction_For_Govt_Hull_Type(int16_t govt_type, s_ship_design* design) {
        int32_t reduced_cost = (TECHDATA::_hull_data[design->size].hull_space * 3) / 2;

        DESIGN::Cost_Reduction_For_Govt_Type_(govt_type, &reduced_cost);

        return (int16_t)reduced_cost;
    }

    int16_t __cdecl Special_Legal_For_Design_(s_ship_design* design, int16_t tech_id) {
        int16_t result = 1;
        int16_t bit = 0;

        while (bit < 40 && result == 1) {
            if (struct_::Test_Bit_Field_(design->special_device_flags, bit)) {
                for (int i = 0; i < 6; ++i) {
                    if (TECHDATA::_specials[bit].field6_0x20[i] == tech_id) {
                        result = 0;
                        break;
                    }
                }
            }
            bit++;
        }

        if (design->shield_type == 0) {
            if (tech_id == 13 || tech_id == 20 || tech_id == 30) {
                result = 0;
            }
        }

        return result;
    }

}