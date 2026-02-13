#include "deps.h"

namespace aidesign {

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

}