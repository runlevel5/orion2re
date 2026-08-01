#include "pch.h"

namespace aidesign {
    int16_t _min_weapon_count_table[8] = {1, 2, 3, 5, 7, 10, 0, 0};

    s_spcl _bio_weapon_spcl[5] = {
        {SPECIAL_PHASE_SHIFTER, 0},
        {SPECIAL_SUB_SPACE_TELEPORTER, 0},
        {SPECIAL_AUGMENTED_ENGINES, 0},
        {WEAPON_BIO_TERMINATOR, 1},
        {WEAPON_DEATH_SPORE, 1},
    };
    s_spcl _capture_spcl[5] = {
        {SPECIAL_TRANSPORTERS, 0},
        {WEAPON_TRACTOR_BEAM, 1},
        {WEAPON_ASSAULT_SHUTTLE, 1},
        {SPECIAL_TROOP_PODS, 0},
        {SPECIAL_WARP_DISSIPATOR, 0},
    };
    s_spcl _cloaking_spcl[3] = {
        {SPECIAL_PHASING_CLOAK, 0},
        {SPECIAL_CLOAKING_DEVICE, 0},
        {SPECIAL_STEALTH_FIELD, 0},
    };
    s_spcl _beam_defense_spcl[4] = {
        {SPECIAL_INERTIAL_NULLIFIER, 0},
        {SPECIAL_DISPLACEMENT_DEVICE, 0},
        {SPECIAL_AUGMENTED_ENGINES, 0},
        {SPECIAL_INERTIAL_STABILIZER, 0},
    };
    s_spcl _missile_defense_spcl[8] = {
        {SPECIAL_WIDE_AREA_JAMMER, 0},
        {SPECIAL_MULTI_WAVE_ECM_JAMMER, 0},
        {WEAPON_SPATIAL_COMPRESSOR, 1},
        {SPECIAL_ECM_JAMMER, 0},
        {WEAPON_PULSAR, 1},
        {SPECIAL_DISPLACEMENT_DEVICE, 0},
        {SPECIAL_LIGHTNING_FIELD, 0},
        {WEAPON_ANTI_MISSILE_ROCKET, 1},
    };
    s_spcl _armor_spcl[3] = {
        {SPECIAL_AUTOMATED_REPAIR_UNIT, 0},
        {SPECIAL_HEAVY_ARMOR, 0},
        {SPECIAL_REINFORCED_HULL, 0},
    };
    s_spcl _shield_spcl[3] = {
        {SPECIAL_HARD_SHIELDS, 0},
        {SPECIAL_MULTI_PHASED_SHIELDS, 0},
        {SPECIAL_SHIELD_CAPACITOR, 0},
    };

    s_aidesign_ship_design _ship_design_tables[7][6] = {
        {
            {-1, -1, -1, 15, -1, -1, -1, -1, -1, 85, -1, 0},
            {10, 0, -1, 5, -1, -1, -1, -1, 45, 30, -1, 10},
            {15, 0, -1, 0, -1, -1, -1, 10, 30, 30, -1, 15},
            {15, 0, -1, 5, -1, -1, -1, 15, 25, 25, -1, 15},
            {15, 0, -1, 10, -1, -1, -1, 15, 25, 20, -1, 15},
            {15, 0, -1, 15, -1, -1, -1, 15, 20, 20, -1, 15},
        },
        {
            {-1, -1, -1, 15, -1, -1, 45, -1, -1, 40, -1, 0},
            {10, -1, -1, 5, -1, -1, 40, -1, 15, 20, -1, 10},
            {15, -1, -1, 0, -1, -1, 35, -1, 20, 20, -1, 10},
            {15, -1, -1, 5, -1, -1, 35, -1, 20, 15, -1, 10},
            {15, -1, -1, 10, -1, -1, 35, -1, 15, 15, -1, 10},
            {15, -1, -1, 15, -1, -1, 30, -1, 15, 15, -1, 10},
        },
        {
            {-1, -1, -1, 15, -1, -1, -1, 45, -1, 40, -1, 0},
            {10, -1, -1, 5, -1, -1, -1, 40, 15, 20, -1, 10},
            {15, -1, -1, 0, -1, -1, -1, 35, 20, 20, -1, 10},
            {15, -1, -1, 5, -1, -1, -1, 35, 20, 15, -1, 10},
            {15, -1, -1, 10, -1, -1, -1, 35, 15, 15, -1, 10},
            {15, -1, -1, 15, -1, -1, -1, 30, 15, 15, -1, 10},
        },
        {
            {-1, -1, -1, 15, -1, -1, -1, 85, -1, -1, 0, 0},
            {10, -1, 0, 5, -1, -1, -1, 75, -1, -1, 0, 10},
            {15, -1, 0, 0, -1, -1, -1, 65, -1, -1, 10, 10},
            {15, -1, 0, 5, -1, -1, -1, 50, -1, 10, 10, 10},
            {15, -1, 0, 10, -1, -1, -1, 45, -1, 10, 10, 10},
            {15, -1, 0, 15, -1, -1, -1, 40, -1, 10, 10, 10},
        },
        {
            {-1, -1, -1, 15, -1, -1, 85, -1, -1, -1, 0, 0},
            {10, -1, -1, 5, -1, -1, 65, -1, -1, 5, 5, 10},
            {15, -1, -1, 0, -1, -1, 60, -1, -1, 10, 5, 10},
            {15, -1, -1, 5, -1, -1, 55, -1, -1, 10, 5, 10},
            {15, -1, -1, 10, -1, -1, 45, -1, -1, 15, 5, 10},
            {15, -1, -1, 15, -1, -1, 40, -1, -1, 15, 5, 10},
        },
        {
            {-1, -1, -1, 15, -1, 50, -1, -1, -1, 35, -1, 0},
            {10, -1, -1, 5, -1, 40, -1, -1, 20, 15, -1, 10},
            {15, -1, -1, 0, -1, 35, -1, -1, 20, 20, -1, 10},
            {15, -1, -1, 5, -1, 30, -1, -1, 20, 20, -1, 10},
            {15, -1, -1, 10, -1, 25, -1, -1, 20, 20, -1, 10},
            {15, -1, -1, 15, -1, 20, -1, -1, 20, 20, -1, 10},
        },
        {
            {-1, -1, -1, 15, 60, -1, -1, -1, -1, 25, 0, 0},
            {10, -1, -1, 5, 50, -1, -1, -1, 15, 10, 0, 10},
            {15, -1, -1, 0, 45, -1, -1, -1, 15, 15, 0, 10},
            {15, -1, -1, 5, 40, -1, -1, -1, 15, 15, 0, 10},
            {15, -1, -1, 10, 35, -1, -1, -1, 15, 15, 0, 10},
            {15, -1, -1, 15, 60, -1, -1, -1, 0, 0, 0, 10},
        },
    };
    s_spcl _beam_offense_spcl[7] = {
        {SPECIAL_TIME_WARP_FACILITATOR, 0},
        {SPECIAL_ACHILLES_TARGETING_UNIT, 0},
        {SPECIAL_HIGH_ENERGY_FOCUS, 0},
        {SPECIAL_RANGEMASTER_UNIT, 0},
        {SPECIAL_STRUCTURAL_ANALYZER, 0},
        {SPECIAL_HYPER_X_CAPACITORS, 0},
        {SPECIAL_BATTLE_SCANNER, 0},
    };
    s_spcl _missile_offense_spcl[2] = {
        {SPECIAL_TIME_WARP_FACILITATOR, 0},
        {SPECIAL_FAST_MISSILE_RACKS, 0},
    };
    s_spcl _special_weapons_spcl[5] = {
        {WEAPON_STELLAR_CONVERTER, 1},
        {WEAPON_BLACK_HOLE_GENERATOR, 1},
        {SPECIAL_ENERGY_ABSORBER, 0},
        {WEAPON_PLASMA_WEB, 1},
        {WEAPON_GYRO_DESTABILIZER, 1},
    };
    s_spcl _special_device_spcl[12] = {
        {SPECIAL_QUANTUM_DETONATOR, 0},
        {WEAPON_SPATIAL_COMPRESSOR, 1},
        {WEAPON_PULSAR, 1},
        {WEAPON_STASIS_FIELD, 1},
        {SPECIAL_PHASING_CLOAK, 0},
        {SPECIAL_CLOAKING_DEVICE, 0},
        {SPECIAL_STEALTH_FIELD, 0},
        {SPECIAL_REFLECTION_FIELD, 0},
        {SPECIAL_PHASE_SHIFTER, 0},
        {SPECIAL_SUB_SPACE_TELEPORTER, 0},
        {SPECIAL_WARP_DISSIPATOR, 0},
        {SPECIAL_SCOUT_LAB, 0},
    };
    s_spcl _defense_specials_f_spcl[20] = {
        {SPECIAL_SECURITY_STATIONS, 0},
        {SPECIAL_DISPLACEMENT_DEVICE, 0},
        {SPECIAL_LIGHTNING_FIELD, 0},
        {SPECIAL_HARD_SHIELDS, 0},
        {SPECIAL_WIDE_AREA_JAMMER, 0},
        {SPECIAL_MULTI_WAVE_ECM_JAMMER, 0},
        {SPECIAL_HEAVY_ARMOR, 0},
        {SPECIAL_INERTIAL_NULLIFIER, 0},
        {SPECIAL_ECM_JAMMER, 0},
        {SPECIAL_REINFORCED_HULL, 0},
        {SPECIAL_INERTIAL_STABILIZER, 0},
        {SPECIAL_AUGMENTED_ENGINES, 0},
        {SPECIAL_SHIELD_CAPACITOR, 0},
        {SPECIAL_MULTI_PHASED_SHIELDS, 0},
        {WEAPON_ANTI_MISSILE_ROCKET, 1},
        {SPECIAL_REFLECTION_FIELD, 0},
        {WEAPON_SPATIAL_COMPRESSOR, 1},
        {WEAPON_PULSAR, 1},
        {SPECIAL_AUTOMATED_REPAIR_UNIT, 0},
        {SPECIAL_NO_SPECIAL, 0},
    };
    s_spcl _defense_specials_t_spcl[20] = {
        {SPECIAL_SECURITY_STATIONS, 0},
        {SPECIAL_DISPLACEMENT_DEVICE, 0},
        {SPECIAL_LIGHTNING_FIELD, 0},
        {SPECIAL_HARD_SHIELDS, 0},
        {SPECIAL_WIDE_AREA_JAMMER, 0},
        {SPECIAL_MULTI_WAVE_ECM_JAMMER, 0},
        {SPECIAL_HEAVY_ARMOR, 0},
        {SPECIAL_INERTIAL_NULLIFIER, 0},
        {SPECIAL_ECM_JAMMER, 0},
        {SPECIAL_REINFORCED_HULL, 0},
        {SPECIAL_INERTIAL_STABILIZER, 0},
        {SPECIAL_AUGMENTED_ENGINES, 0},
        {SPECIAL_SHIELD_CAPACITOR, 0},
        {SPECIAL_MULTI_PHASED_SHIELDS, 0},
        {WEAPON_ANTI_MISSILE_ROCKET, 1},
        {SPECIAL_REFLECTION_FIELD, 0},
        {WEAPON_SPATIAL_COMPRESSOR, 1},
        {WEAPON_PULSAR, 1},
        {SPECIAL_AUTOMATED_REPAIR_UNIT, 0},
        {SPECIAL_NO_SPECIAL, 0},
    };

    void __cdecl Determine_Beam_Weapon_Modifications_(
        int16_t player_idx,
        int16_t hull_size,
        int16_t* type0,
        int16_t* spec0,
        int16_t* arc0,
        int16_t* type1,
        int16_t* spec1,
        int16_t* arc1,
        int16_t* type2,
        int16_t* spec2,
        int16_t* arc2
    ) {
        int16_t weapon_count = 0;
        int16_t weapon_id[40];
        int16_t weapon_mods[40];
        int16_t ai_group[40];
        int16_t spec2_mods[40];
        int16_t spec1_mods[40];

        for (int16_t i = 1; i <= WEAPON_TRACTOR_BEAM; ++i) {
            if (TECHDATA::_weapons[i].type == WEAPON_TYPE_BEAM && MOX::_player[player_idx].tech_applications[TECHDATA::_weapons[i].tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                weapon_id[weapon_count] = i;
                spec1_mods[weapon_count] = 0;
                spec2_mods[weapon_count] = 0;
                weapon_mods[weapon_count] = 0;
                ai_group[weapon_count] = TECHDATA::_technology_fields[TECHDATA::_technology_applications[TECHDATA::_weapons[i].tech_app_id].tech_field_id].ai_group;
                weapon_count++;
            }
        }

        if (weapon_count == 0) {
            *type2 = 0;
            *type1 = 0;
            *type0 = 0;
            return;
        }

        int16_t max_idx = weapon_count - 1;
        bool swapped;
        if (max_idx > 0) {
            do {
                swapped = false;
                for (int16_t i = 0; i < max_idx; ++i) {
                    if (ai_group[i + 1] < ai_group[i]) {
                        int16_t temp_ai = ai_group[i];
                        ai_group[i] = ai_group[i + 1];
                        ai_group[i + 1] = temp_ai;

                        int16_t temp_id = weapon_id[i];
                        weapon_id[i] = weapon_id[i + 1];
                        weapon_id[i + 1] = temp_id;

                        swapped = true;
                    }
                }
            } while (swapped);
        }

        int16_t first_living_player = 0;
        while (MOX::_player[first_living_player].eliminated != 0) {
            first_living_player++;
        }

        int16_t best_comp = INITSHIP::Best_Computer_(player_idx);
        int16_t comp_bonus = TECHDATA::_computers[best_comp].bonus;
        comp_bonus += MOX::_player[player_idx].traits[TRAIT_SHIP_ATTACK];

        int16_t target_defense;
        int16_t target_shield_value;

        if (MOX::_player[player_idx].objectives == PLAYER_OBJECTIVE_HUMAN) {
            target_shield_value = 1;
            target_defense = TECHDATA::_drives[DRIVE_FUSION_DRIVE].combat_speed_min[HULL_CRUISER] + 20;
        } else {
            target_shield_value = AIPOWER::Get_Best_Ship_Shield_Value_(first_living_player);
            int16_t best_drive = INITSHIP::Best_Warp_Drive_(first_living_player);
            target_defense = MOX::_player[first_living_player].traits[TRAIT_SHIP_DEFENSE] + TECHDATA::_drives[best_drive].combat_speed_min[2] * 5;
        }

        int16_t net_bonus = comp_bonus - target_defense;

        for (int16_t i = 0; i < weapon_count; ++i) {
            int16_t w_id = weapon_id[i];
            int16_t min_lvl = DESIGN::Miniaturization_Level_(player_idx, TECHDATA::_weapons[w_id].tech_app_id);
            if (min_lvl > 5) min_lvl = 5;

            int16_t cur_bonus = net_bonus;

            if (TECHDATA::_weapon_modifications[WEAPON_MOD_CONTINUOUS].level <= min_lvl) {
                weapon_mods[i] += WEAPON_MOD_FLAG_CONTINUOUS;
                if (net_bonus < 50) {
                    spec1_mods[i] += WEAPON_MOD_FLAG_CONTINUOUS;
                    spec2_mods[i] += WEAPON_MOD_FLAG_CONTINUOUS;
                    cur_bonus = net_bonus + 25;
                }
            }

            if (cur_bonus > 0 && TECHDATA::_weapon_modifications[WEAPON_MOD_NO_RANGE_DISSIPATION].level <= min_lvl) {
                spec1_mods[i] += WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION;
                spec2_mods[i] += WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION;
            }

            if (cur_bonus >= 0 && TECHDATA::_weapon_modifications[WEAPON_MOD_AUTO_FIRE].level <= min_lvl) {
                if (cur_bonus > 30) {
                    weapon_mods[i] += WEAPON_MOD_FLAG_AUTO_FIRE;
                }
                spec1_mods[i] += WEAPON_MOD_FLAG_AUTO_FIRE;
                spec2_mods[i] += WEAPON_MOD_FLAG_AUTO_FIRE;
            }

            uint16_t avail = TECHDATA::_weapons[w_id].available_mods;

            weapon_mods[i] = (weapon_mods[i] + WEAPON_MOD_FLAG_POINT_DEFENSE) & avail;
            if ((weapon_mods[i] & WEAPON_MOD_FLAG_POINT_DEFENSE) == 0) {
                weapon_mods[i] = 0;
            }

            if (TECHDATA::_weapon_modifications[WEAPON_MOD_ARMOR_PIERCING].level <= min_lvl) {
                spec1_mods[i] += WEAPON_MOD_FLAG_ARMOR_PIERCING;
                spec2_mods[i] += WEAPON_MOD_FLAG_ARMOR_PIERCING;
            }

            if (TECHDATA::_weapon_modifications[WEAPON_MOD_SHIELD_PIERCING].level <= min_lvl) {
                spec1_mods[i] += WEAPON_MOD_FLAG_SHIELD_PIERCING;
                spec2_mods[i] += WEAPON_MOD_FLAG_SHIELD_PIERCING;
            }

            if (TECHDATA::_weapon_modifications[WEAPON_MOD_ENVELOPING].level <= min_lvl) {
                spec1_mods[i] += WEAPON_MOD_FLAG_ENVELOPING;
                spec2_mods[i] += WEAPON_MOD_FLAG_ENVELOPING;
            }

            spec1_mods[i] &= avail;

            spec2_mods[i] = (spec2_mods[i] + 2) & avail;
            if ((spec2_mods[i] & 2) == 0) {
                spec2_mods[i] = 0;
            }
        }

        if (MOX::_settings.game_difficulty < 2) {
            int16_t idx1 = weapon_count - 1;
            while (idx1 >= 0) {
                if ((TECHDATA::_weapons[weapon_id[idx1]].available_mods & WEAPON_MOD_FLAG_POINT_DEFENSE) != 0) {
                    break;
                }
                idx1--;
            }

            if (idx1 < 0) {
                *type0 = 0;
                *spec0 = 0;
            } else {
                *type0 = weapon_id[idx1];
                *spec0 = WEAPON_MOD_FLAG_POINT_DEFENSE;
            }

            *type1 = weapon_id[weapon_count - 1];
            *spec1 = 0;

            int16_t idx2 = weapon_count - 1;
            while (idx2 >= 0) {
                if ((TECHDATA::_weapons[weapon_id[idx2]].available_mods & WEAPON_MOD_FLAG_HEAVY_MOUNT) != 0) {
                    break;
                }
                idx2--;
            }

            if (idx2 < 0) {
                *type2 = 0;
                *spec2 = 0;
            } else {
                *type2 = weapon_id[idx2];
                *spec2 = WEAPON_MOD_FLAG_HEAVY_MOUNT;
            }
        } else {
            if (weapon_count == 1) {
                if ((TECHDATA::_weapons[weapon_id[0]].available_mods & WEAPON_MOD_FLAG_POINT_DEFENSE) == 0) {
                    *type0 = 0;
                } else {
                    *type0 = weapon_id[0];
                    *spec0 = weapon_mods[0] & 0x7ffd;
                }

                *type1 = weapon_id[0];
                *spec1 = spec1_mods[0] & 0x7ff9;

                if ((TECHDATA::_weapons[weapon_id[0]].available_mods & WEAPON_MOD_FLAG_HEAVY_MOUNT) == 0) {
                    *type2 = 0;
                } else {
                    *type2 = weapon_id[0];
                    *spec2 = spec2_mods[0] & 0x7ffb;
                }
            } else {
                int16_t pd_weapon = INITSHIP::Best_Point_Defense_Weapon_By_Size_(player_idx);
                int16_t pd_idx = 0;
                for (pd_idx = 0; pd_idx < weapon_count; pd_idx++) {
                    if (weapon_id[pd_idx] == pd_weapon) {
                        break;
                    }
                }

                if (pd_idx >= weapon_count) {
                    *type0 = 0;
                } else {
                    *type0 = weapon_id[pd_idx];
                    *spec0 = weapon_mods[pd_idx] & 0x7ffd;
                }

                int16_t max_damage_idx = weapon_count - 1;
                if (max_damage_idx < 0) {
                    *type1 = 0;
                } else {
                    int16_t best_idx = 0;
                    if (max_damage_idx != 0) {
                        int32_t best_val = 0;
                        for (int16_t i = 0; i < weapon_count; i++) {
                            int16_t w_id = weapon_id[i];
                            if (w_id != 0) {
                                int32_t space = DESIGN::Weapon_Space_(player_idx, w_id, 1, 1, 1, 0);
                                uint16_t natural_mods = TECHDATA::_weapons[w_id].natural_mods;
                                int32_t damage = TECHDATA::_weapons[w_id].max_damage;

                                if ((natural_mods & 0x100) == 0) {
                                    damage -= target_shield_value;
                                }

                                int32_t val = (damage * 10) / space;

                                if ((natural_mods & 0x20) != 0) {
                                    val = (val * 3) / 2;
                                }
                                if ((natural_mods & 0x4) != 0) {
                                    val = (val * 5) / 4;
                                }
                                if ((natural_mods & 0x400) != 0) {
                                    val = (val * 5) / 4;
                                }
                                if ((natural_mods & 0x200) != 0) {
                                    val *= 2;
                                }
                                if ((natural_mods & 0x10) != 0 && MOX::_player[player_idx].ship_special_theme == 1) {
                                    val *= 2;
                                }

                                if (val > best_val) {
                                    best_val = val;
                                    best_idx = i;
                                }
                            }
                        }
                    }
                    *type1 = weapon_id[best_idx];
                    *spec1 = spec1_mods[best_idx] & 0x7ff9;
                }

                if (max_damage_idx < 0) {
                    *type2 = 0;
                    *spec2 = 0;
                } else if (weapon_count == 1 && (TECHDATA::_weapons[weapon_id[0]].available_mods & WEAPON_MOD_FLAG_HEAVY_MOUNT) == 0) {
                    *type2 = 0;
                } else {
                    int16_t best_idx = 0;
                    if (max_damage_idx != 0) {
                        int32_t best_val = 0;
                        for (int16_t i = 0; i < weapon_count; i++) {
                            int16_t w_id = weapon_id[i];
                            if (w_id != 0) {
                                int32_t space = DESIGN::Weapon_Space_(player_idx, w_id, 1, 1, 1, 2);
                                uint16_t natural_mods = TECHDATA::_weapons[w_id].natural_mods;
                                int32_t damage = TECHDATA::_weapons[w_id].max_damage * 3;

                                if ((natural_mods & 0x100) == 0) {
                                    damage = damage / 2 - target_shield_value;
                                } else {
                                    damage = damage / 2;
                                }

                                int32_t val = (damage * 10) / space;

                                if ((natural_mods & 0x20) != 0) {
                                    val = (val * 3) / 2;
                                }
                                if ((natural_mods & 0x4) != 0) {
                                    val = (val * 5) / 4;
                                }
                                if ((natural_mods & 0x400) != 0) {
                                    val = (val * 5) / 4;
                                }
                                if ((natural_mods & 0x200) != 0) {
                                    val *= 2;
                                }

                                if (val > best_val) {
                                    best_val = val;
                                    best_idx = i;
                                }
                            }
                        }
                    }
                    *type2 = weapon_id[best_idx];
                    *spec2 = spec2_mods[best_idx] & 0x7ffb;
                }
            }
        }

        *arc1 = 1;
        *arc2 = 1;
        *arc0 = 1;

        int16_t comp_tech = Get_Competition_Player_Tech_Values_(player_idx, 0x12);
        int16_t r_base = (int16_t)((0x14 - comp_tech) * hull_size);

        int16_t spec0_mod_count = 0;
        for (uint16_t m = WEAPON_MOD_FLAG_ARMOR_PIERCING; m < WEAPON_MOD_FLAG_MIRV; m *= 2) {
            if ((m & *spec0) != 0) {
                spec0_mod_count++;
            }
        }

        int16_t spec1_mod_count = 0;
        for (uint16_t m = WEAPON_MOD_FLAG_ARMOR_PIERCING; m < WEAPON_MOD_FLAG_MIRV; m *= 2) {
            if ((m & *spec1) != 0) {
                spec1_mod_count++;
            }
        }

        int16_t spec2_mod_count = 0;
        for (uint16_t m = WEAPON_MOD_FLAG_ARMOR_PIERCING; m < WEAPON_MOD_FLAG_MIRV; m *= 2) {
            if ((m & *spec2) != 0) {
                spec2_mod_count++;
            }
        }

        int32_t arc0_threshold = (int32_t)r_base + (int32_t)spec0_mod_count * 20;
        if ((int32_t)game_random::Random_(100) < arc0_threshold) {
            int32_t arc0_inner_threshold = (int32_t)(r_base / 2) + (int32_t)spec0_mod_count * 20;
            if ((int32_t)game_random::Random_(100) < arc0_inner_threshold) {
                *arc0 = 16;
            } else {
                *arc0 = 2;
            }
        }

        int32_t arc1_threshold = (int32_t)spec1_mod_count * 20 + (int32_t)(r_base / 2);
        if ((int32_t)game_random::Random_(100) < arc1_threshold) {
            *arc1 = 2;
        }

        int32_t arc2_threshold = (int32_t)spec2_mod_count * 20 + (int32_t)(r_base / 4);
        if ((int32_t)game_random::Random_(100) < arc2_threshold) {
            *arc2 = 2;
        }
    }

    int16_t __cdecl Add_Specials_To_Design_(struct s_spcl* list, int16_t count, int16_t player_idx, struct s_ship_design* design, int16_t space_available, int16_t* weapon_slot_idx_ptr) {
        int16_t total_space_used = 0;
        int16_t already_has_fighter_bay = 0;

        for (int16_t i = 0; i < count; ++i) {
            int16_t item_id = list[i].id;

            if (list[i].type == 0) {
                int16_t tech_app_id = TECHDATA::_specials[item_id].tech_app_id;

                if (MOX::_player[player_idx].tech_applications[tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    if (Special_Legal_For_Design_(design, item_id)) {
                        // Check if player has Stealth trait, if so skip Stealth Field device
                        if (MOX::_player[player_idx].traits[TRAIT_STEALTHY_SHIPS] == 0 || item_id != SPECIAL_STEALTH_FIELD) {
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
                if (*weapon_slot_idx_ptr < 8) {
                    if (item_id == 0x21 /* WPN_FIGHTER_BAY */) {
                        for (int16_t slot = 0; slot < *weapon_slot_idx_ptr; ++slot) {
                            if (design->ship_weapon[slot].type == 0x21 /* WPN_FIGHTER_BAY */) {
                                already_has_fighter_bay = 1;
                            }
                        }
                    }

                    int16_t weapon_tech_app_id = TECHDATA::_weapons[item_id].tech_app_id;
                    if (MOX::_player[player_idx].tech_applications[weapon_tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED && !already_has_fighter_bay) {
                        int32_t weapon_unit_space = DESIGN::Weapon_Space_(player_idx, item_id, 1, 16 /* ARC_360 */, 1, 0);
                        int16_t max_count = (int16_t)(space_available / weapon_unit_space);

                        if (item_id == 0x21 /* WPN_FIGHTER_BAY */) {
                            if (max_count > (int16_t)design->size * 10) {
                                max_count = (int16_t)design->size * 10;
                            }
                        }

                        if (max_count > 0) {
                            int16_t items_this_type = 0;

                            design->ship_weapon[*weapon_slot_idx_ptr].type = item_id;
                            design->ship_weapon[*weapon_slot_idx_ptr].firing_arc = 16;
                            design->ship_weapon[*weapon_slot_idx_ptr].specials = 0;
                            design->ship_weapon[*weapon_slot_idx_ptr].ammo = 1;

                            while (max_count > 99) {
                                if (*weapon_slot_idx_ptr >= 8) {
                                    break;
                                }

                                items_this_type += 99;
                                design->ship_weapon[*weapon_slot_idx_ptr].count = 99;
                                design->ship_weapon[*weapon_slot_idx_ptr].current_count = 99;
                                max_count -= 99;
                                ++(*weapon_slot_idx_ptr);

                                if (*weapon_slot_idx_ptr < 8) {
                                    design->ship_weapon[*weapon_slot_idx_ptr].type = item_id;
                                    design->ship_weapon[*weapon_slot_idx_ptr].firing_arc = 16;
                                    design->ship_weapon[*weapon_slot_idx_ptr].specials = 0;
                                    design->ship_weapon[*weapon_slot_idx_ptr].ammo = 1;
                                }
                            }

                            if (*weapon_slot_idx_ptr < 8) {
                                items_this_type += max_count;
                                design->ship_weapon[*weapon_slot_idx_ptr].count = (int8_t)max_count;
                                design->ship_weapon[*weapon_slot_idx_ptr].current_count = (int8_t)max_count;
                            }

                            ++(*weapon_slot_idx_ptr);

                            int16_t space_calc = (int16_t)DESIGN::Weapon_Space_(player_idx, item_id, 1, 16 /* ARC_360 */, 1, 0);
                            int16_t used_space = (int16_t)(space_calc * items_this_type);
                            total_space_used = (int16_t)(total_space_used + used_space);
                            space_available = (int16_t)(space_available - used_space);
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

        specials[0] |= WEAPON_MOD_FLAG_POINT_DEFENSE;
        specials[2] |= WEAPON_MOD_FLAG_HEAVY_MOUNT;

        int16_t weapon_type = types[weapon_class];
        int16_t weapon_specials = specials[weapon_class];
        int16_t weapon_arc = arcs[weapon_class];

        int32_t unit_space = DESIGN::Weapon_Space_(player_idx, weapon_type, 1, (uint8_t)weapon_arc, 0, weapon_specials);
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

        int32_t final_unit_space = DESIGN::Weapon_Space_(player_idx, weapon_type, 1, (uint8_t)weapon_arc, 0, weapon_specials);
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
        int16_t result = 0;

        auto get_default_class = [&]() -> int16_t {
            return (player->tech_applications[TECH_APP_LASER_CANNON] == TECH_RESEARCH_STATUS_RESEARCHED) ? 0 : 3;
        };

        auto get_fighters_fallback = [&]() -> int16_t {
            return (fighters_avail != 0) ? 1 : 2;
        };

        switch ((uint8_t)player->ship_design_theme) {
            case 0:
            case 100: {
                switch ((int16_t)game_random::Random_(20)) {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                        return get_default_class();
                    case 14:
                        return 3;
                    case 15:
                        return (fighters_avail != 0) ? 4 : get_default_class();
                    case 16:
                        return (fighters_avail != 0) ? 1 : get_default_class();
                    case 17:
                        return 2;
                    case 18:
                    case 19:
                        return (special_weapons_avail != 0) ? 6 : get_default_class();
                    case 20:
                        return (special_devices_avail != 0) ? 5 : get_default_class();
                    default:
                        return 0;
                }
            }

            case 1: {
                switch ((int16_t)game_random::Random_(20)) {
                    case 1:
                        return get_default_class();
                    case 2:
                        return 3;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                        return (fighters_avail != 0) ? 4 : get_default_class();
                    case 16:
                    case 17:
                    case 18:
                        return get_fighters_fallback();
                    case 19:
                        return (special_weapons_avail != 0) ? 6 : get_fighters_fallback();
                    case 20:
                        return (special_devices_avail != 0) ? 5 : get_fighters_fallback();
                    default:
                        return 0;
                }
            }

            case 2:
                switch ((int16_t)game_random::Random_(20)) {
                    case 1:
                        result = get_default_class();
                        break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                        result = 3;
                        break;
                    case 15:
                        result = (fighters_avail != 0) ? 4 : 3;
                        break;
                    case 16:
                    case 17:
                    case 18:
                        result = 2;
                        break;
                    case 19:
                        result = (special_weapons_avail != 0) ? 6 : 2;
                        break;
                    case 20:
                        result = (special_devices_avail != 0) ? 5 : 2;
                        break;
                    default:
                        break;
                }
                [[fallthrough]];

            case 3:
                switch ((int16_t)game_random::Random_(20)) {
                    case 1:
                        return get_default_class();
                    case 2:
                        return 3;
                    case 3:
                        return (fighters_avail != 0) ? 4 : get_default_class();
                    case 4:
                    case 5:
                        return get_fighters_fallback();
                    case 6:
                    case 7:
                        return 2;
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        return (special_weapons_avail != 0) ? 6 : get_fighters_fallback();
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                        return (special_weapons_avail != 0) ? 6 : 2;
                    case 16:
                    case 17:
                        return (special_devices_avail != 0) ? 5 : get_fighters_fallback();
                    case 18:
                    case 19:
                    case 20:
                        result = (special_devices_avail != 0) ? 5 : 2;
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;
        }

        return result;
    }

    // ORACLE: The original ignores player_idx and always checks player 0's technology state.
    int16_t __cdecl Get_Competition_Player_Tech_Values_(int16_t, int16_t tech_type) {
        int16_t max_ai_group = 0;
        int16_t i = 0;

        do {
            auto& app = TECHDATA::_technology_applications[i];

            if (app.ai_group == tech_type) {
                if (MOX::_player[0].tech_applications[i] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    int16_t field_id = app.tech_field_id;
                    uint8_t field_group = TECHDATA::_technology_fields[field_id].ai_group;

                    if (max_ai_group < field_group) {
                        max_ai_group = field_group;
                    }
                }
            }
            i++;
        } while (i < TECH_APP_COUNT);

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

        if (player.tech_applications[TECH_APP_GYRO_DESTABILIZER] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player.tech_applications[TECH_APP_ENERGY_ABSORBER] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player.tech_applications[TECH_APP_BLACK_HOLE_GENERATOR] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player.tech_applications[TECH_APP_STELLAR_CONVERTER] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player.tech_applications[TECH_APP_PLASMA_WEB] == TECH_RESEARCH_STATUS_RESEARCHED) {
            return 1;
        }

        return 0;
    }

    int16_t __cdecl Special_Devices_Available_(int16_t player_idx) {
        for (int16_t i = 0; i < 12; ++i) {
            int16_t tech_app_id;
            int16_t id = _special_device_spcl[i].id;
            int16_t type = _special_device_spcl[i].type;

            if (type == 0) {
                tech_app_id = (int16_t)TECHDATA::_specials[id].tech_app_id;
            } else {
                tech_app_id = (int16_t)TECHDATA::_weapons[id].tech_app_id;
            }

            if (MOX::_player[player_idx].tech_applications[tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                return 1;
            }
        }

        return 0;
    }

    int16_t __cdecl Fighters_Available_(int16_t player_idx) {
        const auto& player = MOX::_player[player_idx];

        if (player.tech_applications[TECH_APP_FIGHTER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player.tech_applications[TECH_APP_BOMBER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player.tech_applications[TECH_APP_HEAVY_FIGHTER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            return 1;
        }

        return 0;
    }

    int16_t __cdecl Add_Fighters_To_Design_(int16_t player_idx, s_ship_design* design, int16_t* weapon_slot_idx, int16_t available_space) {
        int16_t weapon_type;
        int space_per_weapon;
        int16_t total_fighters = 0;
        int16_t total_space_used = 0;

        if (MOX::_player[player_idx].tech_applications[TECH_APP_HEAVY_FIGHTER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            space_per_weapon = DESIGN::Weapon_Space_(player_idx, WEAPON_HEAVY_FIGHTER, 1, 0x10, 1, 0);
            if (available_space / space_per_weapon > 0) {
                weapon_type = WEAPON_HEAVY_FIGHTER;
                goto calculate_fighters;
            }
        }

        if (MOX::_player[player_idx].tech_applications[TECH_APP_BOMBER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED) {
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

        for (int i = 1; i <= WEAPON_TRACTOR_BEAM; ++i) {
            if (TECHDATA::_weapons[i].type == WEAPON_TYPE_BOMB && (TECHDATA::_weapons[i].natural_mods & WEAPON_MOD_FLAG_AUTO_FIRE) == 0) {
                int16_t tech_app_id = TECHDATA::_weapons[i].tech_app_id;
                if (MOX::_player[player_idx].tech_applications[tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
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

        uint16_t miniaturization_tech_value = Get_Competition_Player_Tech_Values_(comp_idx, 0x21 /* AI_GROUP_MINIATURIZATION */);
        uint16_t force_field_tech_value = Get_Competition_Player_Tech_Values_(comp_idx, 10 /* AI_GROUP_FORCE_FIELDS */);
        int threshold = (miniaturization_tech_value * 2 + 2) / 3 + force_field_tech_value;

        int i = 0;
        for (; i < num_candidates; ++i) {
            int16_t tech_app_id = TECHDATA::_weapons[candidates[i]].tech_app_id;
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

    void __cdecl Determine_Missile_Weapon_Modifications_(
        int16_t player_idx,
        uint16_t* out_weapon_id,
        uint16_t* out_weapon_mods,
        int16_t weapon_count_table_idx,
        int16_t available_space,
        int16_t* out_weapon_count
    ) {
        uint16_t sort_keys[20];
        uint16_t weapon_indices[20];
        uint16_t weapon_mods[20];
        int16_t count = 0;

        for (int16_t i = 1; i <= WEAPON_TRACTOR_BEAM; ++i) {
            uint8_t type = TECHDATA::_weapons[i].type;
            if (type == WEAPON_TYPE_MISSILE || type == WEAPON_TYPE_TORPEDO) {
                uint16_t tech_app_id = TECHDATA::_weapons[i].tech_app_id;
                if (MOX::_player[player_idx].tech_applications[tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    weapon_indices[count] = i;
                    weapon_mods[count] = 0;
                    int16_t tech_field_id = TECHDATA::_technology_applications[tech_app_id].tech_field_id;
                    sort_keys[count] = (uint16_t)TECHDATA::_technology_fields[tech_field_id].ai_group;
                    count++;
                }
            }
        }

        if (count == 0) {
            *out_weapon_id = 0;
            return;
        }

        int16_t swapped;
        do {
            swapped = 0;
            if (count - 1 > 0) {
                for (int16_t i = 0; i < count - 1; ++i) {
                    if (sort_keys[i + 1] < sort_keys[i]) {
                        uint16_t temp_idx = weapon_indices[i + 1];
                        uint16_t temp_key = sort_keys[i];
                        uint16_t temp_idx2 = weapon_indices[i];

                        sort_keys[i] = sort_keys[i + 1];
                        sort_keys[i + 1] = temp_key;

                        weapon_indices[i] = temp_idx;
                        weapon_indices[i + 1] = temp_idx2;

                        swapped = 1;
                    }
                }
            }
        } while (swapped == 1);

        int16_t competitor_idx = 0;
        while (MOX::_player[competitor_idx].eliminated != 0) {
            competitor_idx++;
        }

        int16_t max_shield = AIPOWER::Get_Best_Ship_Shield_Value_(competitor_idx);
        int16_t comp_tech = (int16_t)Get_Competition_Player_Tech_Values_(competitor_idx, 0x1C);

        if (count > 0) {
            for (int16_t i = 0; i < count; ++i) {
                uint16_t w_idx = weapon_indices[i];
                if (TECHDATA::_weapons[w_idx].max_damage < max_shield + 4) {
                    weapon_indices[i] = 0;
                    sort_keys[i] = 0;
                    out_weapon_mods[i] = 0;
                } else {
                    int16_t mini_level = DESIGN::Miniaturization_Level_(player_idx, TECHDATA::_weapons[w_idx].tech_app_id);
                    if (mini_level > 5) {
                        mini_level = 5;
                    }

                    if (mini_level >= 1) {
                        if (comp_tech > 0) {
                            weapon_mods[i] += WEAPON_MOD_FLAG_ECCM;
                        }
                        weapon_mods[i] += WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION | WEAPON_MOD_FLAG_HEAVILY_ARMORED | WEAPON_MOD_FLAG_FAST | WEAPON_MOD_FLAG_OVERLOADED;
                    }

                    if (mini_level >= 2) {
                        weapon_mods[i] += WEAPON_MOD_FLAG_ENVELOPING | WEAPON_MOD_FLAG_MIRV;
                    }

                    if (MOX::_player[player_idx].tech_applications[TECH_APP_EMISSIONS_GUIDANCE_SYSTEM] == TECH_RESEARCH_STATUS_RESEARCHED) {
                        if (mini_level >= 2) {
                            weapon_mods[i] += WEAPON_MOD_FLAG_EMISSION_GUIDANCE;
                        }
                    }

                    weapon_mods[i] &= TECHDATA::_weapons[w_idx].available_mods;
                }
            }
        }

        if (MOX::_settings.game_difficulty < 2) {
            *out_weapon_id = weapon_indices[count - 1];
            *out_weapon_mods = 0;
            return;
        }

        if (count == 1) {
            *out_weapon_id = weapon_indices[0];
            *out_weapon_mods = weapon_mods[0];
        } else {
            int32_t best_score = 0;
            int16_t best_index = 0;

            if (count > 0) {
                for (int16_t i = 0; i < count; ++i) {
                    uint16_t w_idx = weapon_indices[i];
                    if (w_idx != 0) {
                        int32_t space = DESIGN::Weapon_Space_(player_idx, w_idx, 1, 1, 5, 0);
                        int32_t score = (((int32_t)TECHDATA::_weapons[w_idx].max_damage - max_shield) * 10) / space;
                        if (best_score < score) {
                            best_index = i;
                            best_score = score;
                        }
                    }
                }
            }

            *out_weapon_id = weapon_indices[best_index];
            *out_weapon_mods = weapon_mods[best_index] & 0x7FF9;
        }

        uint16_t mods = *out_weapon_mods;
        int32_t space = DESIGN::Weapon_Space_(player_idx, *out_weapon_id, 1, 1, *out_weapon_count, mods);

        if ((available_space / space) < _min_weapon_count_table[weapon_count_table_idx]) {
            if (*out_weapon_count > 5) {
                *out_weapon_count = 5;
            }

            space = DESIGN::Weapon_Space_(player_idx, *out_weapon_id, 1, 1, *out_weapon_count, mods);

            if ((available_space / space) < _min_weapon_count_table[weapon_count_table_idx]) {
                if (mods & WEAPON_MOD_FLAG_HEAVILY_ARMORED) {
                    mods ^= WEAPON_MOD_FLAG_HEAVILY_ARMORED;
                    *out_weapon_mods = mods;
                }

                space = DESIGN::Weapon_Space_(player_idx, *out_weapon_id, 1, 1, *out_weapon_count, mods);

                if ((available_space / space) < _min_weapon_count_table[weapon_count_table_idx]) {
                    if (mods & WEAPON_MOD_FLAG_FAST) {
                        mods ^= WEAPON_MOD_FLAG_FAST;
                        *out_weapon_mods = mods;
                    }

                    DESIGN::Weapon_Space_(player_idx, *out_weapon_id, 1, 1, *out_weapon_count, mods);
                }
            }
        }
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

        int32_t space_per_missile = DESIGN::Weapon_Space_(player_idx, (int16_t)weapon_type, 1, 1, modified_ammo, specials);

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

        int32_t final_space_coeff = DESIGN::Weapon_Space_(player_idx, (int16_t)weapon_type, 1, 1, modified_ammo, specials);
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

        if (hull_idx == SHIP_SIZE_DOOM_STAR && (player->tech_applications[TECH_APP_STELLAR_CONVERTER] == TECH_RESEARCH_STATUS_RESEARCHED)) {
            if (game_random::Random_(4) < 4) {
                ship_class = 6;
            }
        }

        if (player->tech_applications[TECH_APP_BLACK_HOLE_GENERATOR] == TECH_RESEARCH_STATUS_RESEARCHED && hull_idx > SHIP_SIZE_HUGE) {
            if (game_random::Random_(4) < 4) {
                ship_class = 6;
            }
        }

        struct_::Clear_Structure_(design, 99);

        if (hull_idx == SHIP_SIZE_DOOM_STAR) {
            design->picture_num = 0x2b; /* '+' */
            strlcpy(design->name, TECHDATA::_hull_data[SHIP_SIZE_DOOM_STAR].name, sizeof(design->name));
        } else {
            design->picture_num = (int8_t)game_random::Random_(8) + (int8_t)hull_idx * 8 - 1;
            uint8_t player_race = player->race;
            DESIGN::Get_Ship_Design_Name_(player_race, hull_idx, (int16_t)(design->picture_num % 8), design->name, sizeof(design->name));
        }

        if (ship_class == 7) {
            KEN::Ken_Get_Text_Message_(0x54, temp_name, 0xf);
            strlcpy(design->name, temp_name, sizeof(design->name));
        }

        design->ship_type = 0;
        design->size = (int8_t)hull_idx;
        design->shield_type = (int8_t)INITSHIP::Best_Ship_Shield_(player_idx);
        design->ftl_type = (int8_t)INITSHIP::Best_Warp_Drive_(player_idx);
        design->speed = (int8_t)INITSHIP::Get_FTL_Speed_(player_idx, (int16_t)design->ftl_type);
        design->computer_type = (int8_t)INITSHIP::Best_Computer_(player_idx);
        design->armor_type = (int8_t)INITSHIP::Best_Armor_(player_idx);

        remaining_space = TECHDATA::_hull_data[hull_idx].size;

        if (player->tech_applications[TECH_APP_BATTLE_PODS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            remaining_space += remaining_space / 2;
            struct_::Set_Bit_Field_(design->special_device_flags, SPECIAL_BATTLE_PODS);
        }

        remaining_space -= DESIGN::Shield_Space_Given_M_Level_((int16_t)design->shield_type, (int16_t)design->size, player_idx);

        const int16_t table_class = (ship_class == 7) ? 0 : ship_class;
        const s_aidesign_ship_design* table = &_ship_design_tables[table_class][hull_idx];
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
                if (player->tech_applications[TECH_APP_EXTENDED_FUEL_TANKS] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    struct_::Set_Bit_Field_(design->special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS);
                    int8_t cat = DESIGN::Miniaturization_Category_(TECH_APP_EXTENDED_FUEL_TANKS);
                    int16_t m_level = DESIGN::Miniaturization_Level_(player_idx, TECH_APP_EXTENDED_FUEL_TANKS);
                    int16_t fuel_tank_base_space = TECHDATA::_specials[SPECIAL_EXTENDED_FUEL_TANKS].space_per_ship_type[0];
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

        design->combat_speed = (int8_t)TECHDATA::_drives[design->ftl_type].combat_speed_min[hull_idx];
        design->previous_owner = (uint8_t)player_idx;
        design->cost = DESIGN::Cost_Of_Design_(player_idx, design);
        design->date_of_design = (uint16_t)MOX::_stardate;
    }

    int16_t __cdecl Special_Legal_For_Design_(s_ship_design* design, int16_t tech_id) {
        int16_t result = 1;
        int16_t bit = 0;

        while (bit < 39 && result == 1) {
            if (struct_::Test_Bit_Field_(design->special_device_flags, bit)) {
                for (int i = 0; i < 6; ++i) {
                    if (TECHDATA::_specials[bit].exclude[i] == tech_id) {
                        result = 0;
                    }
                }
            }
            bit++;
        }

        if (design->shield_type == 0) {
            if (tech_id == SPECIAL_HARD_SHIELDS || tech_id == SPECIAL_MULTI_PHASED_SHIELDS || tech_id == SPECIAL_SHIELD_CAPACITOR) {
                result = 0;
            }
        }

        return result;
    }

}