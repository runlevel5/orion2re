#include "pch.h"

namespace CMBTFIR2 {
    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        if (special_skills & 0x40) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        if (special_skills & 0x80) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;

        if ((MOX::_leaders[leader_idx].special_skills & 0x4000) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((bonus + 1) * 5);
        }

        if ((MOX::_leaders[leader_idx].special_skills & 0x8000) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((bonus * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    int16_t __cdecl Engineer_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;

        // Check for specific leader special skills
        // Bit 0 corresponds to a skill that provides a flat (Level * 2 + 2) bonus
        if ((MOX::_leaders[leader_idx].special_skills & 0x01) != 0) {
            bonus = Owned_Officer_Level_(leader_idx);
            return bonus * 2 + 2;
        }

        // Bit 1 corresponds to a skill that provides a (Level + 1) * 3 bonus
        if ((MOX::_leaders[leader_idx].special_skills & 0x02) != 0) {
            bonus = Owned_Officer_Level_(leader_idx);
            bonus = (bonus + 1) * 3;
        }

        return bonus;
    }

    int16_t __cdecl Fighter_Pilot_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        if (special_skills & 0x4 /* FIGHTER_PILOT */) {
            int16_t level = Owned_Officer_Level_(leader_idx);
            return (level + 1) * 5;
        }

        if (special_skills & 0x8 /* ACE_PILOT */) {
            int16_t level = Owned_Officer_Level_(leader_idx);
            // Calculation: ((level * 3 + 3) * 5) / 2
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    /**
     * @brief Calculates the Galactic Lore bonus for a leader.
     * @param leader_idx Index of the leader in the global leaders array.
     * @return The calculated bonus value.
     */
    int16_t __cdecl Galactic_Lore_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        // Check for bit 0x10 (likely Galactic Lore skill)
        if (special_skills & 0x10) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        // Check for bit 0x20 (likely Advanced Galactic Lore skill)
        if (special_skills & 0x20) {
            int32_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            // Calculation: ((level * 3 + 3) * 5) / 2
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    void __cdecl Set_Combat_Ordnance_Values_() {
        int16_t attacker_idx = MOX::_combat_attacker_player;
        int16_t attacker_bonus = CMBTFIR2::Get_Fleet_Ordnance_Bonus_(attacker_idx) + 100;

        // Load defender index before storing attacker result to match register usage (SI)
        int16_t defender_idx = MOX::_combat_defender_player;
        MOX::_ordnance_bonus[attacker_idx] = attacker_bonus;

        int16_t defender_bonus = CMBTFIR2::Get_Fleet_Ordnance_Bonus_(defender_idx) + 100;
        MOX::_ordnance_bonus[defender_idx] = defender_bonus;
    }

    int16_t __cdecl Crew_Defense_Bonus_(int16_t combat_idx) {
        int8_t quality = MOX::_combat_data[combat_idx].crew_quality;
        return MOX::_crew_data[quality].defense;
    }

    short __cdecl Fleet_Defensive_Bonus_(short player_idx, short mode, short* bonus_val_ptr) {
        int16_t max_helmsman_bonus = 0;

        if (mode == 1) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                s_combat_data* ship = &MOX::_combat_data[i];
                if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(i, 38 /* 0x26 */)) {
                        *bonus_val_ptr = 70 /* 0x46 */;
                    }
                    if (ship->officer_idx > -1) {
                        int16_t bonus = Helmsman_Bonus_(ship->officer_idx);
                        if (bonus > max_helmsman_bonus) {
                            max_helmsman_bonus = bonus;
                        }
                    }
                }
            }
            return (max_helmsman_bonus / 2) + *bonus_val_ptr;
        } else {
            int16_t extra_bonus = 0;
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                s_combat_data* ship = &MOX::_combat_data[i];
                if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                    if (ship->officer_idx > -1) {
                        int16_t bonus = Helmsman_Bonus_(ship->officer_idx);
                        if (bonus > max_helmsman_bonus) {
                            max_helmsman_bonus = bonus;
                        }
                    }
                    // This block appears to be a no-op but is preserved from the original binary
                    if (ship->ftl_type == 0) {
                        if (ship->size_class == 2 || ship->size_class == 3 || ship->size_class == 4) {
                            extra_bonus = 0;
                        }
                    }
                }
            }
            return max_helmsman_bonus + extra_bonus;
        }
    }

    int16_t __cdecl Defensive_Combat_Bonus_(int16_t combat_unit_idx, int16_t target_type) {
        int16_t specials_base = 0;
        int16_t fleet_base = 0;
        int16_t trait_bonus = 0;
        int16_t speed_bonus;
        int16_t crew_bonus = CMBTFIR2::Crew_Defense_Bonus_(combat_unit_idx);

        if (target_type == 0) {
            speed_bonus = MOX::_combat_data[combat_unit_idx].current_speed * 5;
            if (MOX::_combat_data[combat_unit_idx].current_speed == 0) {
                speed_bonus = -20;
            }
        } else {
            speed_bonus = 0;
            crew_bonus /= 2;
        }

        int16_t specials_bonus = CMBTFIR2::Ship_Specials_Defensive_Bonus_(combat_unit_idx, target_type, &specials_base);
        int8_t owner = MOX::_combat_data[combat_unit_idx].owner;
        int16_t fleet_bonus = CMBTFIR2::Fleet_Defensive_Bonus_(owner, target_type, &fleet_base);

        if (fleet_base > specials_base) {
            specials_bonus -= specials_base;
        } else {
            fleet_bonus -= fleet_base;
        }

        if (MOX::_combat_data[combat_unit_idx].ftl_type > 0 && owner < 8) {
            // Index 6 corresponds to TRAIT_SHIP_DEFENSE
            trait_bonus = (int16_t)MOX::_player[owner].traits[6 /* TRAIT_SHIP_DEFENSE */];
        }

        return fleet_bonus + trait_bonus + speed_bonus + specials_bonus + crew_bonus;
    }

    int16_t __cdecl Computer_Bonus_(int16_t ship_idx) {
        if (MOX::_combat_data[ship_idx].computer_current_hits < 1) {
            return 0;
        }

        return TECHDATA::_computers[MOX::_combat_data[ship_idx].computer_type].bonus;
    }

    int16_t __cdecl Crew_Attack_Bonus_(int16_t ship_idx) {
        int8_t quality = MOX::_combat_data[ship_idx].crew_quality;
        return MOX::_crew_data[quality].attack;
    }

    int16_t __cdecl Ship_Specials_Offensive_Bonus_(int16_t ship_idx) {
        int16_t bonus = 0;

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 5 /* SPEC_STRUCTURAL_ANALYZER */) != 0) {
            bonus = 50;
        }

        return bonus;
    }

    short __cdecl Fleet_Offensive_Bonus_(short player_idx) {
        int16_t size_bonus = 0;
        int16_t special_bonus = 0;
        int16_t max_weaponry_bonus = 0;
        int16_t max_lore_bonus = 0;
        int16_t is_ancient = 0;

        if (MOX::_combat_attacker_player > 8 || MOX::_combat_defender_player > 8) {
            is_ancient = 1;
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data* ship = &MOX::_combat_data[i];

            if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                if (ship->ftl_type == 0) {
                    if (ship->size_class == 2) {
                        size_bonus = 0;
                    } else if (ship->size_class == 3) {
                        size_bonus = 10;
                    } else if (ship->size_class == 4) {
                        size_bonus = 20;
                    }
                }

                int16_t officer_idx = ship->officer_idx;
                if (officer_idx > -1) {
                    int16_t w_bonus = CMBTFIR2::Weaponry_Bonus_(officer_idx);
                    if (w_bonus > max_weaponry_bonus) {
                        max_weaponry_bonus = w_bonus;
                    }

                    if (is_ancient != 0) {
                        int16_t l_bonus = CMBTFIR2::Galactic_Lore_Bonus_(officer_idx);
                        if (l_bonus > max_lore_bonus) {
                            max_lore_bonus = l_bonus;
                        }
                    }
                }

                if (is_ancient == 1) {
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(i, 28 /* 0x1c */)) {
                        special_bonus = 20;
                    }
                }
            }
        }

        return max_lore_bonus + size_bonus + max_weaponry_bonus + special_bonus;
    }

    int16_t __cdecl Offensive_Combat_Bonus_(int16_t ship_idx) {
        int16_t computer_bonus;
        int16_t crew_bonus;
        int16_t specials_bonus;
        int16_t fleet_bonus;
        int16_t trait_bonus;
        int8_t owner;

        computer_bonus = Computer_Bonus_(ship_idx);
        crew_bonus = Crew_Attack_Bonus_(ship_idx);
        specials_bonus = Ship_Specials_Offensive_Bonus_(ship_idx);

        owner = MOX::_combat_data[ship_idx].owner;
        fleet_bonus = Fleet_Offensive_Bonus_(owner);

        trait_bonus = 0;
        if (owner < 8) {
            trait_bonus = (int16_t)MOX::_player[owner].traits[7 /* TRAIT_SHIP_ATTACK */];
        }

        return computer_bonus + crew_bonus + trait_bonus + fleet_bonus + specials_bonus;
    }

    int16_t __cdecl Being_Captured_(int16_t ship_idx) {
        return (int16_t)struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 4);
    }

    void __cdecl Set_Capturing_Flag_(int16_t unit_idx) {
        struct_::Set_Bit_Field_(&MOX::_combat_data[unit_idx].combat_status_flags, 4);
    }

    void __cdecl Set_Boarded_Flag_(int16_t ship_idx) {
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 1);
    }

    int16_t __cdecl Already_Boarded_(int16_t ship_idx) {
        return (int16_t)struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 1);
    }

    void __cdecl Set_Combat_Ship_Done_(int16_t ship_idx) {
        // Sets bit 2 (Turn Finished) in the combat status flags
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 2);
    }

    int16_t __cdecl Used_Teleporter_(int16_t ship_idx) {
        return (int16_t)struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 0);
    }

    void __cdecl Set_Teleporter_Used_(int16_t ship_idx) {
        // Bit 0 of combat_status_flags indicates if teleporter was used
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 0);
    }

    int16_t __cdecl Get_Fleet_Ordnance_Bonus_(int16_t owner) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].owner == (int8_t)owner &&
                MOX::_combat_data[i].is_captured == 0 &&
                MOX::_combat_data[i].unit_status == 0 &&
                MOX::_combat_data[i].officer_idx != -1) {

                int16_t bonus = Leader_Ordnance_Bonus_(MOX::_combat_data[i].officer_idx);

                if (bonus > max_bonus) {
                    max_bonus = bonus;
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Get_Fleet_Engineer_Bonus_(int16_t player_idx) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data* ship = &MOX::_combat_data[i];

            // Only check ships belonging to the player that are not captured and active
            if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                if (ship->officer_idx > -1) {
                    int16_t current_bonus = CMBTFIR2::Engineer_Bonus_(ship->officer_idx);
                    if (current_bonus > max_bonus) {
                        max_bonus = current_bonus;
                    }
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Get_Fleet_Pilot_Bonus_(int16_t owner_idx) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            // MOX::_combat_data is pointer to array of s_combat_data (size 313 / 0x139)
            s_combat_data& ship = MOX::_combat_data[i];

            if (ship.owner == (int8_t)owner_idx) {
                if (ship.is_captured == 0) {
                    if (ship.unit_status == 0 /* ACTIVE */) {
                        if (ship.officer_idx != -1) {
                            int16_t bonus = Fighter_Pilot_Bonus_(ship.officer_idx);
                            if (bonus > max_bonus) {
                                max_bonus = bonus;
                            }
                        }
                    }
                }
            }
        }

        return max_bonus;
    }

    int16_t Leader_Ordnance_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        
        if ((MOX::_leaders[leader_idx].special_skills & 0x400) != 0) {
            bonus = (Owned_Officer_Level_(leader_idx) + 1) * 5;
        } else if ((MOX::_leaders[leader_idx].special_skills & 0x800) != 0) {
            int16_t level = Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }
        
        return bonus;
    }

    int16_t __cdecl Max_Defensive_Fire_Range_(int16_t ship_idx) {
        int16_t min_range = -1;

        for (int i = 0; i < 8; ++i) {
            uint16_t weapon_id = MOX::_combat_data[ship_idx].weapons[i].weapon_id;
            if ((int16_t)weapon_id > 0 &&
                TECHDATA::_weapons[(int16_t)weapon_id].rack == 0 &&
                (MOX::_combat_data[ship_idx].weapons[i].specials & 6) == 0 &&
                MOX::_combat_data[ship_idx].weapons[i].shots_left > 0) {
                min_range = 24;
            }
        }

        if (min_range > 0 && MOX::_combat_total_ship_count > 1) {
            for (int i = 1; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[ship_idx].owner != MOX::_combat_data[i].owner &&
                    MOX::_combat_data[i].is_captured == 0 &&
                    MOX::_combat_data[i].unit_status == 0) {
                    int16_t move_cost = CMBTMOV1::Move_Cost_(i, (int16_t)MOX::_combat_data[ship_idx].x, (int16_t)MOX::_combat_data[ship_idx].y);
                    int16_t current_speed = COMBAT1::Calc_Current_Speed_(i);
                    int16_t range = 1;
                    if (move_cost - current_speed > 1) {
                        range = move_cost - current_speed;
                    }
                    if (range < min_range) {
                        min_range = range;
                    }
                }
            }
        }
        return min_range;
    }

    bool __cdecl Apply_Combat_Artemis_Net_(char strategic) {
        int16_t ship_idx = 1;
        bool is_active = true;

        if (ship_idx < MOX::_combat_total_ship_count) {
            do {
                int16_t size_class;
                if (strategic == 0) {
                    size_class = MOX::_combat_data[ship_idx].size_class;
                } else {
                    size_class = MOX::_qcombat_data[ship_idx].size;
                }

                int16_t chance = _artemis_net_chance[size_class];
                int32_t roll = random::Random_(100);

                if (chance < roll) {
                    is_active = false;
                } else if (strategic == 0) {
                    // Tactical combat
                    for (int i = 0; i < 4; ++i) {
                        MOX::_combat_data[ship_idx].shield_arc_current[i] = 0;
                    }

                    int32_t total_damage = random::Random_(0x191) + 0x95; // Random(401) + 149
                    
                    while (total_damage > 0 && !CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx)) {
                        int16_t damage = 0x14; // 20
                        int16_t sys_damage = 0;
                        CMBTFIRE::Apply_Ship_Damage_(ship_idx, &damage, &sys_damage, 0, 0, 0, 0, 0, 0);
                        total_damage -= 0x14;
                    }

                    if (!CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx)) {
                        is_active = false;
                        int16_t shield_max = COMBINIT::Calc_Combat_Shield_Max_(ship_idx);
                        for (int i = 0; i < 4; ++i) {
                            MOX::_combat_data[ship_idx].shield_arc_current[i] = shield_max;
                        }
                    } else {
                        MOX::_combat_data[ship_idx].unit_status = 5; // Destroyed
                    }
                } else {
                    // Strategic combat
                    int16_t shield_id = MOX::_qcombat_data[ship_idx].shield_id;
                    int16_t damage = (int16_t)(random::Random_(0xC9) + 0x4A); // Random(201) + 74
                    
                    MOX::_qcombat_data[ship_idx].shield_id = 0;
                    COMBAT::Apply_Damage_To_Qship_(ship_idx, damage);
                    MOX::_qcombat_data[ship_idx].shield_id = shield_id;

                    if (MOX::_qcombat_data[ship_idx].status != 1) { // 1 = Destroyed?
                        is_active = false;
                    } else {
                        MOX::_qcombat_data[ship_idx].status = 2; // Inactive?
                        MOX::_combat_attacker_count--;
                    }
                }
                ship_idx++;
            } while (ship_idx < MOX::_combat_total_ship_count);
        }
        return is_active;
    }

    short __cdecl Max_Total_Internal_Space_(short ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        // Assembly uses signed division (CDQ, SUB, SAR) which corresponds to / operator.
        int16_t total_space = ship->drive_max_hits / 2;
        total_space += ship->computer_max_hits;

        for (int i = 0; i < 8; ++i) {
            total_space += ship->weapon_total_hits[i];
        }

        for (int i = 0; i < 8; ++i) {
            total_space += ship->special_device_hits[i];
        }

        total_space += ship->shield_max_hits;

        return total_space;
    }

    void __cdecl Init_Internal_Space_(s_combat_data* ship) {
        if (ship->ftl_type <= 0) {
            ship->drive_max_hits = 0;
        } else {
            ship->drive_max_hits = TECHDATA::_hull_data[ship->size_class].drive_hits;
        }

        if (struct_::Test_Bit_Field_(ship->special_device_flags, 27 /* SPEC_STRUCTURAL_ANALYZER */) &&
            !struct_::Test_Bit_Field_(ship->special_device_damage_flags, 27 /* SPEC_STRUCTURAL_ANALYZER */)) {
            ship->drive_max_hits *= 3;
        }

        ship->computer_max_hits = TECHDATA::_hull_data[ship->size_class].computer_hits;

        for (int i = 0; i < 8; ++i) {
            if (ship->weapons[i].weapon_id < 1) {
                ship->weapon_unit_hits[i] = 0;
                ship->weapon_total_hits[i] = 0;
            } else {
                int16_t space = DESIGN::One_Weapon_Space_Without_M_Level_(
                    ship->weapons[i].weapon_id,
                    ship->weapons[i].arc,
                    ship->weapons[i].specials,
                    ship->weapons[i].ammo
                );

                int16_t hits = space / 10;
                if (hits < 2) {
                    hits = 1;
                }
                ship->weapon_unit_hits[i] = hits;
                ship->weapon_total_hits[i] = (int16_t)ship->weapons[i].count * hits;
            }
        }

        memset(ship->special_device_hits, 0, sizeof(ship->special_device_hits));

        int device_idx = 0;
        for (int bit = 0; bit < 39; ++bit) {
            if (device_idx >= 8) {
                break;
            }

            if (struct_::Test_Bit_Field_(ship->special_device_flags, bit) &&
                !struct_::Test_Bit_Field_(ship->special_device_damage_flags, bit)) {

                if (bit == 27 /* SPEC_STRUCTURAL_ANALYZER */ || bit == 11 /* SPEC_HEAVIER_ARMOR */ || 
                    bit == 4 /* SPEC_BATTLE_PODS */ || bit == 14 /* SPEC_REINFORCED_HULL */ || 
                    bit == 24 /* SPEC_STEALTH_FIELD */) {
                    ship->special_device_hits[device_idx] = 0;
                } else {
                    int16_t hits = TECHDATA::_specials[bit].space_per_ship_type[ship->size_class] / 10;
                    if (hits < 2) {
                        hits = 1;
                    }
                    ship->special_device_hits[device_idx] = hits;
                }
                device_idx++;
            }
        }

        if (ship->shield_type <= 0) {
            ship->shield_max_hits = 0;
        } else {
            ship->shield_max_hits = TECHDATA::_hull_data[ship->size_class].shield_hits;
        }

        CMBTFIR2::Clear_Internal_Space_(ship);
    }

    void __cdecl Repair_Combat_Ship_(int16_t ship_idx, int16_t amount) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];
        int16_t armor_hits = 0;

        if (ship_idx == 0) {
            if (ship->structure_damage > 0) {
                int32_t repair = (amount * ship->structure_damage) / 100;
                if (repair < 2) repair = 1;
                ship->structure_damage -= (int16_t)repair;
                return;
            }
            if (ship->structure_max > 0) {
                int32_t repair = (amount * ship->structure_max) / 100;
                if (repair < 2) repair = 1;
                ship->structure_max -= (int16_t)repair;
                return;
            }
            return;
        }

        if (ship->ship_idx >= 0) {
            armor_hits = INITSHIP::Get_Ship_Armor_Hits_(ship->ship_idx);
        } else {
            int8_t size = ship->size_class;
            if (ship->ftl_type == 0) {
                if (size == 2) {
                    armor_hits = TECHDATA::_hull_data[6].armor_absorption + 
                                 (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].armor_absorption_percent * 
                                           TECHDATA::_hull_data[6].armor_absorption / 100);
                } else if (size == 3) {
                    armor_hits = TECHDATA::_hull_data[7].armor_absorption + 
                                 (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].armor_absorption_percent * 
                                           TECHDATA::_hull_data[6].armor_absorption / 100);
                } else if (size == 4) {
                    armor_hits = TECHDATA::_hull_data[8].armor_absorption + 
                                 (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].armor_absorption_percent * 
                                           TECHDATA::_hull_data[6].armor_absorption / 100);
                }
            } else {
                armor_hits = TECHDATA::_hull_data[size].armor_absorption + 
                             (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].armor_absorption_percent * 
                                       TECHDATA::_hull_data[size].armor_absorption / 100);
            }
            
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 14 /* SPECIAL_REINFORCED_HULL */)) {
                armor_hits *= 3;
            }
        }

        int32_t hull_repair = (amount * (ship->structure_max + armor_hits)) / 100;
        if (hull_repair < 1) hull_repair = 1;

        ship->structure_damage -= (int16_t)hull_repair;
        if (ship->structure_damage < 0) {
            ship->armor_remaining -= ship->structure_damage;
            ship->structure_damage = 0;
            if (ship->armor_remaining > armor_hits) {
                ship->armor_remaining = armor_hits;
            }
        }

        int32_t pool = (amount * Max_Total_Internal_Space_(ship_idx)) / 200;
        if (pool < 2) pool = 1;

        if (ship->drive_current_hits < ship->drive_max_hits && pool > 0) {
            int16_t needed = ship->drive_max_hits - ship->drive_current_hits;
            int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
            ship->drive_current_hits += amt;
            pool -= (int32_t)amt;
        }

        if (ship->computer_current_hits < ship->computer_max_hits && pool > 0) {
            int16_t needed = ship->computer_max_hits - ship->computer_current_hits;
            int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
            ship->computer_current_hits += amt;
            pool -= (int32_t)amt;
        }

        for (int i = 0; i < 8 && pool > 0; i++) {
            if (ship->weapon_hits[i] < ship->weapon_total_hits[i]) {
                int16_t needed = ship->weapon_total_hits[i] - ship->weapon_hits[i];
                int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
                ship->weapon_hits[i] += amt;
                pool -= (int32_t)amt;
            }
        }

        if (ship->shield_current_hits < ship->shield_max_hits && pool > 0) {
            int16_t needed = (int16_t)ship->shield_max_hits - ship->shield_current_hits;
            int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
            ship->shield_current_hits += amt;
            pool -= (int32_t)amt;
        }

        for (int i = 0; i < 8 && pool > 0; i++) {
            if (ship->special_hits[i] < ship->special_device_hits[i]) {
                int16_t needed = ship->special_device_hits[i] - ship->special_hits[i];
                int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
                pool -= (int32_t)amt;
                // Note: Logic in binary sets hits to max even if pool was insufficient
                ship->special_hits[i] = ship->special_device_hits[i];
            }
        }
    }

    int16_t __cdecl Ship_Specials_Defensive_Bonus_(int16_t ship_idx, int16_t target_type, int16_t* field_bonus_out) {
        int16_t total_bonus = 0;
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        // target_type: 0 = Ship, 1 = Missile/Other
        if (ship->special_status_flag == 1 /* STATUS_PHANTOM_SHADOW */ && target_type == 0) {
            total_bonus = 80;
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x26 /* SPECIAL_DISPLACEMENT_FIELD */)) {
            if (target_type == 1) {
                total_bonus += 130;
                *field_bonus_out = 130;
            }
        } else if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x15 /* SPECIAL_INERTIAL_NULLIFIER */)) {
            if (target_type == 1) {
                total_bonus += 100;
                *field_bonus_out = 100;
            }
        } else if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x09 /* SPECIAL_INERTIAL_STABILIZER */)) {
            if (target_type == 1) {
                total_bonus += 70;
                *field_bonus_out = 70;
            }
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x11 /* SPECIAL_CLOAKING_DEVICE */)) {
            if (target_type == 0) {
                total_bonus += 100;
            }
            if (target_type == 1) {
                total_bonus += 50;
            }
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x12 /* SPECIAL_STEALTH_FIELD */)) {
            if (target_type == 0) {
                total_bonus += 50;
            }
            if (target_type == 1) {
                total_bonus += 25;
            }
        }

        return total_bonus;
    }

    void __cdecl Clear_Internal_Space_(s_combat_data* combat_data) {
        // This function copies 54 bytes (27 int16_t elements) of hit data 
        // from the maximum hit values to the current hit values.
        memcpy(&combat_data->drive_current_hits, &combat_data->drive_max_hits, 54);
    }

    void __cdecl Final_Fire_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx) {
        int16_t center_x_offset;
        int16_t center_y_offset;
        int16_t saved_active[8];

        COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x_offset, &center_y_offset);

        if (CMBTAI::Will_Missile_Do_Damage_(ship_idx, missile_idx) != 0) {
            int16_t m_type = MOX::_missile[missile_idx].type;
            if (TECHDATA::_weapons[m_type].rack != 2 && MOX::_missile[missile_idx].quantity > 0) {
                for (int16_t i = 0; i < 8; ++i) {
                    saved_active[i] = (int16_t)MOX::_combat_data[ship_idx].weapons[i].active;
                    MOX::_combat_data[ship_idx].weapons[i].active = -1;
                }

                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_missile[missile_idx].type <= 0) {
                        break;
                    }

                    int16_t weapon_id = MOX::_combat_data[ship_idx].weapons[i].weapon_id;
                    bool can_fire_pulsar = false;

                    if (weapon_id == 13 /* PULSAR */) {
                        if (saved_active[i] != -1 && CMBTAI::Safe_To_Fire_Sphere_Weapon_(ship_idx)) {
                            can_fire_pulsar = true;
                        }
                    }

                    MOX::_combat_data[ship_idx].weapons[i].active = 1;

                    bool should_fire = false;
                    if (TECHDATA::_weapons[weapon_id].rack == 0) {
                        if (MOX::_combat_data[ship_idx].weapons[i].specials & 4) {
                            should_fire = true;
                        }
                    } else {
                        should_fire = false;
                    }

                    if (should_fire || can_fire_pulsar || weapon_id == 33 /* BLACK_HOLE_GENERATOR */) {
                        int16_t rel_target_x = target_x - center_x_offset;
                        int16_t rel_target_y = target_y - center_y_offset;

                        if (weapon_id == 13 /* PULSAR */) {
                            if ((rel_target_x % 20 == 0) && (rel_target_y % 20 == 0)) {
                                if ((int16_t)MOX::_combat_data[ship_idx].x == (rel_target_x / 20) &&
                                    (int16_t)MOX::_combat_data[ship_idx].y == (rel_target_y / 20)) {
                                    CMBTFIRE::Fire_Ship_(ship_idx, rel_target_x, rel_target_y, missile_idx,
                                                        MOX::_missile[missile_idx].x, MOX::_missile[missile_idx].y, 1);
                                }
                            }
                        } else {
                            CMBTFIRE::Fire_Ship_(ship_idx, rel_target_x, rel_target_y, missile_idx,
                                                MOX::_missile[missile_idx].x, MOX::_missile[missile_idx].y, 1);
                        }
                    }

                    MOX::_combat_data[ship_idx].weapons[i].active = -1;
                }

                for (int i = 0; i < 8; ++i) {
                    MOX::_combat_data[ship_idx].weapons[i].active = (int8_t)saved_active[i];
                }
            }
        }
    }

    void __cdecl Defensive_Fire_Check_(int16_t target_idx, int16_t target_x, int16_t target_y, int16_t dest_x, int16_t dest_y) {
        int16_t center_off_x;
        int16_t center_off_y;
        int16_t range1;
        int16_t range2;
        int16_t active_backup[8];

        if (MOX::_combat_data[target_idx].special_status_flag != 4 /* STASIS_FIELD */) {
            for (int16_t attacker_idx = 0; attacker_idx < MOX::_combat_total_ship_count; ++attacker_idx) {
                if (CMBTFIRE::Is_Combat_Ship_Dead_(target_idx)) {
                    return;
                }

                if (MOX::_combat_data[attacker_idx].owner != MOX::_combat_data[target_idx].owner) {
                    if (COMBAT1::Combat_Ship_Is_Functional_(attacker_idx)) {
                        int8_t status = MOX::_combat_data[attacker_idx].special_status_flag;
                        if (status != 1 && status != 4 && MOX::_combat_data[attacker_idx].ai_range_tactic > 0) {
                            COMBINIT::Ship_Center_Offsets_(target_idx, &center_off_x, &center_off_y);
                            range1 = CMBTAI::Range_To_XY_(attacker_idx, target_x + center_off_x, target_y + center_off_y);
                            range2 = CMBTAI::Range_To_XY_(attacker_idx, dest_x + center_off_x, dest_y + center_off_y);

                            if (range1 < 1) {
                                range1 = 1;
                            }

                            if (range1 <= 8 && range1 <= range2) {
                                int32_t threshold_denom = ((-2 - MOX::_combat_data[attacker_idx].ai_range_tactic) / 3) + 9;
                                int16_t threshold = (int16_t)((100 * (9 - range1)) / threshold_denom);

                                if (random::Random_(100) <= threshold) {
                                    for (int16_t i = 0; i < 8; ++i) {
                                        active_backup[i] = MOX::_combat_data[attacker_idx].weapons[i].active;
                                        int16_t weapon_id = MOX::_combat_data[attacker_idx].weapons[i].weapon_id;

                                        if (TECHDATA::_weapons[weapon_id].rack == 0 &&
                                            active_backup[i] != -1 &&
                                            (MOX::_combat_data[attacker_idx].weapons[i].specials & 6) == 0) {

                                            if (CMBTAI::Expected_Weapon_Damage_(attacker_idx, i, target_idx, 0, range1) > 0) {
                                                MOX::_combat_data[attacker_idx].weapons[i].active = 1;
                                            } else {
                                                MOX::_combat_data[attacker_idx].weapons[i].active = -1;
                                            }
                                        } else {
                                            MOX::_combat_data[attacker_idx].weapons[i].active = -1;
                                        }
                                    }

                                    CMBTFIRE::Fire_Ship_(attacker_idx, 
                                                         MOX::_combat_data[attacker_idx].x * 20, 
                                                         MOX::_combat_data[attacker_idx].y * 20, 
                                                         target_idx, target_x, target_y, 0);

                                    MOX::_combat_data[attacker_idx].ai_range_tactic = (int8_t)CMBTFIR2::Max_Defensive_Fire_Range_(attacker_idx);

                                    for (int16_t i = 0; i < 8; ++i) {
                                        MOX::_combat_data[attacker_idx].weapons[i].active = (int8_t)active_backup[i];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void __cdecl Kill_Crew_(int16_t combat_data_idx, int16_t kill_count) {
        s_combat_data& ship = MOX::_combat_data[combat_data_idx];

        // The assembly treats current marine_count as unsigned (0-255) for comparison
        if ((int16_t)(uint8_t)ship.marine_count < kill_count) {
            ship.marine_count = 0;
        } else {
            ship.marine_count -= (int8_t)kill_count;
        }
    }

    int16_t __cdecl Init_Boarding_Action_(int16_t unit_idx, int16_t* param_2) {
        char message_buffer[80];
        int16_t boarding_strength;

        KEN::Ken_Get_Text_Message_(41 /* 0x29 */, message_buffer, 80 /* 0x50 */);

        boarding_strength = (MOX::_combat_data[unit_idx].marine_count + 1) / 2;

        return CMBTDRW1::Board_Popup_(0, boarding_strength, message_buffer, param_2);
    }

    int16_t __cdecl Combat_Ship_Done_(int16_t ship_idx) {
        return struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 2);
    }
}