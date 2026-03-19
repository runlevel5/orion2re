#include "pch.h"

namespace INITSHIP {
    void __cdecl Init_Ship_Designs_(int16_t player_idx) {
        s_player* plr = &MOX::_player[player_idx];
        int16_t design_flag = 0;

        if (MOX::_settings.starting_civilization_level != 2 && plr->objectives == 0x64 /* 'd' */) {
            design_flag = 1;
        }

        if (MOX::_settings.strategic_combat_flag == 1) {
            for (int16_t i = 0; i < 6; ++i) {
                struct_::Clear_Structure_(&plr->ship_designs[i], 99);
                Auto_Strategic_Design_Ship_(player_idx, i, &plr->ship_designs[i]);
            }
            return;
        }

        MOX::_cur_combat_x = -1000;
        aidesign::Auto_Design_Ship_(player_idx, &plr->ship_designs[0], 0);
        MOX::_cur_combat_x = 0;

        for (int16_t i = 1; i < 5; ++i) {
            aidesign::Auto_Design_Ship_(player_idx, &plr->ship_designs[i], i - design_flag);
        }

        if (plr->objectives == 0x64 /* 'd' */) {
            struct_::Clear_Structure_(&plr->ship_designs[5], 99);
        } else {
            aidesign::Auto_Design_Ship_(player_idx, &plr->ship_designs[5], 5);

            int16_t pic_mod_8 = (int16_t)(plr->ship_designs[5].picture_num % 8);
            DESIGN::Get_Ship_Design_Name_(
                (int16_t)plr->race,
                (int16_t)plr->ship_designs[5].size,
                pic_mod_8,
                plr->ship_designs[5].name
            );
        }
    }

    void __cdecl Update_Player_Strategic_Ship_Designs_(int16_t player_idx) {
        s_player* player = &MOX::_player[player_idx];

        for (int16_t i = 0; i < 6; ++i) {
            s_ship_design* design = &player->ship_designs[i];
            int8_t old_picture_num = design->picture_num;

            struct_::Clear_Structure_(design, sizeof(s_ship_design));
            Auto_Strategic_Design_Ship_(player_idx, i, design);

            int8_t new_picture_num;
            if (i == 5) {
                new_picture_num = 0x2b; // '+'
            } else {
                new_picture_num = old_picture_num + 1;
                // Each design slot has 8 associated picture indices
                if (new_picture_num == (i * 8 + 8)) {
                    new_picture_num = (int8_t)(i * 8);
                }
            }
            design->picture_num = new_picture_num;
        }
    }

    void __cdecl Repair_Ship_Full_(int16_t ship_idx) {
        s_ship_data* ship = &MOX::_ship[ship_idx];

        ship->structural_damage = 0;
        ship->drive_damage_percent = 0;
        ship->shield_damage_percent = 0;
        ship->computer_damage = 0;
        ship->armor_damage = 0;

        for (int i = 0; i < 8; ++i) {
            ship->d.ship_weapon[i].current_count = ship->d.ship_weapon[i].count;
        }

        for (int i = 0; i < 5; ++i) {
            ship->special_device_damage_flags[i] = 0;
        }
    }

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx) {
        if (MOX::_settings.strategic_combat_flag == 1) {
            Update_Player_Strategic_Ship_Designs_(player_idx);
            return;
        }

        if (MOX::_player[player_idx].objectives == 100 /* 'd' */) {
            Update_Automatic_Design_Equipment_(player_idx);
            Update_Equipment_On_Ships_Being_Built_(player_idx);
            return;
        }

        Update_Equipment_On_Ships_Being_Built_(player_idx);

        for (int16_t i = 0; i < 5; ++i) {
            Auto_Strategic_Design_Ship_(player_idx, i, &MOX::_player[player_idx].ship_designs[i]);
        }
    }

    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx) {
        if (player_idx >= 8) {
            return 5;
        }

        int16_t best_fuel_idx = 0;
        int16_t best_range = TECHDATA::_fuel[0].ship_range;

        for (int16_t i = 0; i < 6; ++i) {
            // Check if player has researched this fuel technology (application value 3)
            // and if it provides a better range than the current best.
            if (MOX::_player[player_idx].tech_applications[TECHDATA::_fuel[i].tech_app] == 3) {
                if (TECHDATA::_fuel[i].ship_range > best_range) {
                    best_range = TECHDATA::_fuel[i].ship_range;
                    best_fuel_idx = i;
                }
            }
        }

        return best_fuel_idx;
    }

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx) {
        if (player_idx > 7) {
            // Handle special players (Guardians, Antarans, etc.)
            switch (player_idx) {
                case 8:  // Antarans
                case 12:
                case 13:
                    return 6;
                case 9:
                case 11:
                    return 4;
                case 10:
                case 14:
                    return 2;
                default:
                    return 0;
            }
        }

        int16_t best_warp_index = 0;
        // Search through warp drive techs 1-6 (Nuclear to Interdimensional)
        for (int16_t i = 1; i < 7; ++i) {
            int16_t app_idx = TECHDATA::_drives[i].tech_app;
            // Status 3 indicates that the technology has been researched
            if (MOX::_player[player_idx].tech_applications[app_idx] == 3) {
                best_warp_index = i;
            }
        }

        return best_warp_index;
    }

    int16_t __cdecl Calc_Player_FTL_Speed_(int16_t player_idx) {
        int16_t drive_idx = Best_Warp_Drive_(player_idx);
        return Get_FTL_Speed_(player_idx, drive_idx);
    }

    int16_t __cdecl Get_FTL_Speed_(int16_t player_idx, int16_t drive_idx) {
        int16_t speed = 0;

        if (player_idx < 8) {
            speed = TECHDATA::_drives[drive_idx].speed;

            if (MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL] != 0) {
                return speed + 2;
            }
        } else if (drive_idx > 0) {
            speed = 1;
        }

        return speed;
    }

    void __cdecl Update_Equipment_On_Ships_Being_Built_(int16_t player_idx) {
        if (MOX::_player[player_idx].eliminated != 0) {
            return;
        }

        int16_t best_armor = Best_Armor_(player_idx);
        int16_t best_ftl = Best_Warp_Drive_(player_idx);

        for (int16_t ship_idx = 0; ship_idx < static_cast<int16_t>(MOX::_NUM_SHIPS); ++ship_idx) {
            s_ship_data* ship = &MOX::_ship[ship_idx];

            if (ship->owner == static_cast<int8_t>(player_idx) && (ship->status == 4 || ship->status == 6)) {
                if (static_cast<int16_t>(ship->d.ftl_type) != best_ftl) {
                    int16_t old_ftl = static_cast<int16_t>(ship->d.ftl_type);
                    int8_t ship_size = ship->d.size;

                    int8_t old_bonus = TECHDATA::_drives[old_ftl].combat_speed_modifier[ship_size];

                    ship->d.ftl_type = static_cast<int8_t>(best_ftl);
                    ship->d.speed = static_cast<int8_t>(Get_FTL_Speed_(player_idx, best_ftl));

                    int8_t new_bonus = TECHDATA::_drives[best_ftl].combat_speed_modifier[ship_size];
                    ship->d.combat_speed = (ship->d.combat_speed - old_bonus) + new_bonus;
                }

                ship->d.armor_type = static_cast<int8_t>(best_armor);
            }
        }
    }

    void __cdecl Update_Automatic_Design_Equipment_(int16_t player_idx) {
        int16_t best_armor_type = Best_Armor_(player_idx);
        int16_t best_ftl_type = Best_Warp_Drive_(player_idx);

        s_player* player = &MOX::_player[player_idx];

        for (int design_idx = 0; design_idx < 6; ++design_idx) {
            s_ship_design* design = &player->ship_designs[design_idx];

            design->armor_type = static_cast<int8_t>(best_armor_type);

            if (static_cast<int16_t>(design->ftl_type) != best_ftl_type) {
                int8_t old_ftl = design->ftl_type;
                int8_t ship_size = design->size;

                // Adjust combat speed based on the bonus difference between old and new drives
                int8_t old_bonus = TECHDATA::_drives[old_ftl].combat_speed_modifier[ship_size];

                design->ftl_type = static_cast<int8_t>(best_ftl_type);
                design->speed = static_cast<int8_t>(Get_FTL_Speed_(player_idx, best_ftl_type));

                int8_t new_bonus = TECHDATA::_drives[best_ftl_type].combat_speed_modifier[ship_size];
                design->combat_speed = (design->combat_speed - old_bonus) + new_bonus;
            }

            design->cost = DESIGN::Cost_Of_Design_(player_idx, design);
        }
    }

    int16_t __cdecl Best_Armor_(int16_t player_idx) {
        if (player_idx > 7) {
            // Antarans or Space Monsters
            if (player_idx == 8 || player_idx == 9) {
                return 6; // Xentronium
            }
            if (player_idx >= 10 && player_idx <= 14) {
                return 0; // No armor?
            }
            return 1; // Default: Titanium
        }

        int16_t best_armor_idx = 1; // Start with Titanium Armor

        // Check for Tritanium (2) through Xentronium (6)
        for (int16_t i = 2; i < 7; ++i) {
            int16_t tech_id = TECHDATA::_armor[i].tech_app_id;

            // 3 indicates the tech application is fully researched/available
            if (MOX::_player[player_idx].tech_applications[tech_id] == 3) {
                best_armor_idx = i;
            }
        }

        return best_armor_idx;
    }

    int16_t __cdecl Best_Computer_(int16_t player_idx) {
        if (player_idx >= 8) {
            return 5;
        }

        int16_t best_computer_idx = 0;

        for (int16_t i = 1; i < 6; ++i) {
            if (MOX::_player[player_idx].tech_applications[TECHDATA::_computers[i].tech_app] == 3) {
                best_computer_idx = i;
            }
        }

        return best_computer_idx;
    }

    int16_t __cdecl Best_Ship_Shield_(int16_t player_idx) {
        int16_t best_shield_idx = 0;

        if (player_idx < 8) {
            for (int16_t i = 1; i < 6; ++i) {
                if (MOX::_player[player_idx].tech_applications[TECHDATA::_shields[i].tech_app_id] == 3) {
                    best_shield_idx = i;
                }
            }
            return best_shield_idx;
        }

        if (player_idx == 8) {
            return 7;
        }

        if (player_idx == 9) {
            return 3;
        }

        return 0;
    }

    // Suggested name: INITSHIP::Best_Shield_Special_
    int16_t __cdecl Best_Shield_Special_(int16_t player_idx) {
        if (player_idx < 8) {
            // Check if player has Hard Shields tech application researched (level 3)
            if (MOX::_player[player_idx].tech_applications[TECH_APP_HARD_SHIELDS] == 3) {
                return 13; /* HARD_SHIELDS_SPECIAL */
            }
        }
        return 0;
    }

    int16_t __cdecl Get_Structure_(int16_t hull_index, bool is_fort, int16_t armor_tech) {
        int16_t armor_idx = Best_Armor_(armor_tech);

        int16_t base_structure = TECHDATA::_hull_data[hull_index].structure;
        int16_t absorption_pct = TECHDATA::_armor[armor_idx].ships_bonus;

        // Formula: base_structure + (absorption_pct * base_structure) / 100
        int32_t bonus = (static_cast<int32_t>(absorption_pct) * static_cast<int32_t>(base_structure)) / 100;
        int16_t total_structure = base_structure + static_cast<int16_t>(bonus);

        if (is_fort) {
            total_structure *= 3;
        }

        return total_structure;
    }

    void __cdecl Auto_Strategic_Design_Ship_(int16_t player_idx, int16_t design_idx, s_ship_design* design) {
        // Copy the default name for this hull type (e.g., "Frigate")
        strcpy(design->name, TECHDATA::_hull_data[design_idx].name);

        design->size = static_cast<int8_t>(design_idx);
        design->ftl_type = static_cast<int8_t>(Best_Warp_Drive_(player_idx));
        design->shield_type = static_cast<int8_t>(Best_Ship_Shield_(player_idx));
        design->computer_type = static_cast<int8_t>(Best_Computer_(player_idx));
        design->armor_type = static_cast<int8_t>(Best_Armor_(player_idx));

        // Calculate the build cost including government bonuses/penalties
        design->cost = Cost_Of_Strategic_Design_(player_idx, &MOX::_player[player_idx].ship_designs[design_idx]);

        // Fill weapons and special systems
        Update_Strategic_Design_Weapons_Specials_(design, player_idx, design_idx);

        // Assign visual representation
        if (design_idx == 5 /* STAR_BASE_HULL */) {
            design->picture_num = 0x2B; /* '+' sign for starbases */
        } else {
            design->picture_num = static_cast<int8_t>(design_idx) << 3;
        }

        design->previous_owner = static_cast<int8_t>(player_idx);
        design->speed = static_cast<int8_t>(Get_FTL_Speed_(player_idx, static_cast<int16_t>(design->ftl_type)));
        design->date_of_design = static_cast<uint16_t>(MOX::_stardate);
    }

    void __cdecl Load_Small_Antaran_Design_(s_ship_design* design) {
        struct_::Clear_Structure_(design, 99);

        strcpy(design->name, s_Raider_0055768c);

        design->ship_type = 0;
        design->shield_type = 0;
        design->size = 0;
        design->computer_type = 5;
        design->speed = 1;

        design->ship_weapon[0].firing_arc = 1;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 2;
        design->ship_weapon[0].current_count = 2;
        design->ship_weapon[0].type = 4;

        design->ship_weapon[1].specials = 0;
        design->ship_weapon[1].ammo = 0;
        design->ship_weapon[1].count = 1;
        design->ship_weapon[1].current_count = 1;
        design->ship_weapon[1].firing_arc = 15;
        design->ship_weapon[1].type = 24;

        design->armor_type = 6;
        design->ftl_type = 5;

        struct_::Set_Bit_Field_(design->special_device_flags, 7);
        struct_::Set_Bit_Field_(design->special_device_flags, 24);

        if (MOX::_settings.game_difficulty > 2) {
            struct_::Set_Bit_Field_(design->special_device_flags, 9);
        }

        design->picture_num = 0;
        design->previous_owner = 8;
        design->cost = 500;

        design->combat_speed = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size];
    }

    void __cdecl Load_Medium_Antaran_Design_(s_ship_design* design) {
        struct_::Clear_Structure_(design, 99);

        strcpy(design->name, s_Marauder_00557694);

        design->ship_type = 0;
        design->shield_type = 0;
        design->size = 1;
        design->ftl_type = 5;
        design->speed = 1;
        design->computer_type = 5;
        design->armor_type = 6;

        // Weapon 0: Beam (Type 4)
        design->ship_weapon[0].type = 4;
        design->ship_weapon[0].count = 3;
        design->ship_weapon[0].current_count = 3;
        design->ship_weapon[0].firing_arc = 1;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].ammo = 0;

        // Weapon 1: Beam (Type 4) with specials (Value 2)
        design->ship_weapon[1].type = 4;
        design->ship_weapon[1].count = 1;
        design->ship_weapon[1].current_count = 1;
        design->ship_weapon[1].firing_arc = 1;
        design->ship_weapon[1].specials = 2;
        design->ship_weapon[1].ammo = 0;

        // Weapon 2: Missile/Special (Type 24)
        design->ship_weapon[2].type = 24;
        design->ship_weapon[2].count = 1;
        design->ship_weapon[2].current_count = 1;
        design->ship_weapon[2].firing_arc = 15;
        design->ship_weapon[2].specials = 0;
        design->ship_weapon[2].ammo = 10;

        struct_::Set_Bit_Field_(design->special_device_flags, 7);
        struct_::Set_Bit_Field_(design->special_device_flags, 24);

        if (MOX::_settings.game_difficulty > 3) {
            struct_::Set_Bit_Field_(design->special_device_flags, 21);
        } else if (MOX::_settings.game_difficulty > 2) {
            struct_::Set_Bit_Field_(design->special_device_flags, 9);
        }

        design->picture_num = 1;
        design->previous_owner = 8;
        design->cost = 500;

        design->combat_speed = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size];
    }

    void __cdecl Load_Huge_Antaran_Design_(s_ship_design* design) {
        struct_::Clear_Structure_(design, 99);

        strcpy(design->name, s_Interdictor_005576ac);

        design->ship_type = 0;
        design->shield_type = 0;
        design->computer_type = 5;

        design->ship_weapon[0].type = 4;
        design->speed = 1;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].firing_arc = 1;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 10;
        design->ship_weapon[0].current_count = 10;

        design->ship_weapon[1].type = 4;
        design->ship_weapon[1].firing_arc = 1;
        design->size = 3;
        design->ship_weapon[1].ammo = 0;
        design->ship_weapon[1].count = 4;
        design->armor_type = 6;
        design->ship_weapon[1].current_count = 4;

        design->ship_weapon[2].firing_arc = 1;
        design->ship_weapon[2].specials = 0;
        design->ship_weapon[2].ammo = 10;
        design->ftl_type = 5;
        design->ship_weapon[2].count = 15;
        design->ship_weapon[2].current_count = 15;

        design->ship_weapon[3].firing_arc = 15;
        design->ship_weapon[3].specials = 0;
        design->ship_weapon[3].ammo = 0;
        design->ship_weapon[3].count = 2;
        design->ship_weapon[3].current_count = 2;

        design->ship_weapon[4].type = 4;
        design->ship_weapon[4].firing_arc = 1;
        design->ship_weapon[4].specials = 4;
        design->ship_weapon[4].ammo = 0;
        design->ship_weapon[4].count = 10;
        design->ship_weapon[4].current_count = 10;

        design->ship_weapon[1].specials = 2;
        design->ship_weapon[5].specials = 0;
        design->ship_weapon[5].firing_arc = 1;
        design->ship_weapon[2].type = 24;
        design->ship_weapon[3].type = 13;
        design->ship_weapon[5].type = 11;
        design->ship_weapon[5].ammo = 0;
        design->ship_weapon[5].count = 4;
        design->ship_weapon[5].current_count = 4;

        struct_::Set_Bit_Field_(design->special_device_flags, 7);
        struct_::Set_Bit_Field_(design->special_device_flags, 24);
        struct_::Set_Bit_Field_(design->special_device_flags, 33);
        struct_::Set_Bit_Field_(design->special_device_flags, 26);

        switch (MOX::_settings.game_difficulty) {
            case 1:
                struct_::Set_Bit_Field_(design->special_device_flags, 9);
                break;
            case 2:
                struct_::Set_Bit_Field_(design->special_device_flags, 21);
                break;
            case 3:
            case 4:
                struct_::Set_Bit_Field_(design->special_device_flags, 38);
                break;
            default:
                break;
        }

        design->picture_num = 3;
        design->previous_owner = 8;
        design->cost = 500;
        design->combat_speed = global::_drives[design->ftl_type].combat_speed_modifier[design->size];
    }

    void __cdecl Load_Large_Antaran_Design_(s_ship_design* design) {
        struct_::Clear_Structure_(design, 99);
        strcpy(reinterpret_cast<char*>(design), global::s_Intruder_005576a0);

        // Indices into the s_ship_design structure based on disassembly offsets
        design->ship_type = 0;
        design->shield_type = 0;
        design->computer_type = 5;
        design->size = 2;
        design->speed = 1;
        design->ftl_type = 5;
        design->armor_type = 6;

        // Weaponry setup - following offsets mapped in disassembly
        design->ship_weapon[0].type = 4;
        design->ship_weapon[0].count = 4;
        design->ship_weapon[0].current_count = 4;
        design->ship_weapon[0].firing_arc = 1;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].ammo = 0;

        design->ship_weapon[1].type = 4;
        design->ship_weapon[1].count = 2;
        design->ship_weapon[1].current_count = 2;
        design->ship_weapon[1].firing_arc = 1;
        design->ship_weapon[1].specials = 2;
        design->ship_weapon[1].ammo = 0;

        design->ship_weapon[2].type = 24 /* 0x18 */;
        design->ship_weapon[2].count = 5;
        design->ship_weapon[2].current_count = 5;
        design->ship_weapon[2].firing_arc = 15 /* 0x0F */;
        design->ship_weapon[2].specials = 0;
        design->ship_weapon[2].ammo = 10 /* 0x0A */;

        design->ship_weapon[3].type = 13 /* 0x0D */;
        design->ship_weapon[3].count = 2;
        design->ship_weapon[3].current_count = 2;
        design->ship_weapon[3].firing_arc = 15 /* 0x0F */;
        design->ship_weapon[3].specials = 0;
        design->ship_weapon[3].ammo = 0;

        design->ship_weapon[4].type = 31 /* 0x1F */;
        design->ship_weapon[4].count = 3;
        design->ship_weapon[4].current_count = 3;
        design->ship_weapon[4].firing_arc = 15 /* 0x0F */;
        design->ship_weapon[4].specials = 0;
        design->ship_weapon[4].ammo = 0;

        struct_::Set_Bit_Field_(design->special_device_flags, 7);
        struct_::Set_Bit_Field_(design->special_device_flags, 24 /* 0x18 */);

        if (MOX::_settings.game_difficulty == 2) {
            struct_::Set_Bit_Field_(design->special_device_flags, 9);
        } else if (MOX::_settings.game_difficulty == 3) {
            struct_::Set_Bit_Field_(design->special_device_flags, 21 /* 0x15 */);
        } else if (MOX::_settings.game_difficulty == 4) {
            struct_::Set_Bit_Field_(design->special_device_flags, 38 /* 0x26 */);
        }

        design->picture_num = 2;
        design->previous_owner = 8;
        design->cost = 500;
        design->combat_speed = global::_drives[design->ftl_type].combat_speed_modifier[design->size];
    }

    void __cdecl Load_Titan_Antaran_Design_(s_ship_design* design) {
        struct_::Clear_Structure_(design, 99);

        strcpy(design->name, s_Harbinger_005576b8);

        design->size = 4;
        design->ship_type = 0;
        design->shield_type = 0;
        design->computer_type = 5;
        design->speed = 1;
        design->ftl_type = 5;
        design->armor_type = 6;

        design->ship_weapon[0].type = 4;
        design->ship_weapon[0].count = 20;
        design->ship_weapon[0].current_count = 20;
        design->ship_weapon[0].firing_arc = 1;
        design->ship_weapon[0].ammo = 0;

        design->ship_weapon[1].type = 4;
        design->ship_weapon[1].count = 5;
        design->ship_weapon[1].current_count = 5;
        design->ship_weapon[1].firing_arc = 1;
        design->ship_weapon[1].ammo = 2;

        design->ship_weapon[2].type = 24;
        design->ship_weapon[2].count = 30;
        design->ship_weapon[2].current_count = 30;
        design->ship_weapon[2].firing_arc = 1;
        design->ship_weapon[2].ammo = 0;

        design->ship_weapon[3].type = 13;
        design->ship_weapon[3].count = 3;
        design->ship_weapon[3].current_count = 3;
        design->ship_weapon[3].firing_arc = 15;
        design->ship_weapon[3].ammo = 0;

        design->ship_weapon[4].type = 4;
        design->ship_weapon[4].count = 20;
        design->ship_weapon[4].current_count = 20;
        design->ship_weapon[4].firing_arc = 1;
        design->ship_weapon[4].ammo = 4;

        design->ship_weapon[5].type = 11;
        design->ship_weapon[5].count = 5;
        design->ship_weapon[5].current_count = 5;
        design->ship_weapon[5].firing_arc = 1;
        design->ship_weapon[5].ammo = 2;

        design->ship_weapon[6].type = 37;
        design->ship_weapon[6].count = 1;
        design->ship_weapon[6].current_count = 1;
        design->ship_weapon[6].firing_arc = 15;
        design->ship_weapon[6].ammo = 0;

        design->ship_weapon[7].type = 37;
        design->ship_weapon[7].count = 12;
        design->ship_weapon[7].current_count = 12;
        design->ship_weapon[7].firing_arc = 15;
        design->ship_weapon[7].ammo = 0;

        struct_::Set_Bit_Field_(design->special_device_flags, 7);
        struct_::Set_Bit_Field_(design->special_device_flags, 24);
        struct_::Set_Bit_Field_(design->special_device_flags, 33);
        struct_::Set_Bit_Field_(design->special_device_flags, 26);

        switch (MOX::_settings.game_difficulty) {
        case 0:
            struct_::Set_Bit_Field_(design->special_device_flags, 9);
            break;
        case 1:
            struct_::Set_Bit_Field_(design->special_device_flags, 21);
            break;
        case 2:
        case 3:
        case 4:
            struct_::Set_Bit_Field_(design->special_device_flags, 38);
            break;
        }

        design->picture_num = 4;
        design->previous_owner = 8;
        design->cost = 500;
        design->combat_speed = global::_drives[design->ftl_type].combat_speed_modifier[design->size];
    }

    void __cdecl Load_Antaran_Ship_Design_(s_ship_design* design, int16_t hull_size) {
        switch (hull_size) {
            case 0:
                INITSHIP::Load_Small_Antaran_Design_(design);
                break;
            case 1:
                INITSHIP::Load_Medium_Antaran_Design_(design);
                break;
            case 2:
                INITSHIP::Load_Large_Antaran_Design_(design);
                break;
            case 3:
                INITSHIP::Load_Huge_Antaran_Design_(design);
                break;
            case 4:
                INITSHIP::Load_Titan_Antaran_Design_(design);
                break;
            default:
                break;
        }
    }

    void __cdecl Load_Colony_Ship_Design_(int16_t player_idx, s_ship_design* design) {
        struct_::Clear_Structure_(design, 99);

        const char* localized_name = &s_Colony_Ship_Localized[MOX::_settings.language * 16];
        strcpy(design->name, localized_name);

        design->size = 0;
        design->ship_type = SHIP_TYPE_COLONY;
        design->shield_type = 0;

        design->ftl_type = (int8_t)Best_Warp_Drive_(player_idx);
        design->computer_type = 0;

        int16_t armor = Best_Armor_(player_idx);
        if (armor > 1) {
            armor--;
        }
        design->armor_type = (int8_t)armor;

        struct_::Set_Bit_Field_(design->special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS);
        design->picture_num = 45;

        design->speed = (int8_t)TECHDATA::_drives[design->ftl_type].speed;
        design->cost = (int16_t)COLCALC::Ship_Type_Cost_For_Player_(SHIP_TYPE_COLONY, player_idx);

        design->combat_speed = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size];
    }

    void __cdecl Load_Doom_Star_Ship_Design_(int16_t owner_idx, s_ship_design* design) {
        aidesign::Auto_Design_Ship_(owner_idx, design, 5);
        strcpy(design->name, TECHDATA::_hull_data[5].name);
    }

    void __cdecl Repair_Ships_At_Colonies_() {
        int16_t best_warp_drives[8];

        for (int16_t i = 0; i < 8; ++i) {
            best_warp_drives[i] = Best_Warp_Drive_(i);
        }

        if (MOX::_NUM_SHIPS > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                s_ship_data& ship = MOX::_ship[i];

                if (ship.owner < 8 && ship.status == 0) {
                    int16_t best_ftl = best_warp_drives[(uint8_t)ship.owner];
                    if (best_ftl != (int16_t)ship.d.ftl_type) {
                        int8_t old_modifier = TECHDATA::_drives[ship.d.ftl_type].combat_speed_modifier[(uint8_t)ship.d.size];
                        ship.d.ftl_type = (int8_t)best_ftl;
                        int8_t new_modifier = TECHDATA::_drives[ship.d.ftl_type].combat_speed_modifier[(uint8_t)ship.d.size];

                        ship.d.combat_speed = (ship.d.combat_speed - old_modifier) + new_modifier;
                    }
                }

                if (ship.owner > 8) {
                    Repair_Ship_Full_(i);
                } else {
                    if (ship.location >= 0 && ship.location < 72 && 
                        ship.status == 0 && ship.d.ship_type == 0) {
                        
                        if (Test_Bit_Field_(&MOX::_star[ship.location].has_colony, (int16_t)ship.owner)) {
                            Repair_Ship_Full_(i);
                        }
                    }
                }
            }
        }
    }

    int16_t __cdecl Get_Ship_Armor_Hits_(int16_t ship_idx) {
        s_ship_data* ship = &MOX::_ship[ship_idx];

        int16_t armor_absorption_pct = TECHDATA::_armor[(uint8_t)ship->d.armor_type].ships_bonus;
        int16_t hull_absorption = TECHDATA::_hull_data[(uint8_t)ship->d.size].armor_absorption;

        bool has_special_armor = struct_::Test_Bit_Field_(ship->d.special_device_flags, 14);

        int32_t calc = ((int32_t)armor_absorption_pct * (int32_t)hull_absorption) / 100;
        int16_t total_hits = (int16_t)calc + hull_absorption;

        if (has_special_armor) {
            total_hits *= 3;
        }

        return total_hits;
    }

    int16_t __cdecl Get_Ship_Structure_(int16_t ship_idx) {
        int16_t result;
        s_ship_data* ship = &MOX::_ship[ship_idx];

        if (ship->owner <= 9) {
            return Get_Design_Structure_(ship->d);
        }

        switch (ship->owner) {
        case 10:
            result = 750;
            if (ship->d.ftl_type <= 0) {
                return 400;
            }
            break;

        case 11:
            result = 2500;
            if (ship->d.ftl_type <= 0) {
                return 500;
            }
            break;

        case 12:
            result = 2500;
            if (ship->d.ftl_type <= 0) {
                return 500;
            }
            break;

        case 13:
            result = 1000;
            if (ship->d.ftl_type <= 0) {
                return 300;
            }
            break;

        case 14:
            result = 1500;
            if (ship->d.ftl_type <= 0) {
                return 500;
            }
            break;

        default:
            break;
        }

        return result;
    }

    // Suggested name: Get_Ship_Combat_Bonuses
    void __cdecl Get_Ship_Combat_Bonuses_(int16_t ship_idx, int16_t* ship_defense, int16_t* ship_missile_defense, int16_t* ship_attack) {
        int16_t local_defense = 0;
        int16_t local_missile_defense = 0;
        int16_t local_attack = 0;

        s_ship_data* ship = &MOX::_ship[ship_idx];
        s_ship_design design_copy;
        _memcpy(&design_copy, &ship->d, sizeof(s_ship_design));

        Get_Design_Combat_Bonuses_(design_copy, &local_attack, &local_defense, &local_missile_defense);

        if (ship->officer_index > -1) {
            int16_t helmsman_bonus = CMBTFIR2::Helmsman_Bonus_(ship->officer_index);
            local_defense += helmsman_bonus;
            local_missile_defense += (int16_t)(helmsman_bonus / 2);

            local_attack += CMBTFIR2::Weaponry_Bonus_(ship->officer_index);
        }

        int16_t crew_def_bonus = _crew_data[(int)ship->crew_quality].defense;
        local_defense += crew_def_bonus;
        local_missile_defense += (int16_t)(crew_def_bonus / 2);
        local_attack += _crew_data[(int)ship->crew_quality].attack;

        if (ship->owner < 8) {
            s_player* owner_player = &MOX::_player[(int)ship->owner];

            int16_t trait_def_bonus = (int16_t)owner_player->traits[TRAIT_SHIP_DEFENSE];
            local_defense += trait_def_bonus;
            local_missile_defense += (int16_t)(trait_def_bonus / 2);

            local_attack += (int16_t)owner_player->traits[TRAIT_SHIP_ATTACK];

            if (owner_player->traits[TRAIT_TRANS_DIMENSIONAL] != 0) {
                local_defense += 20;
            }
        }

        *ship_defense = local_defense;
        *ship_missile_defense = local_missile_defense;
        *ship_attack = local_attack;
    }

    int16_t __cdecl Cost_Of_Strategic_Design_(int16_t govt_type, s_ship_design* design) {
        int32_t reduced_cost = (TECHDATA::_hull_data[design->size].hull_space * 3) / 2;

        DESIGN::Cost_Reduction_For_Govt_Type_(govt_type, &reduced_cost);

        return (int16_t)reduced_cost;
    }

    // Suggested name: Best_Beam_Weapon_
    int16_t __cdecl Best_Beam_Weapon_(int16_t player_idx) {
        int16_t best_weapon_idx = 3;

        // Special handling for non-standard players (monsters, Antarans, etc.)
        if (player_idx >= 8) {
            switch (player_idx) {
            case 8:
                return 11;
            case 9:
                return 4;
            case 10:
                return 1;
            case 11:
                return 42;
            case 12:
                return 9;
            case 13:
                return 1;
            case 14:
                return 43;
            default:
                // Fallback to default weapon index
                return 3;
            }
        }

        int16_t max_damage = 0;

        // Iterate through known beam weapons (0-39)
        for (int16_t i = 0; i < 40; ++i) {
            // Check if player has researched/learned this weapon (level 3)
            // and ensure it is a beam weapon (rack == 0)
            if (MOX::_player[player_idx].tech_applications[TECHDATA::_weapons[i].tech_application_id] == 3 &&
                TECHDATA::_weapons[i].rack == 0) {

                // Track weapon with highest damage
                if (TECHDATA::_weapons[i].damage > max_damage) {
                    max_damage = TECHDATA::_weapons[i].damage;
                    best_weapon_idx = i;
                }
            }
        }

        return best_weapon_idx;
    }

    // Suggested name: INITSHIP::Best_Missile_Weapon_
    int16_t __cdecl Best_Missile_Weapon_(int16_t player_idx) {
        int16_t best_idx = 14; /* 0xE */
        int16_t max_damage = 0;

        if (player_idx < 8) {
            for (int16_t i = 0; i < 40; ++i) {
                // check if player has researched the weapon (value 3 usually means fully researched/available)
                if (MOX::_player[player_idx].tech_applications[TECHDATA::_weapons[i].tech_application_id] == 3) {
                    // check if weapon is a missile (rack 1 or 2)
                    if (TECHDATA::_weapons[i].rack == 1 || TECHDATA::_weapons[i].rack == 2) {
                        if (max_damage < TECHDATA::_weapons[i].damage) {
                            best_idx = i;
                            max_damage = TECHDATA::_weapons[i].damage;
                        }
                    }
                }
            }
            return best_idx;
        }

        // Special handling for predefined entities or specific player indices >= 8
        switch (player_idx) {
            case 8:
                return 17; /* 0x11 */
            case 9:
                return 20; /* 0x14 */
            case 10:
                return 17; /* 0x11 */
            case 11:
                return 17; /* 0x11 */
            case 12:
                return 40; /* 0x28 */
            case 13:
                return 17; /* 0x11 */
            case 14:
                return 17; /* 0x11 */
            default:
                return 14; /* 0x0E */
        }
    }

    // Suggested name: INITSHIP::Best_Point_Defense_Weapon_By_Size_
    int16_t __cdecl Best_Point_Defense_Weapon_By_Size_(int16_t player_idx) {
        int16_t best_dmg_per_space = 0;
        int16_t best_weapon_idx = 3;

        if (player_idx < 8) {
            for (int16_t i = 0; i < 40; ++i) {
                const s_tech_weapon_data& weapon = TECHDATA::_weapons[i];

                if (MOX::_player[player_idx].tech_applications[weapon.tech_application_id] == 3 /* TECH_STATE_RESEARCHED */ &&
                    (weapon.specials & 0x4 /* WPN_SPECIAL_POINT_DEFENSE */) != 0 &&
                    weapon.rack == 0 /* WPN_RACK_NORMAL */)
                {
                    int16_t mini_level = DESIGN::Miniaturization_Level_(player_idx, weapon.tech_application_id);

                    if (mini_level >= (int16_t)TECHDATA::_weapon_modifications[2].level) {
                        int16_t current_dmg = Point_Defense_Damage_Per_Space_(i, player_idx);

                        if (current_dmg >= best_dmg_per_space) {
                            best_dmg_per_space = current_dmg;
                            best_weapon_idx = i;
                        }
                    }
                }
            }
            return best_weapon_idx;
        }

        return 4;
    }

    int16_t __cdecl Point_Defense_Damage_Per_Space_(int16_t weapon_idx, int16_t player_idx) {
        int16_t damage = TECHDATA::_weapons[weapon_idx].damage;
        int32_t space = DESIGN::Weapon_Space_(player_idx, weapon_idx, 1, 1, 1, 4);
        int32_t scaled_damage = (int32_t)damage * 100;
        return (int16_t)(scaled_damage / space) + damage;
    }

    int16_t __cdecl Get_Design_Structure_(s_ship_design d) {
        int16_t has_special = (int16_t)struct_::Test_Bit_Field_(d.special_device_flags, 27 /* 0x1b */);

        if (MOX::_settings.strategic_combat_flag != 0) {
            return TECHDATA::_hull_data[(int16_t)d.size].base_structure;
        }

        int16_t base_structure = TECHDATA::_hull_data[(int16_t)d.size].structure;
        int16_t absorption = TECHDATA::_armor[(uint8_t)d.armor_type].ships_bonus;

        // Calculate structure bonus from armor
        int32_t total_structure = base_structure + (absorption * base_structure) / 100;

        if (has_special != 0) {
            // Reinforced Hull triples the structure
            total_structure *= 3;
        }

        return (short)total_structure;
    }

    int16_t __cdecl Best_Missile_Weapon_NT_(int16_t player_idx) {
        int16_t best_weapon_idx = 14;
        int16_t max_damage = 0;

        if (player_idx < 8) {
            for (int i = 0; i < 40; ++i) {
                const auto& weapon = TECHDATA::_weapons[i];
                if (MOX::_player[player_idx].tech_applications[weapon.tech_application_id] == 3 &&
                    weapon.rack == 1 &&
                    weapon.damage > max_damage) {
                    best_weapon_idx = (int16_t)i;
                    max_damage = weapon.damage;
                }
            }
            return best_weapon_idx;
        } else {
            switch (player_idx) {
                case 8:  return 17;
                case 9:  return 20;
                case 10: return 17;
                case 11: return 17;
                case 12: return 40;
                case 13: return 17;
                case 14: return 17;
                default: return 14;
            }
        }
    }

    // Suggested name: Best_Strat_Special_Weapon_
    int16_t __cdecl Best_Strat_Special_Weapon_(int16_t player_idx, int16_t size) {
        if (player_idx >= 8) {
            switch (player_idx) {
                case 8:
                    return 37 /* 0x25 */;
                case 10:
                    return 45 /* 0x2d */;
                case 13:
                    return 44 /* 0x2c */;
                case 9:
                case 11:
                case 12:
                case 14:
                default:
                    return 0;
            }
        }

        int16_t best_weapon = 0;
        int16_t max_priority = 0;

        for (int16_t i = 0; i < 40; ++i) {
            // Specific weapon availability logic
            if (i == 37 && size < 4) continue;
            if (i == 38 && size < 5) continue;

            s_tech_weapon_data& weapon = TECHDATA::_weapons[i];
            s_player& player = MOX::_player[player_idx];

            // Check if player knows the weapon technology (3 = TECH_STATE_KNOWN)
            if (player.tech_applications[weapon.tech_application_id] == 3) {
                // Check if weapon is compatible with rack types 4 or 5
                if (weapon.rack == 5 || weapon.rack == 4) {
                    if ((uint8_t)weapon.ai_priority > max_priority) {
                        best_weapon = i;
                        max_priority = (uint8_t)weapon.ai_priority;
                    }
                }
            }
        }

        return best_weapon;
    }

    void __cdecl Update_Strategic_Design_Weapons_Specials_(s_ship_design* design, int16_t player_idx, int16_t design_idx) {
        int16_t weapon_count = 0;
        int16_t best_weapon;
        int16_t count;

        best_weapon = Best_Beam_Weapon_(player_idx);
        count = Strat_Beam_Weapon_Count_(player_idx, design_idx);
        if (best_weapon > 0 && count > 0) {
            design->ship_weapon[weapon_count].type = (int8_t)best_weapon;
            design->ship_weapon[weapon_count].count = (int8_t)count;
            design->ship_weapon[weapon_count].current_count = (int8_t)count;
            design->ship_weapon[weapon_count].firing_arc = 0x10;
            design->ship_weapon[weapon_count].specials = 0;
            design->ship_weapon[weapon_count].ammo = 0x5;
            weapon_count = 1;
        }

        best_weapon = Best_Missile_Weapon_(player_idx);
        count = Strat_Missile_Weapon_Count_(design_idx);
        if (best_weapon > 0 && count > 0) {
            design->ship_weapon[weapon_count].type = (int8_t)best_weapon;
            design->ship_weapon[weapon_count].count = (int8_t)count;
            design->ship_weapon[weapon_count].current_count = (int8_t)count;
            design->ship_weapon[weapon_count].firing_arc = 0x10;
            design->ship_weapon[weapon_count].specials = 0;
            design->ship_weapon[weapon_count].ammo = 0x5;
            weapon_count++;
        }

        best_weapon = Best_Strat_Special_Weapon_(player_idx, design_idx);
        count = Strat_Special_Weapon_Count_(design_idx);
        if (best_weapon > 0 && count > 0) {
            design->ship_weapon[weapon_count].type = (int8_t)best_weapon;
            design->ship_weapon[weapon_count].count = (int8_t)count;
            design->ship_weapon[weapon_count].current_count = (int8_t)count;
            design->ship_weapon[weapon_count].firing_arc = 0x10;
            design->ship_weapon[weapon_count].specials = 0;
            design->ship_weapon[weapon_count].ammo = 0x5;
            weapon_count++;
        }

        best_weapon = Best_Bomb_Weapon_(player_idx);
        count = Strat_Bomb_Weapon_Count_(design_idx);
        if (best_weapon > 0 && count > 0) {
            design->ship_weapon[weapon_count].type = (int8_t)best_weapon;
            design->ship_weapon[weapon_count].count = (int8_t)count;
            design->ship_weapon[weapon_count].current_count = (int8_t)count;
            design->ship_weapon[weapon_count].firing_arc = 0x10;
            design->ship_weapon[weapon_count].specials = 0;
            design->ship_weapon[weapon_count].ammo = 0xA;
        }

        int16_t special;

        special = Best_Beam_To_Hit_Special_(player_idx);
        if (special > 0) {
            struct_::Set_Bit_Field_(design->special_device_flags, special);
        }

        special = Best_Beam_Damage_Special_(player_idx);
        if (special > 0) {
            struct_::Set_Bit_Field_(design->special_device_flags, special);
        }

        special = Best_Missile_Damage_Special_(player_idx);
        if (special > 0) {
            struct_::Set_Bit_Field_(design->special_device_flags, special);
        }

        special = Best_Shield_Special_(player_idx);
        if (special > 0) {
            struct_::Set_Bit_Field_(design->special_device_flags, special);
        }

        special = Best_Beam_Defense_Special_(player_idx);
        if (special > 0) {
            struct_::Set_Bit_Field_(design->special_device_flags, special);
        }

        special = Best_Missile_Defense_Special_(player_idx);
        if (special > 0) {
            struct_::Set_Bit_Field_(design->special_device_flags, special);
        }
    }

    void __cdecl Update_Strategic_Ship_(int16_t ship_idx) {
        s_ship_data* ship = &MOX::_ship[ship_idx];
        int16_t owner = (int16_t)ship->owner;

        if (owner < 8 && MOX::_settings.strategic_combat_flag != 0) {
            ship->d.ftl_type = static_cast<int8_t>(Best_Warp_Drive_(owner));
            ship->d.speed = static_cast<int8_t>(Get_FTL_Speed_(owner, static_cast<int16_t>(ship->d.ftl_type)));
            ship->d.shield_type = static_cast<int8_t>(Best_Ship_Shield_(owner));
            ship->d.computer_type = static_cast<int8_t>(Best_Computer_(owner));
            ship->d.armor_type = static_cast<int8_t>(Best_Armor_(owner));

            Update_Strategic_Design_Weapons_Specials_(&ship->d, owner, static_cast<int16_t>(ship->d.size));
        }
    }

    int16_t __cdecl Strat_Missile_Weapon_Count_(int16_t hull_idx) {
        return TECHDATA::_hull_data[hull_idx].missle_weapon_count;
    }

    int16_t __cdecl Strat_Special_Weapon_Count_(int16_t hull_idx) {
        return TECHDATA::_hull_data[hull_idx].special_weapon_count;
    }

    int16_t __cdecl Strat_Beam_Weapon_Count_(int16_t player_idx, int16_t hull_idx) {
        int16_t weapon_count = TECHDATA::_hull_data[hull_idx].beam_weapon_count;

        if (player_idx < 8) {
            // Check for Megafluxers tech application
            if (MOX::_player[player_idx].tech_applications[TECH_APP_MEGAFLUXERS] == 3) {
                weapon_count = (int16_t)((weapon_count * 3) / 2);
            }
        }

        return weapon_count;
    }
/**
 * Finds the best point defense weapon available to a player.
 * A weapon is considered if it is learned (tech_level 3), has the PD special bit (0x4),
 * and has no rack count (0).
 *
 * @param player_idx The index of the player.
 * @return The index of the best PD weapon found, or default/fallback values (3 or 4).
 */
// Suggested name: INITSHIP::Best_Bomb_Weapon_
int16_t __cdecl Best_Bomb_Weapon_(int16_t player_idx) {
    if (player_idx >= 8) {
        return 24 /* 0x18 */;
    }

    int16_t best_weapon_idx = 21 /* 0x15 */;
    int16_t max_damage = 0;

    for (int16_t i = 0; i < 40 /* 0x28 */; ++i) {
        const s_tech_weapon_data& weapon = TECHDATA::_weapons[i];

        // Check if player has the tech, if it is a bomb (rack == 3), and if it deals more damage
        if (MOX::_player[player_idx].tech_applications[weapon.tech_application_id] == 3 &&
            weapon.rack == 3 &&
            weapon.damage > max_damage)
        {
            best_weapon_idx = i;
            max_damage = weapon.damage;
        }
    }

    return best_weapon_idx;
}

int16_t __cdecl Best_Point_Defense_Weapon_(int16_t player_idx) {
    int16_t best_weapon_idx = 3;
    int16_t max_damage = 0;

    if (player_idx < 8) {
        for (int16_t i = 0; i < 40; ++i) {
            int16_t tech_app_id = TECHDATA::_weapons[i].tech_application_id;

            // Check if the player has researched the weapon to full application (level 3)
            if (MOX::_player[player_idx].tech_applications[tech_app_id] == 3) {
                // Check if weapon has PD special (bit 2) and rack count is 0
                if ((TECHDATA::_weapons[i].specials & 0x4) && (TECHDATA::_weapons[i].rack == 0)) {
                    int16_t mini_level = DESIGN::Miniaturization_Level_(player_idx, tech_app_id);
                    
                    // Check if miniaturization level meets requirements for point defense mod
                    // modification index 2 corresponds to Point Defense modification.
                    if (mini_level >= (int16_t)TECHDATA::_weapon_modifications[2].level) {
                        if (TECHDATA::_weapons[i].damage > max_damage) {
                            max_damage = TECHDATA::_weapons[i].damage;
                            best_weapon_idx = i;
                        }
                    }
                }
            }
        }
    } else {
        best_weapon_idx = 4;
    }

    return best_weapon_idx;
}

/**
 * @brief Returns the maximum number of bomb slots available for a given ship hull.
 * @param hull_idx The index of the ship hull in the technology data.
 * @return The number of bomb weapon slots.
 */
int16_t __cdecl Strat_Bomb_Weapon_Count_(int16_t hull_idx) {
    return TECHDATA::_hull_data[hull_idx].bomb_weapon_count;
}

int16_t __cdecl Best_Beam_To_Hit_Special_(int16_t player_idx) {
    int16_t best_special_idx = 0;
    int16_t max_bonus = 0;

    // Ensure player_idx is within valid bounds
    if (player_idx < 8) {
        for (int16_t special_idx = 0; special_idx < 39 /* 0x27 */; ++special_idx) {
            const s_tech_specials_data& spec = TECHDATA::_specials[special_idx];

            // Check if the player has researched the tech associated with this special device
            // 3 usually corresponds to the application being learned/available
            if (MOX::_player[player_idx].tech_applications[spec.tech_application_id] == 3) {
                // Check if special belongs to the correct category (1 = beam accuracy)
                if (spec.category == 1) {
                    if (spec.bonus_value > max_bonus) {
                        max_bonus = spec.bonus_value;
                        best_special_idx = special_idx;
                    }
                }
            }
        }
    }

    return best_special_idx;
}

int16_t __cdecl Best_Beam_Damage_Special_(int16_t player_idx) {
    int16_t best_idx = 0;
    int16_t max_bonus = 0;

    if (player_idx < 8) {
        for (int16_t i = 0; i < 39; ++i) {
            const s_tech_specials_data& special = TECHDATA::_specials[i];

            // Check if player has fully researched the tech for this special device
            if (MOX::_player[player_idx].tech_applications[special.tech_application_id] == 3) {
                // Check if the device is categorized as a beam damage special (2 or 7)
                if (special.category == 2 || special.category == 7) {
                    if (max_bonus < special.bonus_value) {
                        best_idx = i;
                        max_bonus = special.bonus_value;
                    }
                }
            }
        }
    }

    return best_idx;
}

int16_t __cdecl Best_Beam_Defense_Special_(int16_t player_idx) {
    int16_t best_special_idx = 0;
    int16_t max_bonus = 0;

    if (player_idx < 8) {
        for (int16_t i = 0; i < 39; ++i) {
            const s_tech_specials_data& special = TECHDATA::_specials[i];

            // Check if the player has researched the technology for this special device
            if (MOX::_player[player_idx].tech_applications[special.tech_application_id] == 3) {
                // Category 3 and 6 represent beam defense enhancements
                if (special.category == 3 || special.category == 6) {
                    if (special.bonus_value > max_bonus) {
                        best_special_idx = i;
                        max_bonus = special.bonus_value;
                    }
                }
            }
        }
    }

    return best_special_idx;
}

/**
 * Finds the missile defense special device with the highest bonus value that the player has researched.
 * 
 * @param player_idx The index of the player.
 * @return The index of the best missile defense special device found, or 0 if none or invalid player.
 */
int16_t __cdecl Best_Missile_Defense_Special_(int16_t player_idx) {
    int16_t best_special_idx = 0;
    int16_t best_bonus = 0;

    if (player_idx < 8) {
        for (int16_t special_idx = 0; special_idx < 39 /* 0x27 */; ++special_idx) {
            const s_tech_specials_data& special = TECHDATA::_specials[special_idx];

            // Check if the player has researched the tech application associated with this special
            // Value 3 typically indicates the tech application is known/learned.
            if (MOX::_player[player_idx].tech_applications[special.tech_application_id] == 3) {
                // Category 4 and 6 represent missile defense systems (e.g., Jammer, Lightning Field)
                if (special.category == 4 || special.category == 6) {
                    if (special.bonus_value > best_bonus) {
                        best_bonus = special.bonus_value;
                        best_special_idx = special_idx;
                    }
                }
            }
        }
    }

    return best_special_idx;
}

void __cdecl Load_Small_Antaran_Combat_Ship_(int16_t ship_idx) {
    s_combat_data* ship = &MOX::_combat_data[ship_idx];

    strcpy(ship->name, s_Raider_0055768c);
    ship->owner = 8;
    ship->previous_owner = 8;
    ship->ship_idx = -1;
    ship->shield_type = 0;
    ship->computer_type = 5;
    ship->ftl_type = 5;
    ship->size_class = 0;
    ship->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        ship->special_device_flags[i] = 0;
        ship->special_device_damage_flags[i] = 0;
    }

    ship->weapons[0].weapon_id = 4;
    ship->weapons[0].count = 2;
    ship->weapons[0].arc = 1;
    ship->weapons[0].specials = 0;
    ship->weapons[0].ammo = -1;
    ship->weapons[0].active = 1;
    ship->weapons[0].shots_left = 2;
    ship->weapons[0].status = 0;

    ship->weapons[1].weapon_id = 24;
    ship->weapons[1].count = 1;
    ship->weapons[1].arc = 15;
    ship->weapons[1].specials = 0;
    ship->weapons[1].ammo = -1;
    ship->weapons[1].active = 1;
    ship->weapons[1].shots_left = 1;
    ship->weapons[1].status = 0;

    // Note: The loop starts at index 1 in the assembly, clearing the values just set for weapon 1.
    for (int i = 1; i < 8; ++i) {
        struct_::Clear_Structure_(&ship->weapons[i], sizeof(s_combat_weapon_slot));
    }

    struct_::Set_Bit_Field_(ship->special_device_flags, 7);
    struct_::Set_Bit_Field_(ship->special_device_flags, 24);
    if (MOX::_settings.game_difficulty > 2) {
        struct_::Set_Bit_Field_(ship->special_device_flags, 9);
    }

    ship->crew_quality = 3;
    ship->officer_idx = -1;
    ship->marine_count = (int8_t)TECHDATA::_hull_data[ship->size_class].marine_count;
    ship->armor_type = 6;

    int16_t absorption_percent = TECHDATA::_armor[ship->armor_type].ships_bonus;
    ship->structure_max = (int16_t)(((absorption_percent + 100) * (int)TECHDATA::_hull_data[ship->size_class].structure) / 100);
    ship->armor_remaining = (int16_t)(((absorption_percent + 100) * (int)TECHDATA::_hull_data[ship->size_class].armor_absorption) / 100);
    ship->structure_damage = 0;

    ship->combat_speed_base = TECHDATA::_drives[ship->ftl_type].combat_speed_modifier[ship->size_class];
    ship->picture_num = 0;
    ship->is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(ship);
}

void __cdecl Load_Medium_Antaran_Combat_Ship_(int16_t ship_idx) {
    s_combat_data* ship = &MOX::_combat_data[ship_idx];

    strcpy(ship->name, s_Marauder_00557694);
    ship->owner = 8;
    ship->previous_owner = 8;
    ship->ship_idx = -1;
    ship->shield_type = 0;
    ship->computer_type = 5;
    ship->ftl_type = 5;
    ship->size_class = 1;
    ship->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        ship->special_device_flags[i] = 0;
        ship->special_device_damage_flags[i] = 0;
    }

    // Initialize weapon slot 0
    ship->weapons[0].weapon_id = 4;
    ship->weapons[0].count = 3;
    ship->weapons[0].arc = 1;
    ship->weapons[0].specials = 0;
    ship->weapons[0].ammo = -1;
    ship->weapons[0].active = 1;
    ship->weapons[0].shots_left = 3;
    ship->weapons[0].status = 0;

    // Initialize weapon slot 1
    ship->weapons[1].weapon_id = 4;
    ship->weapons[1].count = 1;
    ship->weapons[1].arc = 1;
    ship->weapons[1].specials = 2;
    ship->weapons[1].ammo = -1;
    ship->weapons[1].active = 1;
    ship->weapons[1].shots_left = 1;
    ship->weapons[1].status = 0;

    // Initialize weapon slot 2
    ship->weapons[2].weapon_id = 24 /* BEAM_STING_RAY */;
    ship->weapons[2].count = 1;
    ship->weapons[2].arc = 15;
    ship->weapons[2].specials = 0;
    ship->weapons[2].ammo = 10;
    ship->weapons[2].active = 1;
    ship->weapons[2].shots_left = 1;
    ship->weapons[2].status = 0;

    // Clear remaining weapon slots
    for (int i = 3; i < 8; ++i) {
        struct_::Clear_Structure_(&ship->weapons[i], sizeof(s_combat_weapon_slot));
    }

    struct_::Set_Bit_Field_(ship->special_device_flags, 7);
    struct_::Set_Bit_Field_(ship->special_device_flags, 24);

    if (MOX::_settings.game_difficulty >= 4) {
        struct_::Set_Bit_Field_(ship->special_device_flags, 21);
    } else if (MOX::_settings.game_difficulty >= 3) {
        struct_::Set_Bit_Field_(ship->special_device_flags, 9);
    }

    ship->crew_quality = 3;
    ship->officer_idx = -1;
    ship->marine_count = (int8_t)TECHDATA::_hull_data[ship->size_class].marine_count;
    ship->armor_type = 6;

    int32_t armor_factor = TECHDATA::_armor[ship->armor_type].ships_bonus + 100;
    
    ship->structure_max = (int16_t)((armor_factor * TECHDATA::_hull_data[ship->size_class].structure) / 100);
    ship->armor_remaining = (int16_t)((armor_factor * TECHDATA::_hull_data[ship->size_class].armor_absorption) / 100);
    ship->structure_damage = 0;

    ship->combat_speed_base = TECHDATA::_drives[ship->ftl_type].combat_speed_modifier[ship->size_class];
    ship->picture_num = 1;
    ship->is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(ship);
}

void __cdecl Get_Design_Combat_Bonuses_(s_ship_design design, int16_t* attack_bonus_out, int16_t* defense_rating_out, int16_t* missile_defense_out) {
    int16_t missile_defense = 0;
    int16_t attack_bonus = TECHDATA::_computers[(uint8_t)design.computer_type].bonus;

    if (struct_::Test_Bit_Field_(design.special_device_flags, 5)) {
        attack_bonus += 50 /* 0x32 */;
    }

    int16_t current_speed = static_cast<uint8_t>(design.combat_speed);
    if (struct_::Test_Bit_Field_(design.special_device_flags, 2)) {
        current_speed += 5;
    }

    int16_t defense_rating = current_speed * 5;

    if (struct_::Test_Bit_Field_(design.special_device_flags, 9)) {
        missile_defense = 70 /* 0x46 */;
    }

    if (struct_::Test_Bit_Field_(design.special_device_flags, 17 /* 0x11 */)) {
        defense_rating += 100 /* 0x64 */;
        missile_defense += 50 /* 0x32 */;
    }

    if (struct_::Test_Bit_Field_(design.special_device_flags, 18 /* 0x12 */)) {
        defense_rating += 50 /* 0x32 */;
        missile_defense += 25 /* 0x19 */;
    }

    if (struct_::Test_Bit_Field_(design.special_device_flags, 21 /* 0x15 */)) {
        missile_defense += 100 /* 0x64 */;
    }

    if (struct_::Test_Bit_Field_(design.special_device_flags, 38 /* 0x26 */)) {
        missile_defense += 130 /* 0x82 */;
    }

    *defense_rating_out = defense_rating;
    *missile_defense_out = missile_defense;
    *attack_bonus_out = attack_bonus;
}

void __cdecl Load_Large_Antaran_Combat_Ship_(int16_t combat_idx) {
    s_combat_data* ship = &MOX::_combat_data[combat_idx];

    strcpy(ship->name, global::s_Intruder_005576a0);
    ship->owner = 8;
    ship->previous_owner = 8;
    ship->ship_idx = -1;
    ship->shield_type = 0;
    ship->computer_type = 5;
    ship->ftl_type = 5;
    ship->size_class = 2;
    ship->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        ship->special_device_flags[i] = 0;
        ship->special_device_damage_flags[i] = 0;
    }

    ship->weapons[0].weapon_id = 4;
    ship->weapons[0].count = 4;
    ship->weapons[0].arc = 1;
    ship->weapons[0].specials = 0;
    ship->weapons[0].ammo = -1;
    ship->weapons[0].active = 1;
    ship->weapons[0].shots_left = 4;
    ship->weapons[0].status = 0;

    ship->weapons[1].weapon_id = 4;
    ship->weapons[1].count = 2;
    ship->weapons[1].arc = 1;
    ship->weapons[1].specials = 2;
    ship->weapons[1].ammo = -1;
    ship->weapons[1].active = 1;
    ship->weapons[1].shots_left = 2;
    ship->weapons[1].status = 0;

    ship->weapons[2].weapon_id = 24 /* WEAPON_DEATH_RAY */;
    ship->weapons[2].count = 5;
    ship->weapons[2].arc = 15 /* ARC_360 */;
    ship->weapons[2].specials = 0;
    ship->weapons[2].ammo = 10;
    ship->weapons[2].active = 1;
    ship->weapons[2].shots_left = 5;
    ship->weapons[2].status = 0;

    ship->weapons[3].weapon_id = 13 /* WEAPON_PARTICLE_BEAM */;
    ship->weapons[3].count = 2;
    ship->weapons[3].arc = 15 /* ARC_360 */;
    ship->weapons[3].specials = 0;
    ship->weapons[3].ammo = -1;
    ship->weapons[3].active = 1;
    ship->weapons[3].shots_left = 2;
    ship->weapons[3].status = 0;

    ship->weapons[4].weapon_id = 31 /* WEAPON_SPATIAL_COMPRESSOR */;
    ship->weapons[4].count = 3;
    ship->weapons[4].arc = 15 /* ARC_360 */;
    ship->weapons[4].specials = 0;
    ship->weapons[4].ammo = -1;
    ship->weapons[4].active = 1;
    ship->weapons[4].shots_left = 3;
    ship->weapons[4].status = 0;

    for (int i = 4; i < 8; ++i) {
        struct_::Clear_Structure_(&ship->weapons[i], 11);
    }

    struct_::Set_Bit_Field_(ship->special_device_flags, 7 /* SDEVICE_QUANTUM_DETONATOR */);
    struct_::Set_Bit_Field_(ship->special_device_flags, 24 /* SDEVICE_ANTARAN_DAMPER */);

    if (MOX::_settings.game_difficulty == 2 /* DIFFICULTY_AVERAGE */) {
        struct_::Set_Bit_Field_(ship->special_device_flags, 9 /* SDEVICE_DISPLACEMENT_DEVICE */);
    } else if (MOX::_settings.game_difficulty == 3 /* DIFFICULTY_HARD */) {
        struct_::Set_Bit_Field_(ship->special_device_flags, 21 /* SDEVICE_REFLECTION_FIELD */);
    } else if (MOX::_settings.game_difficulty == 4 /* DIFFICULTY_IMPOSSIBLE */) {
        struct_::Set_Bit_Field_(ship->special_device_flags, 38 /* SDEVICE_LIGHT_BENDER */);
    }

    ship->crew_quality = 3;
    ship->officer_idx = -1;
    ship->marine_count = (int8_t)TECHDATA::_hull_data[ship->size_class].marine_count;
    ship->armor_type = 6 /* ARMOR_NEUTRONTUIM */;

    int32_t armor_boost = TECHDATA::_armor[ship->armor_type].ships_bonus + 100;
    ship->structure_max = (int16_t)((armor_boost * TECHDATA::_hull_data[ship->size_class].structure) / 100);
    ship->armor_remaining = (int16_t)((armor_boost * TECHDATA::_hull_data[ship->size_class].armor_absorption) / 100);
    ship->structure_damage = 0;

    ship->combat_speed_base = TECHDATA::_drives[ship->ftl_type].combat_speed_modifier[ship->size_class];
    ship->picture_num = 2;
    ship->is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(ship);
}

void __cdecl Load_Huge_Antaran_Combat_Ship_(int16_t ship_idx) {
    s_combat_data* ship = &MOX::_combat_data[ship_idx];

    strcpy(ship->name, s_Interdictor_005576ac);
    ship->owner = 8;
    ship->previous_owner = 8;
    ship->ship_idx = -1;
    ship->shield_type = 0;
    ship->computer_type = 5;
    ship->ftl_type = 5;
    ship->size_class = 3;
    ship->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        ship->special_device_flags[i] = 0;
        ship->special_device_damage_flags[i] = 0;
    }

    // Setup weapon slots
    ship->weapons[0].weapon_id = 4;
    ship->weapons[0].count = 6;
    ship->weapons[0].arc = 1;
    ship->weapons[0].specials = 0;
    ship->weapons[0].ammo = -1;
    ship->weapons[0].active = 1;
    ship->weapons[0].shots_left = 6;
    ship->weapons[0].status = 0;

    ship->weapons[1].weapon_id = 4;
    ship->weapons[1].count = 2;
    ship->weapons[1].arc = 1;
    ship->weapons[1].specials = 2;
    ship->weapons[1].ammo = -1;
    ship->weapons[1].active = 1;
    ship->weapons[1].shots_left = 2;
    ship->weapons[1].status = 0;

    ship->weapons[2].weapon_id = 24;
    ship->weapons[2].count = 15;
    ship->weapons[2].arc = 15;
    ship->weapons[2].specials = 0;
    ship->weapons[2].ammo = 10;
    ship->weapons[2].active = 1;
    ship->weapons[2].shots_left = 15;
    ship->weapons[2].status = 0;

    ship->weapons[3].weapon_id = 13;
    ship->weapons[3].count = 2;
    ship->weapons[3].arc = 15;
    ship->weapons[3].specials = 0;
    ship->weapons[3].ammo = -1;
    ship->weapons[3].active = 1;
    ship->weapons[3].shots_left = 2;
    ship->weapons[3].status = 0;

    ship->weapons[4].weapon_id = 4;
    ship->weapons[4].count = 8;
    ship->weapons[4].arc = 1;
    ship->weapons[4].specials = 4;
    ship->weapons[4].ammo = -1;
    ship->weapons[4].active = 1;
    ship->weapons[4].shots_left = 8;
    ship->weapons[4].status = 0;

    ship->weapons[5].weapon_id = 11;
    ship->weapons[5].count = 2;
    ship->weapons[5].arc = 1;
    ship->weapons[5].specials = 0;
    ship->weapons[5].ammo = -1;
    ship->weapons[5].active = 1;
    ship->weapons[5].shots_left = 2;
    ship->weapons[5].status = 0;

    for (int i = 6; i < 8; ++i) {
        struct_::Clear_Structure_(&ship->weapons[i], 11 /* sizeof(s_combat_weapon_slot) */);
    }

    // Special devices
    struct_::Set_Bit_Field_(ship->special_device_flags, 7);
    struct_::Set_Bit_Field_(ship->special_device_flags, 24);
    struct_::Set_Bit_Field_(ship->special_device_flags, 33);
    struct_::Set_Bit_Field_(ship->special_device_flags, 26);

    switch (MOX::_settings.game_difficulty) {
    case 1:
        struct_::Set_Bit_Field_(ship->special_device_flags, 9);
        break;
    case 2:
        struct_::Set_Bit_Field_(ship->special_device_flags, 21 /* 0x15 */);
        break;
    case 3:
    case 4:
        struct_::Set_Bit_Field_(ship->special_device_flags, 38 /* 0x26 */);
        break;
    default:
        break;
    }

    ship->crew_quality = 3;
    ship->officer_idx = -1;
    ship->marine_count = (int8_t)TECHDATA::_hull_data[ship->size_class].marine_count;
    ship->armor_type = 6;

    int32_t armor_factor = TECHDATA::_armor[6].ships_bonus + 100;
    ship->structure_max = (int16_t)((armor_factor * (int)TECHDATA::_hull_data[ship->size_class].structure) / 100);
    ship->armor_remaining = (int16_t)((armor_factor * (int)TECHDATA::_hull_data[ship->size_class].armor_absorption) / 100);
    ship->structure_damage = 0;

    ship->combat_speed_base = TECHDATA::_drives[ship->ftl_type].combat_speed_modifier[ship->size_class];
    ship->picture_num = 3;
    ship->is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(ship);
}

void __cdecl Load_Titan_Antaran_Combat_Ship_(int16_t ship_index) {
    s_combat_data* ship = &MOX::_combat_data[ship_index];

    strcpy(ship->name, MOX::s_Harbinger);
    ship->owner = 8;
    ship->previous_owner = 8;
    ship->ship_idx = -1;
    ship->shield_type = 0;
    ship->computer_type = 5;
    ship->ftl_type = 5;
    ship->size_class = 4;
    ship->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        ship->special_device_flags[i] = 0;
        ship->special_device_damage_flags[i] = 0;
    }

    // Slot 0
    ship->weapons[0].weapon_id = 4;
    ship->weapons[0].count = 10;
    ship->weapons[0].arc = 1;
    ship->weapons[0].specials = 0;
    ship->weapons[0].ammo = -1;
    ship->weapons[0].active = 1;
    ship->weapons[0].shots_left = 10;
    ship->weapons[0].status = 0;

    // Slot 1
    ship->weapons[1].weapon_id = 4;
    ship->weapons[1].count = 2;
    ship->weapons[1].arc = 1;
    ship->weapons[1].specials = 2;
    ship->weapons[1].ammo = -1;
    ship->weapons[1].active = 1;
    ship->weapons[1].shots_left = 2;
    ship->weapons[1].status = 0;

    // Slot 2
    ship->weapons[2].weapon_id = 24;
    ship->weapons[2].count = 20;
    ship->weapons[2].arc = 15;
    ship->weapons[2].specials = 0;
    ship->weapons[2].ammo = 10;
    ship->weapons[2].active = 1;
    ship->weapons[2].shots_left = 20;
    ship->weapons[2].status = 0;

    // Slot 3
    ship->weapons[3].weapon_id = 13;
    ship->weapons[3].count = 3;
    ship->weapons[3].arc = 15;
    ship->weapons[3].specials = 0;
    ship->weapons[3].ammo = -1;
    ship->weapons[3].active = 1;
    ship->weapons[3].shots_left = 3;
    ship->weapons[3].status = 0;

    // Slot 4
    ship->weapons[4].weapon_id = 4;
    ship->weapons[4].count = 15;
    ship->weapons[4].arc = 1;
    ship->weapons[4].specials = 4;
    ship->weapons[4].ammo = -1;
    ship->weapons[4].active = 1;
    ship->weapons[4].shots_left = 15;
    ship->weapons[4].status = 0;

    // Slot 5
    ship->weapons[5].weapon_id = 11;
    ship->weapons[5].count = 2;
    ship->weapons[5].arc = 1;
    ship->weapons[5].specials = 2;
    ship->weapons[5].ammo = -1;
    ship->weapons[5].active = 1;
    ship->weapons[5].shots_left = 2;
    ship->weapons[5].status = 0;

    // Slot 6
    ship->weapons[6].weapon_id = 37;
    ship->weapons[6].count = 1;
    ship->weapons[6].arc = 15;
    ship->weapons[6].specials = 0;
    ship->weapons[6].ammo = -1;
    ship->weapons[6].active = 1;
    ship->weapons[6].shots_left = 1;
    ship->weapons[6].status = 0;

    // Slot 7
    ship->weapons[7].weapon_id = 31;
    ship->weapons[7].count = 6;
    ship->weapons[7].arc = 15;
    ship->weapons[7].specials = 0;
    ship->weapons[7].ammo = -1;
    ship->weapons[7].active = 1;
    ship->weapons[7].shots_left = 6;
    ship->weapons[7].status = 0;

    // Assembly at 0x480b0a calls Clear_Structure_ on weapon slot 7 specifically
    struct_::Clear_Structure_(&ship->weapons[7], 11 /* sizeof(s_combat_weapon_slot) */);

    struct_::Set_Bit_Field_(ship->special_device_flags, 7);
    struct_::Set_Bit_Field_(ship->special_device_flags, 24);
    struct_::Set_Bit_Field_(ship->special_device_flags, 33);
    struct_::Set_Bit_Field_(ship->special_device_flags, 26);

    uint16_t difficulty_bit = 0;
    switch (MOX::_settings.game_difficulty) {
        case 0:
            difficulty_bit = 9;
            break;
        case 1:
            difficulty_bit = 21;
            break;
        case 2:
        case 3:
        case 4:
            difficulty_bit = 38;
            break;
    }

    if (difficulty_bit != 0) {
        struct_::Set_Bit_Field_(ship->special_device_flags, difficulty_bit);
    }

    ship->crew_quality = 3;
    ship->officer_idx = -1;
    ship->marine_count = TECHDATA::_hull_data[ship->size_class].marine_count;
    ship->armor_type = 6;

    int16_t abs_pct = TECHDATA::_armor[ship->armor_type].ships_bonus;
    ship->structure_max = (int16_t)((abs_pct + 100) * TECHDATA::_hull_data[ship->size_class].structure / 100);
    ship->armor_remaining = (int16_t)((abs_pct + 100) * TECHDATA::_hull_data[ship->size_class].armor_absorption / 100);
    ship->structure_damage = 0;
    ship->combat_speed_base = TECHDATA::_drives[ship->ftl_type].combat_speed_modifier[ship->size_class];
    ship->picture_num = 4;
    ship->is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(ship);
}

void __cdecl Load_Guardian_Ship_Design_(s_ship_design* design) {
    struct_::Clear_Structure_(design, 99 /* sizeof(s_ship_design) */);

    strcpy(design->name, MOX::_race_names[14]);

    design->ship_type = 0;
    design->size = 4;
    design->ftl_type = 0;
    design->speed = 0;
    design->shield_type = 3;

    design->ship_weapon[0].type = 11;
    design->ship_weapon[0].count = 2;
    design->ship_weapon[0].current_count = 2;
    design->ship_weapon[0].firing_arc = 15;
    design->ship_weapon[0].specials = 2;
    design->ship_weapon[0].ammo = 0;

    design->ship_weapon[1].type = 4;
    design->ship_weapon[1].count = 10;
    design->ship_weapon[1].current_count = 10;
    design->ship_weapon[1].firing_arc = 15;
    design->ship_weapon[1].specials = 4;
    design->ship_weapon[1].ammo = 0;

    design->ship_weapon[2].type = 20;
    design->ship_weapon[2].count = 1;
    design->ship_weapon[2].current_count = 1;
    design->ship_weapon[2].firing_arc = 15;
    design->ship_weapon[2].specials = 0x4500;
    design->ship_weapon[2].ammo = 0;

    design->ship_weapon[3].type = 20;
    design->ship_weapon[3].count = 1;
    design->ship_weapon[3].current_count = 1;
    design->ship_weapon[3].firing_arc = 15;
    design->ship_weapon[3].specials = 0x4500;
    design->ship_weapon[3].ammo = 0;

    design->ship_weapon[4].type = 13;
    design->ship_weapon[4].count = 2;
    design->ship_weapon[4].current_count = 2;
    design->ship_weapon[4].firing_arc = 15;
    design->ship_weapon[4].specials = 0;
    design->ship_weapon[4].ammo = 0;

    design->computer_type = 5;
    design->armor_type = 6;

    struct_::Set_Bit_Field_(design->special_device_flags, 3);
    struct_::Set_Bit_Field_(design->special_device_flags, 13);
    struct_::Set_Bit_Field_(design->special_device_flags, 1);
    struct_::Set_Bit_Field_(design->special_device_flags, 5);
    struct_::Set_Bit_Field_(design->special_device_flags, 25);
    struct_::Set_Bit_Field_(design->special_device_flags, 19);
    struct_::Set_Bit_Field_(design->special_device_flags, 21);
    struct_::Set_Bit_Field_(design->special_device_flags, 18);

    int16_t drive_idx = Best_Warp_Drive_(9);
    design->combat_speed = TECHDATA::_drives[drive_idx].combat_speed_modifier[(uint8_t)design->size];
    design->cost = 0;
    design->picture_num = 7;
    design->previous_owner = 9;
}

void __cdecl Load_Amoeba_Ship_Design_(s_ship_design* design, int16_t is_large) {
    struct_::Clear_Structure_(design, sizeof(s_ship_design));
    strcpy(design->name, MOX::_race_names[15]);

    if (is_large != 0) {
        design->ship_type = 0;
        design->size = 3;
        design->shield_type = 0;
        design->ftl_type = (int8_t)Best_Warp_Drive_(10);
        design->computer_type = 2;
        design->armor_type = 0;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 2;
        design->ship_weapon[0].current_count = 2;
        design->ship_weapon[1].firing_arc = 15;
        design->speed = 1;
        design->ship_weapon[1].specials = 0;
        design->ship_weapon[1].count = 5;
        design->ship_weapon[1].current_count = 5;
        design->ship_weapon[1].ammo = 10;
        design->picture_num = 8;
        design->ship_weapon[0].type = 45;
        design->ship_weapon[1].type = 23;
        design->combat_speed = TECHDATA::_drives[(uint8_t)design->ftl_type].combat_speed_modifier[design->size];
    } else {
        design->size = 2;
        design->ship_type = 0;
        design->shield_type = 0;
        design->computer_type = 2;
        design->armor_type = 0;
        design->ftl_type = 0;
        design->ship_weapon[0].specials = 0;
        design->speed = 0;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 1;
        design->ship_weapon[0].current_count = 1;
        design->ship_weapon[0].firing_arc = 15;
        design->picture_num = 20;
        design->ship_weapon[0].type = 45;
        int16_t best_drive = Best_Warp_Drive_(10);
        design->combat_speed = TECHDATA::_drives[best_drive].combat_speed_modifier[design->size];
    }

    struct_::Set_Bit_Field_(design->special_device_flags, 39 /* 0x27 */);
    design->previous_owner = 10;
    design->cost = 0;
}

// Suggested name: INITSHIP::Load_Crystal_Ship_Design_
void __cdecl Load_Crystal_Ship_Design_(s_ship_design *design, int16_t variant) {
    struct_::Clear_Structure_(design, 99 /* sizeof(s_ship_design) */);
    
    // Copy "Crystal" or specific race name from the race names array
    _sprintf(design->name, "%s", MOX::_race_names[16]);

    design->ship_type = 0;
    design->computer_type = 5;
    design->shield_type = 0;

    if (variant != 0) {
        // Crystal variant
        design->armor_type = 0;
        design->size = 4;
        int16_t best_drive = Best_Warp_Drive_(11);
        design->ftl_type = (int8_t)best_drive;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].ammo = 0;
        design->speed = 1;
        design->ship_weapon[0].count = 1;
        design->ship_weapon[0].current_count = 1;
        design->ship_weapon[1].firing_arc = 15;
        design->ship_weapon[1].ammo = 10;
        design->ship_weapon[1].count = 5;
        design->ship_weapon[1].current_count = 5;
        design->ship_weapon[0].type = 42;
        design->ship_weapon[1].specials = 0;
        design->ship_weapon[1].type = 26;
        design->ship_weapon[0].specials = 2;
        design->picture_num = 9;
        design->combat_speed = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size];
    } else {
        // Guardian variant
        design->armor_type = 0;
        design->ftl_type = 0;
        design->ship_weapon[0].specials = 0;
        design->speed = 0;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 1;
        design->ship_weapon[0].current_count = 1;
        design->ship_weapon[1].firing_arc = 15;
        design->size = 2;
        design->ship_weapon[1].ammo = 10;
        design->ship_weapon[1].specials = 0;
        design->ship_weapon[0].type = 42;
        design->ship_weapon[1].type = 25;
        design->ship_weapon[1].count = 5;
        design->ship_weapon[1].current_count = 5;
        int16_t best_drive = Best_Warp_Drive_(11);
        design->picture_num = 21;
        design->combat_speed = TECHDATA::_drives[best_drive].combat_speed_modifier[2];
    }

    design->previous_owner = 11;
    design->cost = 0;
}

void __cdecl Load_Dragon_Ship_Design_(s_ship_design* design, int16_t is_monster) {
    struct_::Clear_Structure_(design, sizeof(s_ship_design));
    strcpy(design->name, MOX::_race_names[17]);

    design->ship_type = 0;
    design->computer_type = 5;
    design->shield_type = 0;

    if (is_monster != 0) {
        design->armor_type = 0;
        design->size = 4;
        design->speed = 1;

        design->ftl_type = (int8_t)Best_Warp_Drive_(12);

        design->ship_weapon[0].type = 0x29;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].specials = 4;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 20;
        design->ship_weapon[0].current_count = 20;

        design->ship_weapon[1].type = 0x28;
        design->ship_weapon[1].firing_arc = 15;
        design->ship_weapon[1].specials = 0x4000;
        design->ship_weapon[1].ammo = 0;
        design->ship_weapon[1].count = 1;
        design->ship_weapon[1].current_count = 1;

        int8_t modifier = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size];
        design->picture_num = 10;
        design->combat_speed = modifier + 4;
    } else {
        design->armor_type = 0;
        design->speed = 0;
        design->size = 2;

        design->ship_weapon[0].type = 0x29;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].specials = 4;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 8;
        design->ship_weapon[0].current_count = 8;

        design->ship_weapon[1].type = 0x28;
        design->ship_weapon[1].firing_arc = 15;
        design->ship_weapon[1].specials = 0;
        design->ship_weapon[1].ammo = 0;
        design->ship_weapon[1].count = 1;
        design->ship_weapon[1].current_count = 1;

        int16_t best_drive = Best_Warp_Drive_(12);
        int8_t modifier = TECHDATA::_drives[best_drive].combat_speed_modifier[design->size];
        design->picture_num = 22;
        design->combat_speed = modifier + 4;
    }

    design->previous_owner = 12;
    design->cost = 0;
}

/**
 * Loads the ship design for a Space Eel (Young/Small or Greater).
 * @param design The ship design structure to populate.
 * @param is_greater_eel If non-zero, loads Greater Eel design; otherwise, Young/Small Eel.
 */
void __cdecl Load_Eel_Ship_Design_(s_ship_design* design, int16_t is_greater_eel) {
    struct_::Clear_Structure_(design, sizeof(s_ship_design));

    // MOX::_race_names[18] is the design name for Eels
    strcpy(design->name, MOX::_race_names[18]);

    design->ship_type = 0;
    design->computer_type = 4;
    design->shield_type = 0;

    if (is_greater_eel != 0) {
        design->size = 3;
        design->ftl_type = (int8_t)INITSHIP::Best_Warp_Drive_(13 /* TECH_WARP_DRIVE */);
        design->armor_type = 0;
        design->speed = 1;

        struct_::Set_Bit_Field_(design->special_device_flags, 19 /* SPECIAL_DEVICE_19 */);

        design->ship_weapon[0].type = 44 /* WEAPON_EEL_ATTACK */;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 2;
        design->ship_weapon[0].current_count = 2;
        design->ship_weapon[0].firing_arc = 15 /* ARC_360 */;

        design->picture_num = 11;
        design->combat_speed = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size] + 8;
    } else {
        design->armor_type = 0;
        int16_t best_warp = INITSHIP::Best_Warp_Drive_(13 /* TECH_WARP_DRIVE */);

        design->ftl_type = 0;
        design->size = 2;
        design->speed = 0;

        struct_::Set_Bit_Field_(design->special_device_flags, 19 /* SPECIAL_DEVICE_19 */);

        design->ship_weapon[0].type = 44 /* WEAPON_EEL_ATTACK */;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 1;
        design->ship_weapon[0].current_count = 1;
        design->ship_weapon[0].firing_arc = 15 /* ARC_360 */;

        design->picture_num = 23;
        // Lookup uses Best_Warp_Drive directly regardless of ftl_type field
        design->combat_speed = TECHDATA::_drives[best_warp].combat_speed_modifier[design->size] + 8;
    }

    design->previous_owner = 13 /* PLAYER_MONSTER */;
    design->cost = 0;
}

void __cdecl Load_Hydra_Ship_Design_(s_ship_design* design, int16_t is_monster) {
    struct_::Clear_Structure_(design, sizeof(s_ship_design));

    strcpy(design->name, MOX::_race_names[19]);

    design->ship_type = 0;
    design->computer_type = 2;
    design->shield_type = 0;

    if (is_monster != 0) {
        design->size = 4;
        design->armor_type = 0;
        design->ftl_type = (int8_t)INITSHIP::Best_Warp_Drive_(14);
        design->speed = 1;
        design->ship_weapon[0].type = 43;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].specials = 2;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 5;
        design->ship_weapon[0].current_count = 5;
        design->picture_num = 12;
        design->combat_speed = TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->size];
    } else {
        design->size = 2;
        design->armor_type = (int8_t)INITSHIP::Best_Armor_(14);
        design->speed = 0;
        design->ship_weapon[0].type = 43;
        design->ship_weapon[0].firing_arc = 15;
        design->ship_weapon[0].specials = 0;
        design->ship_weapon[0].ammo = 0;
        design->ship_weapon[0].count = 3;
        design->ship_weapon[0].current_count = 3;
        design->picture_num = 24;

        int16_t best_drive = INITSHIP::Best_Warp_Drive_(14);
        design->combat_speed = TECHDATA::_drives[best_drive].combat_speed_modifier[design->size];
    }

    struct_::Set_Bit_Field_(design->special_device_flags, 10);
    design->previous_owner = 14;
    design->cost = 0;
}

}