#include "deps.h"
#include <cstring>

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

    int16_t __cdecl Get_Structure_(int16_t hull_index, bool is_fort, int16_t armor_tech) {
        int16_t armor_idx = Best_Armor_(armor_tech);

        int16_t base_structure = TECHDATA::_hull_data[hull_index].structure;
        int16_t absorption_pct = TECHDATA::_armor[armor_idx].armor_absorption_percent;

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
        design->cost = aidesign::Cost_Reduction_For_Govt_Hull_Type(player_idx, &MOX::_player[player_idx].ship_designs[design_idx]);

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
}