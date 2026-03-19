#include "pch.h"

namespace aipower {
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
                default:
                    // Assembly at 0x00509451 returns uninitialized value from stack
                    int16_t uninitialized_ret = 0;
                    return uninitialized_ret;
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
            attack_bonus = aipower::Base_Generic_Ocv_(ship->owner);
            INITSHIP::Update_Strategic_Ship_(ship_idx);
        } else {
            INITSHIP::Get_Ship_Combat_Bonuses_(ship_idx, &temp_attack, &temp_defense, &attack_bonus);
        }

        defense_bonus = Base_Generic_Dcv_(target_idx);

        for (int i = 0; i < 8; ++i) {
            int16_t weapon_type = *reinterpret_cast<int16_t*>((uint8_t*)&ship->d + 0x1c + i * 8);
            if (weapon_type <= 0) continue;

            int16_t count = *reinterpret_cast<int16_t*>((uint8_t*)&ship->d + 0x1c + i * 8 + 2);
            uint16_t slot_specials = *reinterpret_cast<uint16_t*>((uint8_t*)&ship->d + 0x1c + i * 8 + 5);
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
                        uint8_t ammo = *reinterpret_cast<uint8_t*>((uint8_t*)&ship->d + 0x1c + i * 8 + 4);
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
        int16_t current_shield = *reinterpret_cast<int8_t*>((uint8_t*)&ship->d + 0x12);
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
    int16_t __cdecl Ship_Strength_Vs_Ships_(int16_t attacker_idx, int16_t target_idx) {
        int16_t shield_value = Get_Best_Ship_Shield_Value_(target_idx);
        return Ship_Strength_Vs_Shield_(attacker_idx, target_idx, shield_value);
    }

    /**
     * Calculates the combat strength of an attacking ship against a target ship in a nebula.
     * In a nebula, shields are disabled, so the shield value is set to 0.
     */
    int16_t __cdecl Ship_Strength_Vs_Ships_In_Nebula_(int16_t attacker_idx, int16_t target_idx) {
        return aipower::Ship_Strength_Vs_Shield_(attacker_idx, target_idx, 0);
    }

    int16_t __cdecl Current_Ship_Hits_(int16_t ship_idx) {
        int16_t hits;

        hits = aipower::Max_Ship_Hits_(ship_idx);

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
    void __cdecl Ship_Strength_Vs_Planetary_Shield_(int16_t ship_idx, int16_t* results) {
        uint32_t strengths[4];
        uint16_t shield_reductions[4] = { 2, 7, 12, 22 };

        for (int i = 0; i < 4; ++i) {
            strengths[i] = 0;
        }

        if (MOX::_settings.strategic_combat_flag == 1) {
            uint32_t base_strength = (uint32_t)MOX::_ship[ship_idx].d.size * (uint32_t)MOX::_ship[ship_idx].d.size * 10;
            strengths[0] = base_strength;

            for (int i = 1; i < 4; ++i) {
                // ceil(base_strength / (i + 1))
                strengths[i] = (base_strength + i) / (i + 1);
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

            results[i] = (int16_t)0xfa00; /* 64000 */
            if (final_power <= 0xfa00) {
                results[i] = (int16_t)final_power;
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

}