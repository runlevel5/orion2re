#include "pch.h"

namespace AIPOWER {
    uint32_t __cdecl Modified_Weapon_Strength_(uint32_t weapon_strength, uint16_t modification_flags) {
        int16_t total_space = 100;

        if ((modification_flags & 0x0002) != 0) {
            total_space = static_cast<int16_t>(TECHDATA::_weapon_modifications_1_space + 100);
        }
        if ((modification_flags & 0x0004) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_2_space);
        }
        if ((modification_flags & 0x0020) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_5_space);
        }
        if ((modification_flags & 0x0080) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_7_space);
        }
        if ((modification_flags & 0x0100) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_8_space);
        }
        if ((modification_flags & 0x0200) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_9_space);
        }
        if ((modification_flags & 0x0400) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_10_space);
        }
        if ((modification_flags & 0x0800) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_11_space);
        }
        if ((modification_flags & 0x1000) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_12_space);
        }
        if ((modification_flags & 0x2000) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_13_space);
        }
        if ((modification_flags & 0x4000) != 0) {
            total_space = static_cast<int16_t>(total_space + TECHDATA::_weapon_modifications_14_space);
        }

        int32_t result =
            (static_cast<int32_t>(weapon_strength & 0xFFFFu) * static_cast<int32_t>(total_space)) / 100;

        if (static_cast<uint32_t>(result) > 64000u) {
            result = 64000;
        }

        return static_cast<uint32_t>(result);
    }

    int16_t __cdecl Get_Best_Ship_Shield_Value_(int16_t player_idx) {
        if (player_idx >= 8) {
            switch (player_idx) {
                case 8:
                    return 6;
                case 9:
                    return 8;
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    return 0;
                default: {
                    // Original returns uninitialized variable, we return 0
                    return 0;
                }
            }
        }

        int16_t shield_idx = INITSHIP::Best_Ship_Shield_(player_idx);
        return TECHDATA::_shields[shield_idx].shield_class;
    }

    /**
     * Calculates ship strength vs a specific shield value.
     * used for AI power estimation and strategic combat resolution.
     */
    int16_t __cdecl Ship_Strength_Vs_Shield_(int16_t ship_idx, int16_t target_idx, int16_t shield_strength) {
        s_ship_data* ship = &MOX::_ship[ship_idx];
        int32_t total_strength = 0;
        int16_t attack_bonus = 0;
        int16_t defense_bonus;
        int16_t temp_attack;
        int16_t temp_defense;

        if (MOX::_settings.strategic_combat_flag != 0) {
            attack_bonus = AIPOWER::Base_Generic_Ocv_(ship->owner);
            INITSHIP::Update_Strategic_Ship_(ship_idx);
        } else {
            INITSHIP::Get_Ship_Combat_Bonuses_(ship_idx, &temp_attack, &temp_defense, &attack_bonus);
        }

        defense_bonus = Base_Generic_Dcv_(target_idx);

        for (int i = 0; i < 8; ++i) {
            int16_t weapon_type = ship->d.ship_weapon[i].type;
            if (weapon_type <= 0) continue;

            int16_t count = ship->d.ship_weapon[i].current_count;
            uint16_t slot_specials = ship->d.ship_weapon[i].specials;
            uint32_t slot_strength;

            if (!(slot_specials & 0x40) && weapon_type != 0x22 && weapon_type != 0x0D) {
                if (weapon_type == 0x1F /* Point Defense */) {
                    weapon_type = INITSHIP::Best_Point_Defense_Weapon_(ship->owner);
                    count *= 3;
                } else if (weapon_type == 0x1E || weapon_type == 0x1D /* Bombs */) {
                    weapon_type = INITSHIP::Best_Bomb_Weapon_(ship->owner);
                    count *= 3;
                } else if (weapon_type == 0x1C /* Stellar Converter */) {
                    weapon_type = 0x1B;
                    count *= 3;
                } else if (weapon_type == 0x22 || weapon_type == 0x0D || weapon_type == 0x24 /* Fighters/Bombers */) {
                    count *= 3;
                }

                int16_t damage = TECHDATA::_weapons[weapon_type].max_damage;
                if (slot_specials & 0x02) {
                    damage += (int16_t)(((damage * 5 + 5) * 10) / 100);
                } else if (slot_specials & 0x04) {
                    damage = (int16_t)(((damage * 5 + 5) * 10) / 100);
                }

                int32_t adjusted_damage = (int32_t)damage - (int32_t)shield_strength;
                if (adjusted_damage < 1) adjusted_damage = 0;
                slot_strength = adjusted_damage * count;
            } else {
                slot_strength = (int32_t)TECHDATA::_weapons[weapon_type].max_damage * (int32_t)count;
            }

            if (slot_strength != 0) {
                int8_t rack_type = TECHDATA::_weapons[weapon_type].type;
                switch (rack_type) {
                    case 0: {
                        int16_t hit_chance = (attack_bonus - defense_bonus) + 50;
                        if (slot_specials != 0) {
                            hit_chance += 25;
                        }
                        if (hit_chance < 10) hit_chance = 10;
                        if (hit_chance > 100) hit_chance = 100;
                        slot_strength = (slot_strength * (uint32_t)hit_chance) / 100;
                        break;
                    }
                    case 1: {
                        uint8_t ammo = ship->d.ship_weapon[i].ammo;
                        if (ammo == 2) {
                            slot_strength = (slot_strength * 3) >> 2;
                        }
                        break;
                    }
                    case 2:
                        slot_strength = (slot_strength * 3) >> 2;
                        break;
                    case 3:
                        slot_strength = 0;
                        break;
                    default:
                        break;
                }
            }

            uint32_t mod_strength = Modified_Weapon_Strength_(slot_strength, slot_specials);
            total_strength += (uint16_t)mod_strength;
        }

        int16_t best_shield = INITSHIP::Best_Ship_Shield_(ship->owner);
        int16_t current_shield = ship->d.shield_type;
        int16_t shield_diff = best_shield - current_shield;
        int16_t shield_scale = 100;

        if (shield_diff < 0) {
            int16_t abs_diff = (shield_diff < 0) ? -shield_diff : shield_diff;
            shield_scale = (10 - abs_diff) * 10;
        }

        if (shield_scale < 10) shield_scale = 10;

        uint32_t scaled_strength = ((uint32_t)shield_scale * (uint32_t)total_strength) / 100;

        if (MOX::_settings.strategic_combat_flag == 0) {
            int16_t current_hits = Current_Ship_Hits_(ship_idx);
            uint32_t exists_flag = (scaled_strength != 0) ? 1 : 0;
            int16_t hits_mod = Strength_Hits_Modifier_(current_hits);
            uint32_t hit_modified_strength = ((uint32_t)hits_mod * scaled_strength) / 100;
            
            if (scaled_strength > 0) {
                if (hit_modified_strength > exists_flag) {
                    scaled_strength = hit_modified_strength;
                } else {
                    scaled_strength = exists_flag;
                }
            }
        }

        if (scaled_strength > 0xFA00 /* 64000 */) {
            scaled_strength = 0xFA00;
        }

        return (int16_t)scaled_strength;
    }

    int16_t __cdecl Max_Ship_Hits_(int16_t ship_idx) {
        if (MOX::_settings.strategic_combat_flag == 0) {
            int16_t structure = INITSHIP::Get_Ship_Structure_(ship_idx);
            int16_t armor_hits = INITSHIP::Get_Ship_Armor_Hits_(ship_idx);
            return structure + armor_hits;
        } else {
            s_ship_data* ship = &MOX::_ship[ship_idx];

            int hull_type = (int)ship->d.size;
            int16_t base_hits = TECHDATA::_hull_data[hull_type].strat_hits;

            int16_t best_armor_idx = INITSHIP::Best_Armor_((int16_t)ship->owner);
            if (best_armor_idx > 0) {
                // The original code performs a second call to Best_Armor_ if result is positive
                best_armor_idx = INITSHIP::Best_Armor_((int16_t)ship->owner);

                int absorption_pct = (int)TECHDATA::_armor[best_armor_idx].ships_bonus;

                // bonus = (absorption_percent * base_hits) / 100
                int16_t bonus_hits = (int16_t)((absorption_pct * (int)base_hits) / 100);
                return base_hits + bonus_hits;
            }

            return base_hits;
        }
    }

    /**
     * Calculates the combat strength of an attacking ship against a target ship by first 
     * determining the target's best shield value and then calculating strength vs that shield.
     */
    uint16_t __cdecl Ship_Strength_Vs_Ships_(int16_t attacker_idx, int16_t target_idx) {
        int16_t shield_value = Get_Best_Ship_Shield_Value_(target_idx);
        return (uint16_t)Ship_Strength_Vs_Shield_(attacker_idx, target_idx, shield_value);
    }

    /**
     * Calculates the combat strength of an attacking ship against a target ship in a nebula.
     * In a nebula, shields are disabled, so the shield value is set to 0.
     */
    uint16_t __cdecl Ship_Strength_Vs_Ships_In_Nebula_(int16_t attacker_idx, int16_t target_idx) {
        return (uint16_t)AIPOWER::Ship_Strength_Vs_Shield_(attacker_idx, target_idx, 0);
    }

    int16_t __cdecl Current_Ship_Hits_(int16_t ship_idx) {
        int16_t hits;

        hits = AIPOWER::Max_Ship_Hits_(ship_idx);

        // Substract structural damage
        hits -= MOX::_ship[ship_idx].structural_damage;

        // In strategic combat armor damage is not counted
        if (MOX::_settings.strategic_combat_flag == 0) {
            hits -= MOX::_ship[ship_idx].armor_damage;
        }

        return hits;
    }

    /**
     * Calculates ship strength against 4 levels of planetary shields.
     * results[0..3] corresponds to the 4 shield levels.
     */
    void __cdecl Ship_Strength_Vs_Planetary_Shield_(int16_t ship_idx, uint16_t* results) {
        uint32_t strengths[4];
        uint16_t shield_reductions[4] = { 2, 7, 12, 22 };

        for (int i = 0; i < 4; ++i) {
            strengths[i] = 0;
        }

        if (MOX::_settings.strategic_combat_flag == 1) {
            int32_t size = MOX::_ship[ship_idx].d.size;
            uint32_t base_strength = (uint32_t)(size * size * 10);
            strengths[0] = base_strength;

            for (int i = 1; i < 4; ++i) {
                strengths[i] = (base_strength + (uint32_t)i - 1) / (uint32_t)i;
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                int16_t weapon_type = MOX::_ship[ship_idx].d.ship_weapon[i].type;
                if (weapon_type > 0) {
                    int16_t weapon_damage;
                    uint8_t rack_type = TECHDATA::_weapons[weapon_type].type;

                    if (rack_type == 0 || rack_type == 2) {
                        weapon_damage = TECHDATA::_weapons[weapon_type].max_damage / 2;
                    } else {
                        weapon_damage = TECHDATA::_weapons[weapon_type].max_damage;
                    }

                    int32_t count = MOX::_ship[ship_idx].d.ship_weapon[i].current_count;

                    for (int j = 0; j < 4; ++j) {
                        int32_t damage_vs_shield = (int32_t)weapon_damage - (int32_t)shield_reductions[j];
                        int32_t weapon_power = damage_vs_shield * count;

                        if (weapon_power > 0) {
                            strengths[j] += weapon_power;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < 4; ++i) {
            int16_t current_hits = Current_Ship_Hits_(ship_idx);
            int16_t max_hits = Max_Ship_Hits_(ship_idx);

            // Scale strength by current ship health
            uint32_t final_power = (uint32_t)current_hits * strengths[i] / (uint32_t)max_hits;

            results[i] = 0xfa00; /* 64000 */
            if (final_power <= 0xfa00) {
                results[i] = (uint16_t)final_power;
            }
        }
    }

    int16_t __cdecl Base_Generic_Ocv_(int16_t player_idx) {
        int16_t computer_idx;
        int16_t bonus;

        computer_idx = INITSHIP::Best_Computer_(player_idx);
        bonus = TECHDATA::_computers[computer_idx].bonus;

        if (player_idx < 8) {
            bonus += MOX::_player[player_idx].traits[7 /* TRAIT_SHIP_ATTACK */];
        }

        return bonus;
    }

    /**
     * @brief Calculates the defensive space strength of a colony against a specific player.
     * 
     * Determines the building level of the colony's defensive satellites (Star Base, Battlestation, 
     * or Star Fortress) and calculates the total strength based on that building and the colony's owner.
     * 
     * @param colony_idx The index of the colony in the global colony array.
     * @param player_idx The index of the player attacking the colony.
     * @return The calculated space strength as a 16-bit unsigned integer.
     */
    uint16_t __cdecl Colony_Space_Strength_Vs_Player_(int16_t colony_idx, int16_t player_idx) {
        s_colony* colony = &MOX::_colony[colony_idx];
        int16_t building_type = -1;

        if (colony->buildings[BUILDING_STAR_BASE] != 0) {
            building_type = 1;
        }
        else if (colony->buildings[BUILDING_BATTLESTATION] != 0) {
            building_type = 2;
        }
        else if (colony->buildings[BUILDING_STAR_FORTRESS] != 0) {
            building_type = 3;
        }

        return AIPOWER::Satelite_Strength_Vs_Ships_(building_type, (int16_t)colony->owner, player_idx);
    }

    /**
     * @brief Calculates the defensive strength of a stationary defense (Star Base, Battlestation, Fortress) against ships.
     * @param building_type Index offset for stationary defense (0=Star Base, 1=Battlestation, 2=Fortress).
     * @param player_idx Index of the player owning the satellite.
     * @param target_player_idx Index of the attacking player.
     * @return Strength value clamped to 64000.
     */
    int16_t __cdecl Base_Generic_Dcv_(int16_t player_idx) {
        const int16_t best_drive_idx = INITSHIP::Best_Warp_Drive_(player_idx);

        int16_t dcv =
            static_cast<int16_t>(static_cast<int8_t>(TECHDATA::_drives[best_drive_idx].combat_speed_min[2]));
        dcv = static_cast<int16_t>(dcv * 5);

        if (player_idx < 8) {
            dcv = static_cast<int16_t>(dcv + MOX::_player[player_idx].traits[6]);
            if (MOX::_player[player_idx].traits[0x1d] != 0) {
                dcv = static_cast<int16_t>(dcv + 0x14);
            }
        }

        return dcv;
    }

    uint16_t __cdecl Satelite_Strength_Vs_Ships_(int16_t building_type, int16_t player_idx, int16_t target_player_idx) {
        uint32_t total_strength = 0;

        if (building_type != -1) {
            int16_t ocv = AIPOWER::Base_Generic_Ocv_(player_idx);
            int16_t dcv = (int16_t)Base_Generic_Dcv_(target_player_idx);
            int32_t accuracy_mod = (ocv - dcv) + 100;

            if (accuracy_mod < 10) {
                accuracy_mod = 10;
            }
            if (accuracy_mod > 100) {
                accuracy_mod = 100;
            }

            int16_t best_beam = INITSHIP::Best_Beam_Weapon_(player_idx);
            int16_t best_missile = INITSHIP::Best_Missile_Weapon_NT_(player_idx);

            int16_t beam_count_normal;
            int16_t beam_count_heavy;
            int16_t missile_count;
            s_tech_hull_data* hull = &TECHDATA::_hull_data[5 + building_type];

            if (MOX::_settings.strategic_combat_flag == 0) {
                int16_t hull_size = hull->size;
                int16_t size_div_2 = hull_size / 2;
                int16_t rem_size = hull_size - size_div_2;

                // Tactical: Calculate weapon counts based on available hull space
                int32_t space_normal = DESIGN::Weapon_Space_(player_idx, best_beam, 1, 0xF /* STATUS_OK */, 0, 0);
                beam_count_normal = (int16_t)((size_div_2 / 4) / space_normal);

                int32_t space_heavy = DESIGN::Weapon_Space_(player_idx, best_beam, 1, 0xF /* STATUS_OK */, 0, 2 /* MOD_HEAVY_MOUNT */);
                beam_count_heavy = (int16_t)((size_div_2 / 2) / space_heavy);

                int32_t space_missile = DESIGN::Weapon_Space_(player_idx, best_missile, 1, 0xF /* STATUS_OK */, 20, 0);
                missile_count = (int16_t)(rem_size / space_missile);
            } else {
                // Strategic: Use predefined counts
                beam_count_heavy = 0;
                beam_count_normal = hull->strat_beam;
                missile_count = hull->strat_missile;
            }

            int16_t shield_val = AIPOWER::Get_Best_Ship_Shield_Value_(target_player_idx);
            
            int32_t adj_beam_dmg_heavy = ((TECHDATA::_weapons[best_beam].max_damage * 3) / 2) - shield_val;
            if (adj_beam_dmg_heavy < 0) adj_beam_dmg_heavy = 0;

            int32_t adj_missile_dmg = TECHDATA::_weapons[best_missile].max_damage - shield_val;
            if (adj_missile_dmg < 0) adj_missile_dmg = 0;

            int32_t adj_beam_dmg_normal = TECHDATA::_weapons[best_beam].max_damage - shield_val;
            if (adj_beam_dmg_normal < 0) adj_beam_dmg_normal = 0;

            total_strength = (adj_beam_dmg_heavy * beam_count_heavy * accuracy_mod) / 100;
            total_strength += (adj_missile_dmg * missile_count);
            total_strength += (adj_beam_dmg_normal * beam_count_normal * accuracy_mod) / 100;

            if (MOX::_settings.strategic_combat_flag == 0) {
                uint32_t structural_mod = Strength_Hits_Modifier_(hull->structure_hp);
                uint32_t val = (structural_mod * total_strength) / 100;
                
                if (val > 1) {
                    val = (Strength_Hits_Modifier_(hull->structure_hp) * total_strength) / 100;
                } else {
                    val = 1;
                }
                
                total_strength = (val * 75) / 100;
            }

            if (total_strength > 0xFA00 /* 64000 */) {
                total_strength = 0xFA00;
            }
        }

        return (uint16_t)total_strength;
    }

    /**
     * Calculates the defensive strength of a colony's ground-based installations 
     * against a specific player.
     */
    /**
     * Calculates the defensive ground strength of a colony's missile base.
     */
    uint32_t __cdecl Missile_Base_Strength_(int16_t player_idx, int16_t target_idx) {
        if (MOX::_settings.strategic_combat_flag == 1) {
            return 90 /* 0x5a */;
        }

        int16_t weapon_idx = INITSHIP::Best_Missile_Weapon_NT_(player_idx);

        // weapon_idx is used here to get space cost
        int32_t weapon_space = DESIGN::Weapon_Space_(player_idx, weapon_idx, 1, 16 /* 0x10 */, 5, 0);

        // Calculate number of weapon mounts based on base space 300
        int16_t capacity = (int16_t)(300 / weapon_space);

        int16_t shield_val = AIPOWER::Get_Best_Ship_Shield_Value_(target_idx);

        // Calculate base damage per weapon mount minus shield reduction
        int32_t net_damage = (int32_t)TECHDATA::_weapons[weapon_idx].max_damage - (int32_t)shield_val;

        if (net_damage <= 0) {
            net_damage = 0;
        }

        uint32_t strength = (uint32_t)net_damage * (uint32_t)capacity * 2;

        if (strength > 64000 /* 0xfa00 */) {
            strength = 64000;
        }

        return strength;
    }

    uint16_t __cdecl Colony_Ground_Strength_Vs_Player_(int16_t colony_idx, int16_t player_idx) {
        uint16_t strength = 0;
        s_colony* colony = &MOX::_colony[colony_idx];

        // BUILDING_MISSILE_BASE: 26
        if (colony->buildings[BUILDING_MISSILE_BASE] != 0) {
            strength = AIPOWER::Missile_Base_Strength_((int16_t)colony->owner, player_idx);
        }

        // BUILDING_GROUND_BATTERIES: 27
        if (colony->buildings[BUILDING_GROUND_BATTERIES] != 0) {
            strength += AIPOWER::Phasor_Base_Strength_((int16_t)colony->owner, player_idx);
        }

        // BUILDING_STELLAR_CONVERTER: 42
        if (colony->buildings[BUILDING_STELLAR_CONVERTER] != 0) {
            if (MOX::_settings.strategic_combat_flag == 1) {
                strength += 200;
            } else {
                strength += 500;
            }
        }

        // BUILDING_FIGHTER_GARRISON: 47
        if (colony->buildings[BUILDING_FIGHTER_GARRISON] != 0) {
            strength += AIPOWER::Fighter_Garrison_Strength_((int16_t)colony->owner, player_idx);
        }

        // Cap maximum defensive strength
        if (strength > 64000) {
            strength = 64000;
        }

        return strength;
    }

    // Suggested name: AIPOWER::Phasor_Base_Strength_
    uint32_t __cdecl Phasor_Base_Strength_(int16_t ship_idx, int16_t target_idx) {
        if (MOX::_settings.strategic_combat_flag == 1) {
            return 120 /* 0x78 */;
        }

        int16_t weapon_idx = INITSHIP::Best_Beam_Weapon_(ship_idx);
        int32_t space = DESIGN::Weapon_Space_(ship_idx, weapon_idx, 1, 1, 1, 2);

        // Number of weapon mounts that can fit in standard phasor space allocation
        int16_t weapon_count = (int16_t)(225 /* 0xE1 */ / space);

        int16_t shield_value = AIPOWER::Get_Best_Ship_Shield_Value_(target_idx);

        int32_t max_damage = TECHDATA::_weapons[weapon_idx].max_damage;
        int32_t base_damage = ((max_damage * 3) / 2) - shield_value;

        if (base_damage < 0) {
            base_damage = 0;
        }

        uint32_t total_power = (uint32_t)base_damage * (uint32_t)weapon_count;

        if ((TECHDATA::_weapons[weapon_idx].natural_mods & 8) != 0) {
            total_power *= 3;
        }

        if (total_power > 64000 /* 0xFA00 */) {
            total_power = 64000 /* 0xFA00 */;
        }

        return total_power;
    }

    uint16_t __cdecl Fighter_Garrison_Strength_(int16_t player_idx, int16_t target_player_idx) {
        int16_t pd_weapon_idx = INITSHIP::Best_Point_Defense_Weapon_(player_idx);
        int16_t bomb_weapon_idx = INITSHIP::Best_Bomb_Weapon_(player_idx);
        int16_t bomber_modifier = 0;
        int16_t shield_value = Get_Best_Ship_Shield_Value_(target_player_idx);
        int16_t bomber_count = 0;

        if (MOX::_settings.strategic_combat_flag == 1) {
            return 120;
        }

        int16_t pd_count = 10;
        s_player* player = &MOX::_player[player_idx];

        if (player->tech_applications[TECH_APP_HEAVY_FIGHTER_BAYS] == 3) {
            pd_count = 0;
            bomber_modifier = 0;
            bomber_count = 4;
        } else if (player->tech_applications[TECH_APP_BOMBER_BAYS] == 3) {
            pd_count = 0;
            bomber_modifier = 6;
        }

        int32_t bomb_dmg = (int32_t)TECHDATA::_weapons[bomb_weapon_idx].max_damage - shield_value;
        if (bomb_dmg < 0) {
            bomb_dmg = 0;
        }

        int32_t pd_dmg = (int32_t)TECHDATA::_weapons[pd_weapon_idx].max_damage - shield_value;
        if (pd_dmg < 0) {
            pd_dmg = 0;
        }

        uint16_t base_strength = (int16_t)bomb_dmg * ((bomber_count * 3 / 2) + bomber_modifier) +
                                 (int16_t)pd_dmg * (pd_count + bomber_count * 2);

        base_strength &= 0x3fff;

        uint16_t final_strength = base_strength * 2;
        if (final_strength > 64000 /* 0xFA00 */) {
            final_strength = 64000;
        }

        return final_strength;
    }

    int16_t __cdecl Strength_Hits_Modifier_(int16_t strength) {
        if (strength <= 100) {
            return static_cast<int16_t>(strength + (100 - strength) / 10);
        }

        return static_cast<int16_t>(100 + (strength - 100) / 4);
    }

}